#include "blowfish.h"
// g++ main.cpp -o out // -lgmpxx -lgmp

static uint32_t F(uint32_t leftHalf)
{
    uint8_t box0 = (uint8_t)(leftHalf >> 24);
    uint8_t box1 = (uint8_t)(leftHalf >> 16);
    uint8_t box2 = (uint8_t)(leftHalf >> 8);
    uint8_t box3 = (uint8_t)leftHalf;

    uint32_t a = sbox[0][box0];
    uint32_t b = sbox[1][box1];
    uint32_t c = sbox[2][box2];
    uint32_t d = sbox[3][box3];

    uint32_t out;
    out = a + b; // Modulo not neccesary due to overflow being removed due to the size limit of uint32_t
    out = out ^ c;
    out = out + d;

    return out;
}

void encryptBlock(uint64_t *block)
{
    // printf("Encrypting block\n");
    uint32_t rightHalf = (uint32_t)(*block & 0xFFFFFFFF);
    uint32_t leftHalf = (uint32_t)(*block >> 32);
    uint32_t temp;

    for (int i = 0; i < pArrayLength - 2; ++i)
    {
        leftHalf = leftHalf ^ pArray[i];
        rightHalf = F(leftHalf) ^ rightHalf;
        temp = leftHalf;
        leftHalf = rightHalf;
        rightHalf = temp;
    }

    temp = leftHalf;
    leftHalf = rightHalf;
    rightHalf = temp;

    rightHalf = rightHalf ^ pArray[pArrayLength - 2];
    leftHalf = leftHalf ^ pArray[pArrayLength - 1];

    *block = ((uint64_t)leftHalf << 32) | rightHalf;
}

void decryptBlock(uint64_t *block)
{
    uint32_t rightHalf = (uint32_t)(*block & 0xFFFFFFFF);
    uint32_t leftHalf = (uint32_t)(*block >> 32);
    uint32_t temp;

    for (int i = pArrayLength - 1; i > 1; --i)
    {
        leftHalf = leftHalf ^ pArray[i];
        rightHalf = F(leftHalf) ^ rightHalf;
        temp = leftHalf;
        leftHalf = rightHalf;
        rightHalf = temp;
    }

    temp = leftHalf;
    leftHalf = rightHalf;
    rightHalf = temp;

    rightHalf = rightHalf ^ pArray[1];
    leftHalf = leftHalf ^ pArray[0];

    *block = ((uint64_t)leftHalf << 32) | rightHalf;
}

void encryptData(uint64_t *data, size_t dataLength)
{
    for (int i = 0; i < dataLength; ++i)
    {
        encryptBlock(data + i);
    }
}

void decryptData(uint64_t *data, size_t dataLength)
{
    for (int i = 0; i < dataLength; ++i)
    {
        decryptBlock(data + i);
    }
}

void initBlowfish(uint8_t *key, size_t keyLength)
{
    int keyIndex = 0;
    for (int i = 0; i < pArrayLength; ++i)
    {
        for (int j = 0; j < sizeof(uint32_t); ++j)
        {
            if (keyIndex >= keyLength)
            {
                keyIndex = 0;
            }
            pArray[i] = (pArray[i] & (uint32_t)(0xFFFFFFFF00FFFFFF >> (8 * j))) | ((uint32_t)((uint8_t)(pArray[i] >> (24 - (8 * j))) ^ key[keyIndex++]) << 24 - (8 * j));
        }
    }
    uint64_t block = 0x0000000000000000;
    for (int i = 0; i < pArrayLength; i += 2)
    {
        encryptBlock(&block);
        uint32_t rightHalf = (uint32_t)(block);
        uint32_t leftHalf = (uint32_t)(block >> 32);
        pArray[i] = leftHalf;
        pArray[i + 1] = rightHalf;
    }
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 256; j += 2)
        {
            encryptBlock(&block);
            uint32_t rightHalf = (uint32_t)(block);
            uint32_t leftHalf = (uint32_t)(block >> 32);
            sbox[i][j] = leftHalf;
            sbox[i][j + 1] = rightHalf;
        }
    }
}

void printData(uint64_t *data, size_t length)
{
    for (int i = 0; i < length; ++i)
    {
        printf("%0lX\n", data[i]);
    }
}

void printText(uint64_t *data, size_t length)
{
    for (int i = 0; i < length; ++i)
    {
        for (int j = 0; j < sizeof(uint64_t); ++j)
            printf("%c", (uint8_t)(data[i] >> 8 * j));
    }
}

void decryptBlock7(uint64_t *block)
{

    uint32_t leftHalf = (uint32_t)(*block & 0xFFFFFFFF);
    uint32_t rightHalf = (uint32_t)(*block >> 32);
    uint32_t temp;

    for (int i = pArrayLength - 1; i > 1; --i)
    {
        switch (i)
        { // version7
        case 4:
            temp = pArray[2];
            break;
        case 3:
            temp = pArray[1];
            break;
        case 2:
            temp = pArray[4];
            break;
        default:
            temp = pArray[i];
            break;
        }
        leftHalf = leftHalf ^ temp;
        rightHalf = F(leftHalf) ^ rightHalf;

        /* Exchange Xl and Xr */
        temp = leftHalf;
        leftHalf = rightHalf;
        rightHalf = temp;
    }

    /* Exchange Xl and Xr */
    temp = leftHalf;
    leftHalf = rightHalf;
    rightHalf = temp;

    rightHalf = rightHalf ^ pArray[3]; // version7
    leftHalf = leftHalf ^ pArray[0];

    *block = ((uint64_t)rightHalf << 32) | leftHalf;
}

void encryptBlock7(uint64_t *block)
{
    uint32_t leftHalf = (uint32_t)(*block & 0xFFFFFFFF);
    uint32_t rightHalf = (uint32_t)(*block >> 32);
    uint32_t temp;

    for (int i = 0; i < pArrayLength - 2; ++i)
    {
        switch (i)
        { // version7
        case 1:
            temp = pArray[3];
            break;
        case 2:
            temp = pArray[4];
            break;
        case 3:
            temp = pArray[1];
            break;
        case 4:
            temp = pArray[2];
            break;
        default:
            temp = pArray[i];
            break;
        }

        leftHalf = leftHalf ^ temp;
        rightHalf = F(leftHalf) ^ rightHalf;

        temp = leftHalf;
        leftHalf = rightHalf;
        rightHalf = temp;
    }

    temp = leftHalf;
    leftHalf = rightHalf;
    rightHalf = temp;

    rightHalf = rightHalf ^ pArray[pArrayLength - 2];
    leftHalf = leftHalf ^ pArray[pArrayLength - 1];

    temp = leftHalf;
    leftHalf = rightHalf;
    rightHalf = temp;

    *block = ((uint64_t)leftHalf << 32) | rightHalf;
}

void encryptData7(uint64_t *data, size_t dataLength)
{
    for (int i = 0; i < dataLength; ++i)
    {
        encryptBlock7(data + i);
    }
}

void decryptData7(uint64_t *data, size_t dataLength)
{
    for (int i = 0; i < dataLength; ++i)
    {
        decryptBlock7(data + i);
    }
}

uint32_t bswap(uint32_t num)
{
    return (((num & 0xff000000) >> 24) |
            ((num & 0x00ff0000) >> 8) |
            ((num & 0x0000ff00) << 8) |
            ((num & 0x000000ff) << 24));
}

void initBlowfish7(uint8_t *key, size_t keyLength)
{
    sbox[0][118] = bswap(sbox[0][118]);
    int keyIndex = 0;
    for (int i = 0; i < pArrayLength; ++i)
    {
        for (int j = 0; j < sizeof(uint32_t); ++j)
        {
            if (keyIndex >= keyLength)
            {
                keyIndex = 0;
            }
            pArray[i] = (pArray[i] & (uint32_t)(0xFFFFFFFF00FFFFFF >> (8 * j))) | ((uint32_t)((uint8_t)(pArray[i] >> (24 - (8 * j))) ^ key[keyIndex++]) << 24 - (8 * j));
        }
    }
    uint64_t block = 0x0000000000000000;
    for (int i = 0; i < pArrayLength; i += 2)
    {
        encryptBlock(&block);
        uint32_t rightHalf = (uint32_t)(block);
        uint32_t leftHalf = (uint32_t)(block >> 32);
        pArray[i] = leftHalf;
        pArray[i + 1] = rightHalf;
    }
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 256; j += 2)
        {
            encryptBlock(&block);
            uint32_t rightHalf = (uint32_t)(block);
            uint32_t leftHalf = (uint32_t)(block >> 32);
            sbox[i][j] = leftHalf;
            sbox[i][j + 1] = rightHalf;
        }
    }
}

#define keyLength 110
#define dataLength 967

int main(void)
{
    uint8_t set_key[keyLength] = "\x96\xCA\x99\x9F\x8D\xDA\x9A\x87\xD7\xCD\xD9\xBB\x93\xD1\xBE\xC0\xD7\x91\x71\xDC\x9E\xD9\x8D\xD0\xD1\x8C\xD8\xC3\xA0\xB0\xC6\x95\xC3\x9C\x93\xBB\xCC\xCC\xA7\xD3\xB9\xD9\xD9\xD0\x8E\x93\xBE\xDA\xAE\xD1\x8D\x77\xD5\xD3\xA3\x96\xCA\x99\x9F\x8D\xDA\x9A\x87\xD7\xCD\xD9\xBB\x93\xD1\xBE\xC0\xD7\x91\x71\xDC\x9E\xD9\x8D\xD0\xD1\x8C\xD8\xC3\xA0\xB0\xC6\x95\xC3\x9C\x93\xBB\xCC\xCC\xA7\xD3\xB9\xD9\xD9\xD0\x8E\x93\xBE\xDA\xAE\xD1\x8D\x77\xD5\xD3\xA3";

    uint8_t *buf = malloc(dataLength);
    uint8_t *throw = malloc(4);
    FILE *ptr;
    ptr = fopen("../cipherTexts/_resdesc_50_WalkingDead301.lua", "rb");
    fread(throw, 4, 1, ptr);
    fread(buf, dataLength, 1, ptr);
    fclose(ptr);
    printf("%s", buf);
    uint64_t *buffer = (uint64_t *)buf;

    initBlowfish7(set_key, keyLength);
    printData(buffer, dataLength / sizeof(uint64_t));
    printf("\n------------------------------------------------------------------------------\n");
    decryptData7(buffer, dataLength / sizeof(uint64_t));
    printText(buffer, dataLength / sizeof(uint64_t));
    printf("\n------------------------------------------------------------------------------\n");

    printText(buffer, dataLength / sizeof(uint64_t));
    FILE *pFile = fopen("../plainTexts/_resdesc_50_WalkingDead301.lua", "wb");
    fwrite(buffer, dataLength, 1, pFile);
    fclose(pFile);
    free(buffer);
    free(throw);

    return 0;
}
