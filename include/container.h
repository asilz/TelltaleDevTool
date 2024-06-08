#include <tree.h>
#include <stdio.h>
#include <inttypes.h>

int genericArrayRead(FILE *stream, struct TreeNode *node, uint32_t flags, const struct MetaClassDescription *description);
int genericMapRead(FILE *stream, struct TreeNode *node, uint32_t flags, const struct MetaClassDescription *description1, const struct MetaClassDescription *description2);