#include <stdio.h>
#include <inttypes.h>

struct Blowfish;

int decryptLua(struct Blowfish *blowfish, const char *encryptedFilePath, const char *decryptedFilePath);
int encryptLua(struct Blowfish *blowfish, const char *decryptedFilePath, const char *encryptedFilePath);