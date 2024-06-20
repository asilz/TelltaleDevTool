#include <container.h>
#include <stdlib.h>
#include <intrinsic.h>
#include <types.h>
#include <meta.h>
#include <stream.h>

int genericArrayRead(FILE *stream, struct TreeNode *node, uint32_t flags, const struct MetaClassDescription *description)
{
    node->childCount = 1;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));
    node->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->description = getMetaClassDescriptionByIndex(int_type);
    node->children[0]->description->read(stream, node->children[0], flags);
    node->children[0]->parent = node;

    node->childCount += *(uint32_t *)(node->children[0]->data.staticBuffer);
    node->children = realloc(node->children, node->childCount * sizeof(struct TreeNode *));

    for (uint32_t i = 1; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->description = description;
        node->children[i]->parent = node;
        description->read(stream, node->children[i], flags);
    }
    return 0;
}

int genericMapRead(FILE *stream, struct TreeNode *node, uint32_t flags, const struct MetaClassDescription *description1, const struct MetaClassDescription *description2)
{
    node->childCount = 1;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));
    node->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->parent = node;
    node->children[0]->description = getMetaClassDescriptionByIndex(int_type);
    node->children[0]->description->read(stream, node->children[0], flags);

    node->childCount += (*(uint32_t *)(node->children[0]->data.staticBuffer)) * 2;
    node->children = realloc(node->children, node->childCount * sizeof(struct TreeNode *));

    for (uint32_t i = 1; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
        if (i % 2) // if odd
        {
            node->children[i]->description = description1;
            description1->read(stream, node->children[i], flags);
        }
        else
        {
            node->children[i]->description = description2;
            description2->read(stream, node->children[i], flags);
        }
    }
    return 0;
}

int Set_Symbolless_Symbol__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(Symbol));
}