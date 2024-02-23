#include <stdio.h>
#include <inttypes.h>

int decryptLua(uint8_t *encryptedFilePath, uint8_t *decryptedFilePath, size_t fileSize);
int encryptLua(uint8_t *decryptedFilePath, uint8_t *encryptedFilePath, size_t fileSize);