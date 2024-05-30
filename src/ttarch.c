#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <zlib.h>
#include <assert.h>
#include <blowfish.h>
#include <crc64.h>
#include <stream.h>

struct FileHeader
{
    uint64_t crcName; // CRC64 EMCA 182 format
    uint64_t offset;  // How many bytes after the name table the file is located
    uint32_t size;
    uint32_t unkownBytes; // TODO: Figure what these are or just ask Lucas
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

int streamDecrypt(FILE **compressedStreamPtr)
{
    int err;

    uint64_t initialPosition = cftell(*compressedStreamPtr);
    rewind(*compressedStreamPtr);

    FILE *outputStream = tmpfile();

    for (uint64_t i = 0; i < initialPosition; ++i)
    {
        fputc(fgetc(*compressedStreamPtr), outputStream);
    }

    struct CompressedHeader header;
    fread((uint8_t *)(&header), sizeof(struct CompressedHeader), 1, *compressedStreamPtr);

    printf("version = 0x%" PRIx32 "\n", header.version);
    printf("chunk size = 0x%" PRIx32 "\n", header.chunkDecompressedSize);
    printf("found %" PRIu32 " compressed chunks\n", header.chunkCount);

    uint8_t *compressedChunk = malloc(header.chunkDecompressedSize); // This compressedChunk buffer will probably have more space than the space required for the chunk
    uint8_t *decompressedChunk = malloc(header.chunkDecompressedSize);

    uint64_t *chunkOffsets = malloc(sizeof(uint64_t) * header.chunkCount);
    fread(chunkOffsets, sizeof(uint64_t), header.chunkCount, *compressedStreamPtr);
    cfseek(*compressedStreamPtr, chunkOffsets[0] + initialPosition, SEEK_SET);
    for (uint32_t i = 1; i < header.chunkCount; ++i)
    {
        fread(compressedChunk, chunkOffsets[i] - chunkOffsets[i - 1], 1, *compressedStreamPtr);

        if ((uint8_t)header.version == 'E')
        {
            decryptData7((uint64_t *)compressedChunk, (chunkOffsets[i] - chunkOffsets[i - 1]) / sizeof(uint64_t));
        }

        uint32_t size = header.chunkDecompressedSize;
        ZlibDecompress(compressedChunk, chunkOffsets[i] - chunkOffsets[i - 1], decompressedChunk, &size);
        // printf("%d", size);
        fwrite(decompressedChunk, header.chunkDecompressedSize, 1, outputStream);
    }

    uint64_t compressedEnd = cftell(outputStream);

    for (int i = fgetc(*compressedStreamPtr); i != EOF; i = fgetc(*compressedStreamPtr))
    {
        fputc(i, outputStream);
    }

    cfseek(outputStream, compressedEnd, SEEK_SET);

    err = fclose(*compressedStreamPtr);
    if (err)
    {
        printf("fclose return %d\n", err);
        exit(err);
    }
    *compressedStreamPtr = outputStream;

    free(decompressedChunk);
    free(compressedChunk);
    free(chunkOffsets);
    return 0;
}

int archiveSplit(FILE *stream, uint8_t *folderPath)
{

    struct ArchiveHeader header;
    fread((uint8_t *)(&header), sizeof(struct ArchiveHeader), 1, stream);
    printf("fileCount = %" PRIu32 " \n", header.fileCount);

    uint64_t nameTableOffset = 28 * header.fileCount + sizeof(struct ArchiveHeader); // 28 is the size of all the FileHeader members. Using sizeof(struct FileHeader) will return the size including the 4 byte trailing padding.

    uint8_t filePath[512];
    size_t outPathLength = strlen(folderPath);
    memcpy(filePath, folderPath, outPathLength);

    for (uint32_t i = 0; i < header.fileCount; ++i)
    {
        struct FileHeader entry;
        cfseek(stream, sizeof(struct ArchiveHeader) + 28 * i, SEEK_SET);
        fread((uint8_t *)(&entry), 28, 1, stream);

        cfseek(stream, nameTableOffset + entry.nameTableChunkIndex * 0x10000 + entry.nameTableOffset, SEEK_SET);

        for (int j = 0; j < 512 - outPathLength; ++j)
        {
            filePath[outPathLength + j] = fgetc(stream);
            if (filePath[outPathLength + j] == '\0')
            {
                break;
            }
        }

        if (entry.crcName != CRC64_CaseInsensitive(0, filePath + outPathLength))
        {
            printf("Warning: Name does not match crc Name = %s\n", filePath + outPathLength);
        }
        cfseek(stream, nameTableOffset + header.nameSize + entry.offset, SEEK_SET);

        uint8_t *fileData = malloc(entry.size);
        fread(fileData, entry.size, 1, stream);

        FILE *file = cfopen(filePath, "wb");
        if (file == NULL)
        {
            printf("Error: Failed to open file at %s\n", filePath);
            free(fileData);
            return -1;
        }

        fwrite(fileData, entry.size, 1, file);
        fclose(file);
        free(fileData);
    }
    return 0;
}

int streamToFile(FILE *stream, uint8_t *outputPath)
{
    int err;

    FILE *outputStream = cfopen(outputPath, "wb");
    if (outputStream == NULL)
    {
        printf("fopen error\n");
        return EOF;
    }

    for (int i = fgetc(stream); i != EOF; i = fgetc(stream))
    {
        fputc(i, outputStream);
    }
}
