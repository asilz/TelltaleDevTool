#include <inttypes.h>
#include <stdio.h>

int intrinsic1Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int intrinsic2Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int intrinsic4Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int intrinsic8Read(FILE *stream, struct TreeNode *node, uint32_t flags);