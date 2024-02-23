#include "blowfish.h"
#include "lua.h"

int decryptLua(uint8_t *encryptedFilePath, uint8_t *decryptedFilePath, size_t fileSize)
{
    uint64_t *buffer = malloc(fileSize);
    FILE *ptr;
    ptr = fopen(encryptedFilePath, "rb");
    fread(buffer, 4, 1, ptr);
    fread(buffer, fileSize, 1, ptr);
    if (fclose(ptr) == EOF)
    {
        return EOF;
    }
    decryptData7(buffer, fileSize / sizeof(uint64_t));
    ptr = fopen(decryptedFilePath, "wb");
    fwrite(buffer, fileSize, 1, ptr);
    if (fclose(ptr) == EOF)
    {
        return EOF;
    }
    free(buffer);
    return 0;
}

int encryptLua(uint8_t *decryptedFilePath, uint8_t *encryptedFilePath, size_t fileSize)
{
    uint64_t *buffer = malloc(fileSize);
    FILE *ptr;
    ptr = fopen(decryptedFilePath, "rb");
    fread(buffer, fileSize, 1, ptr);
    if (fclose(ptr) == EOF)
    {
        return EOF;
    }
    encryptData7(buffer, fileSize / sizeof(uint64_t));
    ptr = fopen(encryptedFilePath, "wb");
    fwrite("\x1BLEo", 4, 1, ptr);
    fwrite(buffer, fileSize, 1, ptr);
    if (fclose(ptr) == EOF)
    {
        return EOF;
    }
    free(buffer);
    return 0;
}
