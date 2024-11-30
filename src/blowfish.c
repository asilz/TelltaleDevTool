#include <blowfish_tab.h>
#include <blowfish.h>
#include <string.h>

static uint32_t F(struct Blowfish *blowfish, uint32_t leftHalf)
{
    uint8_t box0 = (uint8_t)(leftHalf >> 24);
    uint8_t box1 = (uint8_t)(leftHalf >> 16);
    uint8_t box2 = (uint8_t)(leftHalf >> 8);
    uint8_t box3 = (uint8_t)leftHalf;

    uint32_t a = blowfish->sbox[0][box0];
    uint32_t b = blowfish->sbox[1][box1];
    uint32_t c = blowfish->sbox[2][box2];
    uint32_t d = blowfish->sbox[3][box3];

    uint32_t out = a + b; // Modulo not neccesary due to overflow being removed due to the size limit of uint32_t
    out = out ^ c;
    out = out + d;

    return out;
}

static void encryptBlock(struct Blowfish *blowfish, uint64_t *block)
{
    uint32_t rightHalf = (uint32_t)(*block & 0xFFFFFFFF);
    uint32_t leftHalf = (uint32_t)(*block >> 32);
    uint32_t temp;

    for (size_t i = 0; i < sizeof(blowfish->pArray) / sizeof(*(blowfish->pArray)) - 2; ++i)
    {
        leftHalf = leftHalf ^ blowfish->pArray[i];
        rightHalf = F(blowfish, leftHalf) ^ rightHalf;

        temp = leftHalf;
        leftHalf = rightHalf;
        rightHalf = temp;
    }

    temp = leftHalf;
    leftHalf = rightHalf;
    rightHalf = temp;

    rightHalf = rightHalf ^ blowfish->pArray[sizeof(blowfish->pArray) / sizeof(*(blowfish->pArray)) - 2];
    leftHalf = leftHalf ^ blowfish->pArray[sizeof(blowfish->pArray) / sizeof(*(blowfish->pArray)) - 1];

    *block = ((uint64_t)leftHalf << 32) | rightHalf;
}

static void decryptBlock(struct Blowfish *blowfish, uint64_t *block)
{
    uint32_t rightHalf = (uint32_t)(*block & 0xFFFFFFFF);
    uint32_t leftHalf = (uint32_t)(*block >> 32);
    uint32_t temp;

    for (size_t i = sizeof(blowfish->pArray) / sizeof(*(blowfish->pArray)) - 1; i > 1; --i)
    {
        leftHalf = leftHalf ^ blowfish->pArray[i];
        rightHalf = F(blowfish, leftHalf) ^ rightHalf;

        temp = leftHalf;
        leftHalf = rightHalf;
        rightHalf = temp;
    }

    temp = leftHalf;
    leftHalf = rightHalf;
    rightHalf = temp;

    rightHalf = rightHalf ^ blowfish->pArray[1];
    leftHalf = leftHalf ^ blowfish->pArray[0];

    *block = ((uint64_t)leftHalf << 32) | rightHalf;
}

int blowfishInit(struct Blowfish *blowfish, const char *key, size_t keyLength)
{
    blowfish->decryptBlock = decryptBlock;
    blowfish->encryptBlock = encryptBlock;

    memcpy(blowfish->pArray, pArray, sizeof(pArray));
    memcpy(blowfish->sbox, sbox, sizeof(sbox));

    for (size_t i = 0; i < sizeof(blowfish->pArray); i += 4)
    {
        ((uint8_t *)(blowfish->pArray))[i + 3] ^= key[i % keyLength];
        ((uint8_t *)(blowfish->pArray))[i + 2] ^= key[(i + 1) % keyLength];
        ((uint8_t *)(blowfish->pArray))[i + 1] ^= key[(i + 2) % keyLength];
        ((uint8_t *)(blowfish->pArray))[i] ^= key[(i + 3) % keyLength];
    }

    uint64_t block = 0;
    for (size_t i = 0; i < sizeof(blowfish->pArray) / sizeof(*(blowfish->pArray)); i += 2)
    {
        blowfish->encryptBlock(blowfish, &block);
        uint32_t rightHalf = (uint32_t)(block);
        uint32_t leftHalf = (uint32_t)(block >> 32);
        blowfish->pArray[i] = leftHalf;
        blowfish->pArray[i + 1] = rightHalf;
    }
    for (size_t i = 0; i < sizeof(blowfish->sbox) / sizeof(blowfish->sbox[0]); ++i)
    {
        for (size_t j = 0; j < sizeof(blowfish->sbox[0]) / sizeof(blowfish->sbox[0][0]); j += 2)
        {
            blowfish->encryptBlock(blowfish, &block);
            uint32_t rightHalf = (uint32_t)(block);
            uint32_t leftHalf = (uint32_t)(block >> 32);
            blowfish->sbox[i][j] = leftHalf;
            blowfish->sbox[i][j + 1] = rightHalf;
        }
    }

    return 0;
}

static void decryptBlock7(struct Blowfish *blowfish, uint64_t *block)
{
    uint32_t leftHalf = (uint32_t)(*block & 0xFFFFFFFF);
    uint32_t rightHalf = (uint32_t)(*block >> 32);
    uint32_t temp;

    for (size_t i = sizeof(blowfish->pArray) / sizeof(*(blowfish->pArray)) - 1; i > 1; --i)
    {
        switch (i)
        { // version7
        case 4:
            temp = blowfish->pArray[2];
            break;
        case 3:
            temp = blowfish->pArray[1];
            break;
        case 2:
            temp = blowfish->pArray[4];
            break;
        default:
            temp = blowfish->pArray[i];
            break;
        }
        leftHalf = leftHalf ^ temp;
        rightHalf = F(blowfish, leftHalf) ^ rightHalf;

        temp = leftHalf;
        leftHalf = rightHalf;
        rightHalf = temp;
    }

    temp = leftHalf;
    leftHalf = rightHalf;
    rightHalf = temp;

    rightHalf = rightHalf ^ blowfish->pArray[3]; // version7
    leftHalf = leftHalf ^ blowfish->pArray[0];

    *block = ((uint64_t)rightHalf << 32) | leftHalf;
}

static void encryptBlock7(struct Blowfish *blowfish, uint64_t *block)
{
    uint32_t leftHalf = (uint32_t)(*block & 0xFFFFFFFF);
    uint32_t rightHalf = (uint32_t)(*block >> 32);
    uint32_t temp;

    for (size_t i = 0; i < sizeof(blowfish->pArray) / sizeof(*(blowfish->pArray)) - 2; ++i)
    {
        switch (i)
        { // version7
        case 1:
            temp = blowfish->pArray[3];
            break;
        case 2:
            temp = blowfish->pArray[4];
            break;
        case 3:
            temp = blowfish->pArray[1];
            break;
        case 4:
            temp = blowfish->pArray[2];
            break;
        default:
            temp = blowfish->pArray[i];
            break;
        }

        leftHalf = leftHalf ^ temp;
        rightHalf = F(blowfish, leftHalf) ^ rightHalf;

        temp = leftHalf;
        leftHalf = rightHalf;
        rightHalf = temp;
    }

    temp = leftHalf;
    leftHalf = rightHalf;
    rightHalf = temp;

    rightHalf = rightHalf ^ blowfish->pArray[sizeof(blowfish->pArray) / sizeof(*(blowfish->pArray)) - 2];
    leftHalf = leftHalf ^ blowfish->pArray[sizeof(blowfish->pArray) / sizeof(*(blowfish->pArray)) - 1];

    *block = ((uint64_t)rightHalf << 32) | leftHalf;
}

static inline uint32_t bswap(uint32_t num)
{
    return (((num & 0xff000000) >> 24) |
            ((num & 0x00ff0000) >> 8) |
            ((num & 0x0000ff00) << 8) |
            ((num & 0x000000ff) << 24));
}

int blowfish7Init(struct Blowfish *blowfish, const char *key, size_t keyLength)
{
    blowfish->decryptBlock = decryptBlock7;
    blowfish->encryptBlock = encryptBlock7;

    memcpy(blowfish->pArray, pArray, sizeof(pArray));
    memcpy(blowfish->sbox, sbox, sizeof(sbox));

    blowfish->sbox[0][118] = bswap(blowfish->sbox[0][118]);
    for (size_t i = 0; i < sizeof(blowfish->pArray); i += 4)
    {
        ((uint8_t *)(blowfish->pArray))[i + 3] ^= key[i % keyLength];
        ((uint8_t *)(blowfish->pArray))[i + 2] ^= key[(i + 1) % keyLength];
        ((uint8_t *)(blowfish->pArray))[i + 1] ^= key[(i + 2) % keyLength];
        ((uint8_t *)(blowfish->pArray))[i] ^= key[(i + 3) % keyLength];
    }

    uint64_t block = 0;
    for (size_t i = 0; i < sizeof(blowfish->pArray) / sizeof(*(blowfish->pArray)); i += 2)
    {
        encryptBlock(blowfish, &block);
        uint32_t rightHalf = (uint32_t)(block);
        uint32_t leftHalf = (uint32_t)(block >> 32);
        blowfish->pArray[i] = leftHalf;
        blowfish->pArray[i + 1] = rightHalf;
    }
    for (size_t i = 0; i < sizeof(blowfish->sbox) / sizeof(blowfish->sbox[0]); ++i)
    {
        for (size_t j = 0; j < sizeof(blowfish->sbox[0]) / sizeof(blowfish->sbox[0][0]); j += 2)
        {
            encryptBlock(blowfish, &block);
            uint32_t rightHalf = (uint32_t)(block);
            uint32_t leftHalf = (uint32_t)(block >> 32);
            blowfish->sbox[i][j] = leftHalf;
            blowfish->sbox[i][j + 1] = rightHalf;
        }
    }

    return 0;
}
