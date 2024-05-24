#include <tree.h>
#include <stdio.h>
#include <inttypes.h>

typedef int (*serializeFunction)(FILE *stream, struct TreeNode *node, uint32_t flags);

int genericArrayRead(FILE *stream, struct TreeNode *node, uint32_t flags, serializeFunction readFunction, uint64_t typeSymbol);
int genericMapRead(FILE *stream, struct TreeNode *node, uint32_t flags, serializeFunction readFunction1, serializeFunction readFunction2, uint64_t typeSymbol1, uint64_t typeSymbol2);
