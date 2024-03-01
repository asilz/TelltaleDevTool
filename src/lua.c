#include "blowfish.h"
#include "lua.h"

int decryptLua(uint8_t *encryptedFilePath, uint8_t *decryptedFilePath)
{
    uint64_t *buffer = malloc(sizeof(uint64_t));
    FILE *encryptedFile = fopen(encryptedFilePath, "rb");

    fread(buffer, 4, 1, encryptedFile);

    if (*buffer != 0x000000006F454C1B)
    {
        printf("Warning: Unexpected header in lua file %lx\n", *buffer);
    }

    FILE *decryptedFile = fopen(decryptedFilePath, "wb");

    while (1)
    {
        size_t bytesRead = fread(buffer, 1, sizeof(uint64_t), encryptedFile);
        if (bytesRead < sizeof(uint64_t))
        {
            fwrite(buffer, bytesRead, 1, decryptedFile);
            break;
        }
        decryptBlock7(buffer);
        fwrite(buffer, sizeof(uint64_t), 1, decryptedFile);
    }

    if (fclose(encryptedFile) == EOF)
    {
        return EOF;
    }

    if (fclose(decryptedFile) == EOF)
    {
        return EOF;
    }
    free(buffer);
    return 0;
}

int encryptLua(uint8_t *decryptedFilePath, uint8_t *encryptedFilePath)
{
    uint64_t *buffer = malloc(sizeof(uint64_t));
    FILE *decryptedFile = fopen(decryptedFilePath, "rb");
    FILE *encryptedFile = fopen(encryptedFilePath, "wb");

    fwrite("\x1BLEo", 4, 1, encryptedFile);

    while (1)
    {
        size_t bytesRead = fread(buffer, 1, sizeof(uint64_t), decryptedFile);
        if (bytesRead < sizeof(uint64_t))
        {
            fwrite(buffer, bytesRead, 1, encryptedFile);
            break;
        }
        encryptBlock7(buffer);
        fwrite(buffer, sizeof(uint64_t), 1, encryptedFile);
    }

    if (fclose(decryptedFile) == EOF)
    {
        return EOF;
    }
    if (fclose(encryptedFile) == EOF)
    {
        return EOF;
    }
    free(buffer);
    return 0;
}
