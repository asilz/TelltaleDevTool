#ifndef LANDB_H
#define LANDB_H

#include <inttypes.h>
#include <stdio.h>

int LanguageDBRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int LanguageResRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int ProjectDatabaseIDPairRead(FILE *stream, struct TreeNode *node, uint32_t flags);

int DCArray_LanguageResLocal_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int DCArray_ProjectDatabaseIDPair_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int Map_intSymbolless_int__Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int Map_unsignedintLanguageResless_unsignedint__Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int LocalizationRegistryRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int LanguageResLocalRead(FILE *stream, struct TreeNode *node, uint32_t flags);

#endif // LANDB_H