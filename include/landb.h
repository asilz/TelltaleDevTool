#ifndef LANDB_H
#define LANDB_H

#include <inttypes.h>
#include <stdio.h>

int LanguageDBRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int LanguageResRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int ProjectDatabaseIDPairRead(FILE *stream, struct TreeNode *node, uint32_t flags);

#endif // LANDB_H