#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <zlib.h>
#include <assert.h>
#include "blowfish.h"

int64_t _z_decompress(uint8_t *in, int64_t insz, uint8_t *out, int64_t outsz);
int64_t (*do_decompress)(uint8_t *in, int64_t insz, uint8_t *out, int64_t outsz) = _z_decompress;

struct FileHeader
{
    uint64_t crcName; // CRC64 EMCA 182 format
    uint64_t offset;  // How many bytes after the name table the files is located
    uint32_t size;
    uint32_t unkownBytes;
    uint16_t nameTableChunkIndex;
    uint16_t nameTableOffset;
};

struct ArchiveHeader
{
    uint32_t type;
    uint32_t version;
    uint32_t blockSize; // AKA NameTable chunkSize
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

static uint64_t g_dbg_offset = 0;

int64_t _z_decompress(uint8_t *in, int64_t insz, uint8_t *out, int64_t outsz)
{
    static z_stream *z_zlib = NULL;
    static z_stream *z_deflate = NULL;
    z_stream *z;

#define UNZIP_INIT(X, Y)                                      \
    if (!z_##X)                                               \
    {                                                         \
        z_##X = calloc(1, sizeof(z_stream));                  \
        if (!z_##X)                                           \
            return -33;                                       \
        z_##X->zalloc = (alloc_func)0;                        \
        z_##X->zfree = (free_func)0;                          \
        z_##X->opaque = (voidpf)0;                            \
        if (inflateInit2(z_##X, Y))                           \
        {                                                     \
            printf("\nError: " #X " initialization error\n"); \
            exit(1);                                          \
        }                                                     \
    }
#define UNZIP_END(X)       \
    if (z_##X)             \
    {                      \
        inflateEnd(z_##X); \
        free(z_##X);       \
        z_##X = NULL;      \
    }

    if (!insz || !outsz)
        return (0);
    if (!in && !out)
    {
        UNZIP_END(zlib)
        UNZIP_END(deflate)
        return (-1);
    }

    UNZIP_INIT(zlib, 15)
    UNZIP_INIT(deflate, -15)
    z = z_zlib;
redo:
    inflateReset(z);

    z->next_in = in;
    z->avail_in = insz;
    z->next_out = out;
    z->avail_out = outsz;
    if (inflate(z, Z_FINISH) != Z_STREAM_END)
    {
        if (z == z_zlib)
        {
            z = z_deflate;
            goto redo;
        }
        printf("\nError: the compressed zlib/deflate input at offset 0x%08x (%d -> %d) is wrong or incomplete\n", (int)g_dbg_offset, (int)insz, (int)outsz);
        exit(1);
    }
    return (z->total_out);
}

int64_t _z_decompress2(uint8_t *in, int64_t insz, uint8_t *out, int64_t outsz)
{
    int ret;
    z_stream strm;

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);

    if (ret != Z_OK)
    {
        printf("Zlib error: inflateInit() failed");
        return -1;
    }

    strm.avail_in = insz;
    strm.next_in = (uint8_t *)in;
    strm.avail_out = outsz;
    strm.next_out = (uint8_t *)out;

    ret = inflate(&strm, Z_NO_FLUSH);

    switch (ret)
    {
    case Z_NEED_DICT:
        ret = Z_DATA_ERROR;
    case Z_DATA_ERROR:
    case Z_MEM_ERROR:
        printf("Zlib error: inflate()");
        return ret;
    }
    inflateEnd(&strm);
}

int archiveExtract(uint8_t *archivePath, uint8_t *outPath, size_t fileSize)
{
    printf("Archive Extract\n");
    uint8_t *buffer = calloc(fileSize, 1);
    if (buffer == NULL)
    {
        printf("alloc failed\n");
    }
    FILE *ptr = fopen(archivePath, "rb");
    fread(buffer, fileSize, 1, ptr);
    if (fclose(ptr) == EOF)
    {
        return EOF;
    }

    for (int i = 0; i < 128; ++i)
    {
        printf("%x ", (buffer + sizeof(uint32_t) * 3)[i]);
    }
    printf("\n");

    // printf("%s\n", buffer);

    struct CompressedHeader *compressedHeader = (struct CompressedHeader *)buffer;
    buffer += sizeof(struct CompressedHeader);
    printf("version = 0x%x\n", compressedHeader->version);
    printf("chunk size = 0x%x\n", compressedHeader->chunkDecompressedSize);
    printf("found %d compressed chunks\n", compressedHeader->chunkCount);
    for (int i = 0; i < 128; ++i)
    {
        printf("%x ", buffer[i]);
    }
    printf("\n");
    for (int i = 0; i < compressedHeader->chunkCount; ++i)
    {
        printf("%lx\n", ((uint64_t *)(buffer))[i]);
    }

    ptr = fopen("../plainTexts/WDC_pc_WalkingDead301_data.ttarch2", "wb");
    uint8_t *decompressedChunk = malloc(compressedHeader->chunkDecompressedSize);

    for (int i = 1; i < compressedHeader->chunkCount; ++i)
    {
        printf("loop %d\n", i);
        g_dbg_offset = ((uint64_t *)(buffer))[i];
        uint64_t *chunk = (uint64_t *)(buffer + sizeof(uint64_t) * compressedHeader->chunkCount + *((uint64_t *)(buffer + sizeof(uint64_t) * (i - 1))));
        // printf("%lx\n", *((uint64_t *)(buffer + sizeof(uint64_t) * (i - 1))) + sizeof(uint64_t) * compressedHeader->chunkCount);
        decryptData7(chunk, (*((uint64_t *)(buffer + sizeof(uint64_t) * i)) - *((uint64_t *)(buffer + sizeof(uint64_t) * (i - 1)))) / 8);
        int err = _z_decompress((uint8_t *)chunk, (*((uint64_t *)(buffer + sizeof(uint64_t) * i)) - *((uint64_t *)(buffer + sizeof(uint64_t) * (i - 1)))), decompressedChunk, compressedHeader->chunkDecompressedSize);
        fwrite(decompressedChunk, compressedHeader->chunkDecompressedSize, 1, ptr);
        if (err)
        {
            printf("%d\n", err);
        }
    }

    // fwrite(buffer, fileSize, 1, ptr);
    if (fclose(ptr) == EOF)
    {
        return EOF;
    }
    free(buffer - sizeof(struct CompressedHeader));
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