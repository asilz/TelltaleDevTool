#include <stdlib.h>
#include <inttypes.h>

struct Blowfish
{
    uint32_t pArray[18];
    uint32_t sbox[4][256];

    void (*encryptBlock)(struct Blowfish *, uint64_t *block);
    void (*decryptBlock)(struct Blowfish *, uint64_t *block);
};

#define TWD_DE_KEY "\x96\xCA\x99\x9F\x8D\xDA\x9A\x87\xD7\xCD\xD9\xBB\x93\xD1\xBE\xC0\xD7\x91\x71\xDC\x9E\xD9\x8D\xD0\xD1\x8C\xD8\xC3\xA0\xB0\xC6\x95\xC3\x9C\x93\xBB\xCC\xCC\xA7\xD3\xB9\xD9\xD9\xD0\x8E\x93\xBE\xDA\xAE\xD1\x8D\x77\xD5\xD3\xA3"

int blowfishInit(struct Blowfish *blowfish, const char *key, size_t keyLength);
int blowfish7Init(struct Blowfish *blowfish, const char *key, size_t keyLength);