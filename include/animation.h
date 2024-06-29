#include <stdio.h>
#include <inttypes.h>

int AnimationRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int KeyframedValue_Transform_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int DCArray_KeyframedValue_Transform___Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int AnimatedValueInterface_Transform_Read(FILE *stream, struct TreeNode *node, uint32_t flags);