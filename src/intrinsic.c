#include <stdio.h>
#include <stdlib.h>
#include <tree.h>

int BoolRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->dataSize = sizeof(uint8_t);
    node->data.staticBuffer[0] = (uint8_t)fgetc(stream);

    if (flags & 0x00000001)
    {
        printf("bool = %c\n", node->data.staticBuffer[0]);
    }

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