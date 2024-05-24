#include <inttypes.h>
#include <stdio.h>

int BoolRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int intrinsic4Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int intrinsic8Read(FILE *stream, struct TreeNode *node, uint32_t flags);