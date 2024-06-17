#include <stdio.h>
#include <inttypes.h>

int PhonemeTable__PhonemeEntryRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int Map_SymbolPhonemeTable__PhonemeEntryless_Symbol__Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int PhonemeTableRead(FILE *stream, struct TreeNode *node, uint32_t flags);