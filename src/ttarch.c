#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <zlib.h>
#include <assert.h>
#include "blowfish.h"
#include "crc64.h"

struct __attribute__((__packed__)) FileHeader // This has to be packed otherwise the OS will try to use 32 bytes instead of 28 bytes for it
{
    uint64_t crcName; // CRC64 EMCA 182 format
    uint64_t offset;  // How many bytes after the name table the file is located
    uint32_t size;
    uint32_t unkownBytes;
    uint16_t nameTableChunkIndex;
    uint16_t nameTableOffset;
};

struct ArchiveHeader
{
    uint32_t version;
    uint32_t nameSize; // The size of the nameTable
    uint32_t fileCount;
};

struct Archive
{
    struct ArchiveHeader header;
    struct FileHeader *entries;
    uint8_t *fileNames;
    uint8_t *fileData;
};

struct CompressedHeader
{
    uint32_t version;
    uint32_t chunkDecompressedSize;
    uint32_t chunkCount;
};

struct CompressedArchive
{
    struct CompressedHeader header;
    uint64_t *chunkOffset;
    uint8_t *chunkData;
};

int ZlibDecompress(void *source, unsigned int sourceLen, const void *dest, unsigned int *destLen)
{
    z_stream stream;
    int err;
    const uInt max = (uInt)-1;
    uLong len, left;
    unsigned char buf[1];

    len = sourceLen;
    if (*destLen)
    {
        left = *destLen;
        *destLen = 0;
    }
    else
    {
        left = 1;
        dest = buf;
    }

    stream.next_in = (z_const Bytef *)source;
    stream.avail_in = 0;
    stream.zalloc = (alloc_func)0;
    stream.zfree = (free_func)0;
    stream.opaque = (voidpf)0;

    err = inflateInit2(&stream, -15);
    if (err != Z_OK)
        return err;

    stream.next_out = (Bytef *)dest;
    stream.avail_out = 0;

    do
    {
        if (stream.avail_out == 0)
        {
            stream.avail_out = left > (uLong)max ? max : (uInt)left;
            left -= stream.avail_out;
        }
        if (stream.avail_in == 0)
        {
            stream.avail_in = len > (uLong)max ? max : (uInt)len;
            len -= stream.avail_in;
        }
        err = inflate(&stream, Z_NO_FLUSH);
    } while (err == Z_OK);

    sourceLen -= len + stream.avail_in;
    if (dest != buf)
        *destLen = stream.total_out;
    else if (stream.total_out && err == Z_BUF_ERROR)
        left = 1;

    inflateEnd(&stream);
    return err == Z_STREAM_END ? 1 /*Z_OK*/ : err == Z_NEED_DICT                          ? 0 /*Z_DATA_ERROR*/
                                          : err == Z_BUF_ERROR && left + stream.avail_out ? 0 /*Z_DATA_ERROR*/
                                                                                          : 0 /*err*/;
}

void endianSwapUint64(uint64_t *value)
{
    *value = ((*value & 0x00000000000000FFULL) << 56) |
             ((*value & 0x000000000000FF00ULL) << 40) |
             ((*value & 0x0000000000FF0000ULL) << 24) |
             ((*value & 0x00000000FF000000ULL) << 8) |
             ((*value & 0x000000FF00000000ULL) >> 8) |
             ((*value & 0x0000FF0000000000ULL) >> 24) |
             ((*value & 0x00FF000000000000ULL) >> 40) |
             ((*value & 0xFF00000000000000ULL) >> 56);
}

int archiveDecrypt(uint8_t *archivePath, uint8_t *outPath)
{
    int err;

    printf("Archive Extract\n");
    uint8_t *buffer = malloc(sizeof(struct CompressedHeader));

    if (buffer == NULL)
    {
        printf("alloc failed\n");
    }

    FILE *compressedFile = fopen(archivePath, "rb");
    FILE *decompressedFile = fopen(outPath, "wb");

    fread(buffer, sizeof(struct CompressedHeader), 1, compressedFile);
    struct CompressedHeader *header = (struct CompressedHeader *)buffer;

    printf("version = 0x%x\n", header->version);
    printf("chunk size = 0x%x\n", header->chunkDecompressedSize);
    printf("found %d compressed chunks\n", header->chunkCount);

    uint8_t *compressedChunk = malloc(header->chunkDecompressedSize); // This compressedChunk buffer will probably have more space than the space required for the chunk
    uint8_t *decompressedChunk = malloc(header->chunkDecompressedSize);

    uint64_t *chunkOffsets = malloc(sizeof(uint64_t) * header->chunkCount);
    fread(chunkOffsets, sizeof(uint64_t), header->chunkCount, compressedFile);

    fseek(compressedFile, chunkOffsets[0], SEEK_SET);
    for (uint32_t i = 1; i < header->chunkCount; ++i)
    {
        fread(compressedChunk, chunkOffsets[i] - chunkOffsets[i - 1], 1, compressedFile);

        decryptData7((uint64_t *)compressedChunk, (chunkOffsets[i] - chunkOffsets[i - 1]) / sizeof(uint64_t));

        uint32_t size = header->chunkDecompressedSize;
        ZlibDecompress(compressedChunk, chunkOffsets[i] - chunkOffsets[i - 1], decompressedChunk, &size);
        // printf("%d", size);
        fwrite(decompressedChunk, header->chunkDecompressedSize, 1, decompressedFile);
    }

    if (fclose(decompressedFile) == EOF)
    {
        return EOF;
    }
    if (fclose(compressedFile) == EOF)
    {
        return EOF;
    }
    free(buffer);
    free(decompressedChunk);
    free(compressedChunk);
    return 0;
}

int archiveSplit(uint8_t *archivePath, uint8_t *outPath)
{
    FILE *decompressedFile = fopen(archivePath, "rb");
    if (decompressedFile == NULL)
    {
        printf("Error: Failed to open file at %s\n", archivePath);
        return -1;
    }

    struct ArchiveHeader *header = malloc(sizeof(struct ArchiveHeader));
    fread((uint8_t *)header, sizeof(struct ArchiveHeader), 1, decompressedFile);
    printf("fileCount = %d \n", header->fileCount);

    struct FileHeader *entries = malloc(sizeof(struct FileHeader) * header->fileCount);
    fread((uint8_t *)entries, sizeof(struct FileHeader) * header->fileCount, 1, decompressedFile);

    int64_t nameTableOffset = sizeof(struct FileHeader) * header->fileCount + sizeof(struct ArchiveHeader);

    uint8_t *filePath = malloc(512);
    size_t outPathLength = strlen(outPath);
    memcpy(filePath, outPath, outPathLength);

    for (uint32_t i = 0; i < header->fileCount; ++i)
    {
        fseek(decompressedFile, nameTableOffset + entries[i].nameTableChunkIndex * 0x10000 + entries[i].nameTableOffset, SEEK_SET);

        for (int j = 0; j < 512 - outPathLength; ++j)
        {
            filePath[outPathLength + j] = fgetc(decompressedFile);
            if (filePath[outPathLength + j] == '\0')
            {
                break;
            }
        }

        if (entries[i].crcName != CRC64_CaseInsensitive(0, filePath + outPathLength))
        {
            printf("Warning: Name does not match crc Name = %s\n", filePath + outPathLength);
        }
        fseek(decompressedFile, nameTableOffset + header->nameSize + entries[i].offset, SEEK_SET);

        uint8_t *fileData = malloc(entries[i].size);
        fread(fileData, entries[i].size, 1, decompressedFile);

        FILE *file = fopen(filePath, "wb");
        if (file == NULL)
        {
            printf("Error: Failed to open file at %s\n", filePath);
            free(filePath);
            free(header);
            free(entries);
            return -1;
        }

        fwrite(fileData, entries[i].size, 1, file);
        fclose(file);
    }
    fclose(decompressedFile);
    free(filePath);
    free(header);
    free(entries);
    return 0;
}
