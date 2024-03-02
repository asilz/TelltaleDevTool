#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <zlib.h>
#include <assert.h>
#include "blowfish.h"

struct FileHeader
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
    uint32_t nameSize; // nameTable total size?
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

int archiveExtract(uint8_t *archivePath, uint8_t *outPath, size_t fileSize)
{
    int err;

    printf("Archive Extract\n");
    uint8_t *buffer = malloc(sizeof(struct CompressedHeader));

    if (buffer == NULL)
    {
        printf("alloc failed\n");
    }

    FILE *compressedFile = fopen(archivePath, "rb");
    FILE *decompressedFile = fopen("../plainTexts/WDC_pc_WalkingDead301_data.ttarch2", "wb");

    fread(buffer, sizeof(struct CompressedHeader), 1, compressedFile);
    struct CompressedHeader *header = (struct CompressedHeader *)buffer;

    printf("version = 0x%x\n", header->version);
    printf("chunk size = 0x%x\n", header->chunkDecompressedSize);
    printf("found %d compressed chunks\n", header->chunkCount);

    uint8_t *compressedChunk = malloc(header->chunkDecompressedSize); // This compressedChunk buffer will probably have more space than the space required for the chunk
    uint8_t *decompressedChunk = malloc(header->chunkDecompressedSize);

    uint64_t *chunkOffsets = malloc(sizeof(uint64_t) * header->chunkCount);
    fread(chunkOffsets, sizeof(uint64_t), header->chunkCount, compressedFile);
    for (uint32_t i = 0; i < header->chunkCount; ++i)
    {
        printf("0x%lx\n", chunkOffsets[i]);
    }

    fseek(compressedFile, chunkOffsets[0], SEEK_SET);
    for (uint32_t i = 1; i < header->chunkCount; ++i)
    {
        // printf("loop %d\n", i);
        fread(compressedChunk, chunkOffsets[i] - chunkOffsets[i - 1], 1, compressedFile);

        decryptData7((uint64_t *)compressedChunk, (chunkOffsets[i] - chunkOffsets[i - 1]) / sizeof(uint64_t));

        uint32_t size = header->chunkDecompressedSize;
        int err = ZlibDecompress(compressedChunk, chunkOffsets[i] - chunkOffsets[i - 1], decompressedChunk, &size);
        if (err)
        {
            printf("%d\n", err);
        }
        printf("%d", size);
        size_t bytesWritten = fwrite(decompressedChunk, header->chunkDecompressedSize, 1, decompressedFile);
    }

    // fwrite(buffer, fileSize, 1, ptr);
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

    /*
    struct Archive archive = *((struct Archive *)buffer);
    archive.entries = (struct FileHeader *)(buffer + sizeof(struct ArchiveHeader));
    printf("Archive entries\n");
    archive.fileNames = (uint8_t *)(buffer + sizeof(struct ArchiveHeader) + sizeof(struct FileHeader) * archive.header.fileCount);
    printf("%d\n", archive.header.fileCount);
    printf("%d\n", (archive.entries[archive.header.fileCount - 3].nameTableChunkIndex + 1));
    printf("%d\n", (archive.entries[archive.header.fileCount - 1].nameTableChunkIndex + 1));
    archive.fileData = archive.fileNames + archive.header.blockSize * (archive.entries[archive.header.fileCount - 1].nameTableChunkIndex + 1);
    printf("%d\n", (archive.entries[archive.header.fileCount - 1].nameTableChunkIndex + 1));

    for (int i = 0; i < archive.header.fileCount; ++i)
    {
        printf("Loop\n");
        struct FileHeader entry = archive.entries[i];
        printf("entry grabbed\n");

        uint8_t *name = archive.fileNames + archive.header.blockSize * entry.nameTableChunkIndex + entry.nameTableOffset;
        size_t outPathLength = strlen(outPath);

        uint8_t *path = malloc(outPathLength + strlen(name));

        printf("%s\n", path);

        memcpy(path, outPath, outPathLength);
        memcpy(path + outPathLength, name, strlen(name) + 1);
        printf("%s\n", path);

        uint8_t *fileData = archive.fileData + entry.offset;
        uint8_t *fileEnd = fileData + entry.size;

        uint32_t fileType = ((uint32_t *)fileData)[0]; // Grab the first 4 bytes. Example:  0x12 0x34 0x56 0x78 -> 0x78563412

        int blockSize = 0;
        int blockCrypt = 0;
        int blockClean = 0;

        printf("%x\n", fileType);

        switch (fileType)
        {
        case 0x4D424553:
            blockSize = 0x40;
            blockCrypt = 0x40;
            blockClean = 0x64;
            break; // SEBM
        case 0x4D42494E:
            break; // NIBM
        case 0xFB4A1764:
            blockSize = 0x80;
            blockCrypt = 0x20;
            blockClean = 0x50;
            break;
        case 0xEB794091:
            blockSize = 0x80;
            blockCrypt = 0x20;
            blockClean = 0x50;
            break;
        case 0x64AFDEFB:
            blockSize = 0x80;
            blockCrypt = 0x20;
            blockClean = 0x50;
            break;
        case 0x64AFDEAA:
            blockSize = 0x100;
            blockCrypt = 0x8;
            blockClean = 0x18;
            break;
        case 0x4D545245:
            break; // ERTM
        default:
            break; // is not a meta stream file
        }

        printf("init j loop\n");
        for (int j = 0; j < ((entry.size - 4) / blockSize); ++j)
        {
            int offset = (blockSize * j) + sizeof(uint32_t);
            if (fileData + offset >= fileEnd)
            {
                break;
            }
            if (!(j % blockCrypt))
            {
                decryptData7((uint64_t *)(fileData + offset), blockSize / sizeof(uint64_t));
            }
            else if (!(j % blockClean) && (j > 0))
            {
                // skip this block
            }
            else
            {
                for (int k = 0; k < blockSize; ++k)
                {
                    fileData[k + offset] ^= 0xff;
                }
            }
        }
        ptr = fopen(path, "wb");

        fwrite(fileData, entry.size, 1, ptr);
        if (fclose(ptr) == EOF)
        {
            return EOF;
        }
        free(path);
    }
    return 0;
    */
}