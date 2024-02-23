#include "blowfish.h"
#include "lua.h"
#include "crc64.h"

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

#define dataLength 4562957

int main(void)
{
    // uint8_t gameKey[keyLength] = {0x96, 0xCA, 0x99, 0x9F, 0x8D, 0xDA, 0x9A, 0x87, 0xD7, 0xCD, 0xD9, 0xBB, 0x93, 0xD1, 0xBE, 0xC0, 0xD7, 0x91, 0x71, 0xDC, 0x9E, 0xD9, 0x8D, 0xD0, 0xD1, 0x8C, 0xD8, 0xC3, 0xA0, 0xB0, 0xC6, 0x95, 0xC3, 0x9C, 0x93, 0xBB, 0xCC, 0xCC, 0xA7, 0xD3, 0xB9, 0xD9, 0xD9, 0xD0, 0x8E, 0x93, 0xBE, 0xDA, 0xAE, 0xD1, 0x8D, 0x77, 0xD5, 0xD3, 0xA3, 0x96, 0xCA, 0x99, 0x9F, 0x8D, 0xDA, 0x9A, 0x87, 0xD7, 0xCD, 0xD9, 0xBB, 0x93, 0xD1, 0xBE, 0xC0, 0xD7, 0x91, 0x71, 0xDC, 0x9E, 0xD9, 0x8D, 0xD0, 0xD1, 0x8C, 0xD8, 0xC3, 0xA0, 0xB0, 0xC6, 0x95, 0xC3, 0x9C, 0x93, 0xBB, 0xCC, 0xCC, 0xA7, 0xD3, 0xB9, 0xD9, 0xD9, 0xD0, 0x8E, 0x93, 0xBE, 0xDA, 0xAE, 0xD1, 0x8D, 0x77, 0xD5, 0xD3, 0xA3};

    initBlowfish7(gameKey, keyLen);
    decryptLua("../cipherTexts/WDC_pc_WalkingDead301_data.ttarch2", "../plainTexts/WDC_pc_WalkingDead301_data.ttarch2", dataLength);

    return 0;
}
