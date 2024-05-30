#include <blowfish.h>
#include <lua.h>
#include <stream.h>

int decryptLua(uint8_t *encryptedFilePath, uint8_t *decryptedFilePath)
{
    uint64_t buffer;
    FILE *encryptedFile = cfopen(encryptedFilePath, "rb");

    fread(&buffer, 4, 1, encryptedFile);

    FILE *decryptedFile = cfopen(decryptedFilePath, "wb");

    if ((uint32_t)buffer == 0x6E454C1B) // \x1BLEn
    {
        fwrite("\x1BLua", 4, 1, decryptedFile);
    }
    else if ((uint32_t)buffer != 0x6F454C1B) // \x1BLEo
    {
        printf("Warning: Unexpected header in lua file %lx\n", buffer);
    }

    while (1)
    {
        size_t bytesRead = fread(&buffer, 1, sizeof(uint64_t), encryptedFile);
        if (bytesRead < sizeof(uint64_t))
        {
            fwrite(&buffer, bytesRead, 1, decryptedFile);
            break;
        }
        decryptBlock7(&buffer);
        fwrite(&buffer, sizeof(uint64_t), 1, decryptedFile);
    }

    if (fclose(encryptedFile) == EOF)
    {
        return EOF;
    }

    if (fclose(decryptedFile) == EOF)
    {
        return EOF;
    }

    return 0;
}

int encryptLua(uint8_t *decryptedFilePath, uint8_t *encryptedFilePath)
{
    uint64_t buffer;
    FILE *decryptedFile = cfopen(decryptedFilePath, "rb");
    FILE *encryptedFile = cfopen(encryptedFilePath, "wb");

    fread(&buffer, sizeof(uint32_t), 1, decryptedFile);
    if ((uint32_t)buffer == 0x61754C1B) // \x1BLua
    {
        fwrite("\x1BLEn", 4, 1, encryptedFile);
    }
    else
    {
        rewind(decryptedFile);
        fwrite("\x1BLEo", 4, 1, encryptedFile);
    }

    while (1)
    {
        size_t bytesRead = fread(&buffer, 1, sizeof(uint64_t), decryptedFile);
        if (bytesRead < sizeof(uint64_t))
        {
            fwrite(&buffer, bytesRead, 1, encryptedFile);
            break;
        }
        encryptBlock7(&buffer);
        fwrite(&buffer, sizeof(uint64_t), 1, encryptedFile);
    }

    if (fclose(decryptedFile) == EOF)
    {
        return EOF;
    }
    if (fclose(encryptedFile) == EOF)
    {
        return EOF;
    }

    return 0;
}
