#include <tree.h>
#include <stdlib.h>
#include <stdio.h>
#include <stream.h>

void treeFree(struct TreeNode *root)
{
    if (root->dataSize > sizeof(root->data))
    {
        free(root->data.dynamicBuffer);
        root->data.dynamicBuffer = NULL;
        root->dataSize = 0;
    }

    if (root->children != NULL)
    {
        for (uint32_t i = 0; i < root->childCount; ++i)
        {
            if (root->children[i] != NULL)
            {
                treeFree(root->children[i]);
                free(root->children[i]);
                root->children[i] = NULL;
            }
        }
        free(root->children);
        root->children = NULL;
        root->childCount = 0;
    }
}

uint32_t writeTree(FILE *stream, struct TreeNode *root)
{
    // printf("ftell = %lx\n", cftell(stream));
    uint32_t ret = 0;
    if (root->serializeType)
    {
        ret += fwrite(&root->typeSymbol, 1, sizeof(root->typeSymbol), stream);
    }
    for (uint16_t i = 0; i < root->childCount; ++i)
    {
        if (root->children[i]->isBlocked)
        {
            size_t childSize = fwrite(&childSize, 1, sizeof(uint32_t), stream);
            childSize += writeTree(stream, root->children[i]);
            cfseek(stream, -(int32_t)childSize, SEEK_CUR);
            fwrite(&childSize, 1, sizeof(uint32_t), stream);
            cfseek(stream, childSize - sizeof(uint32_t), SEEK_CUR);
            ret += childSize;
        }
        else
        {
            ret += writeTree(stream, root->children[i]);
        }
    }
    if (root->dataSize <= 8)
    {
        ret += fwrite(root->data.staticBuffer, 1, root->dataSize, stream);
    }
    else
    {
        ret += fwrite(root->data.dynamicBuffer, 1, root->dataSize, stream);
    }
    return ret;
}
