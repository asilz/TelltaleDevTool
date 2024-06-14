#include <stdio.h>
#include <stdlib.h>
#include <tree.h>

int intrinsic1Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->dataSize = sizeof(uint8_t);
    *(uint64_t *)node->data.staticBuffer = (uint8_t)fgetc(stream);

    return 0;
}

int intrinsic2Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->dataSize = sizeof(uint16_t);
    fread(node->data.staticBuffer, node->dataSize, 1, stream);

    return 0;
}

int intrinsic4Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->dataSize = sizeof(uint32_t);
    fread(node->data.staticBuffer, node->dataSize, 1, stream);

    return 0;
}

int intrinsic8Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->dataSize = sizeof(uint64_t);
    fread(node->data.staticBuffer, node->dataSize, 1, stream);

    return 0;
}
