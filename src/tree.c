#include <tree.h>
#include <stdlib.h>
#include <stdio.h>
#include <stream.h>
#include <string.h>
#include <assert.h>

void treeFree(struct TreeNode *root)
{
    if (root->sibling != NULL)
    {
        treeFree(root->sibling);
        free(root->sibling);
    }
    if (root->dataSize > sizeof(root->staticBuffer))
    {
        free(root->dynamicBuffer);
    }
    else if (root->dataSize == 0 && root->child != NULL)
    {
        treeFree(root->child);
        free(root->child);
    }
}

uint32_t writeTree(FILE *stream, const struct TreeNode *root)
{
    int64_t tell = cftell(stream);
    if (tell < 0x2ba8 && tell > 0x2b90)
    {
        printf("stop\n");
    }
    if (root->dataSize > 0)
    {
        if (root->dataSize > sizeof(root->staticBuffer))
        {
            return fwrite(root->dynamicBuffer, 1, root->dataSize, stream);
        }
        else
        {
            return fwrite(root->staticBuffer, 1, root->dataSize, stream);
        }
    }

    uint32_t ret = 0;
    root = root->child;
    while (root != NULL)
    {
        if (root->serializeType)
        {
            ret += fwrite(&(root->description->crc), 1, sizeof(root->description->crc), stream);
        }
        if (root->isBlocked)
        {
            size_t childSize = fwrite(&ret, 1, sizeof(ret), stream);
            childSize += writeTree(stream, root);
            cfseek(stream, -(int64_t)childSize, SEEK_CUR);
            fwrite(&childSize, 1, sizeof(uint32_t), stream);
            cfseek(stream, childSize - sizeof(uint32_t), SEEK_CUR);
            ret += childSize;
        }
        else
        {
            ret += writeTree(stream, root);
        }
        root = root->sibling;
    }
    return ret;
}
