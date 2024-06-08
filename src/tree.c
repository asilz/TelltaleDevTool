#include <tree.h>
#include <stdlib.h>
#include <stdio.h>
#include <stream.h>
#include <string.h>

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
    int64_t ftell = cftell(stream);
    uint32_t ret = 0;
    if (root->serializeType)
    {
        ret += fwrite(&root->description->crc, 1, sizeof(root->description->crc), stream);
    }
    for (uint16_t i = 0; i < root->childCount; ++i)
    {
        if (root->children[i]->isBlocked)
        {
            size_t childSize = 0;
            childSize = fwrite(&childSize, 1, sizeof(uint32_t), stream);
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

void treePushBack(struct TreeNode *tree, struct TreeNode *child)
{
    tree->children = realloc(tree->children, (++tree->childCount) * sizeof(struct TreeNode *));
    tree->children[tree->childCount - 1] = child;
    child->parent = tree; // TODO: Maybe remove this and let the user do this manually since this requires that memory is allocated for the child which I might not always be able to garantuee.
}

struct TreeNode *copyTree(struct TreeNode *tree)
{
    struct TreeNode *copy = malloc(sizeof(struct TreeNode));

    copy->dataSize = tree->dataSize;
    copy->childCount = tree->childCount;
    copy->isBlocked = tree->isBlocked;
    copy->description = tree->description;
    copy->serializeType = tree->serializeType;

    if (copy->dataSize > sizeof(tree->data))
    {
        copy->data.dynamicBuffer = malloc(copy->dataSize);
        memcpy(copy->data.dynamicBuffer, tree->data.dynamicBuffer, copy->dataSize);
    }
    else
    {
        memcpy(copy->data.staticBuffer, tree->data.staticBuffer, sizeof(tree->data));
    }

    copy->children = malloc(copy->childCount * sizeof(struct TreeNode *));
    for (uint16_t i = 0; i < tree->childCount; ++i)
    {
        copy->children[i] = copyTree(tree->children[i]);
    }

    return copy;
}
