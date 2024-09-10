#include <container.h>
#include <stdlib.h>
#include <intrinsic.h>
#include <types.h>
#include <meta.h>
#include <stream.h>
#include <assert.h>

int genericArrayRead(FILE *stream, struct TreeNode *node, uint32_t flags, const struct MetaClassDescription *description)
{
    node->child = malloc(sizeof(struct TreeNode));
    node->child->description = getMetaClassDescriptionByIndex(int_type);
    node->child->description->read(stream, node->child, flags);
    node->child->parent = node;
    node->child->serializeType = 0;
    node->child->memberName = "entryCount";
    node->child->isBlocked = 0;
    node->child->sibling = NULL;

    struct TreeNode *currentNode = node->child;

    assert(*(uint32_t *)(node->child->staticBuffer) < 0x400);

    for (uint32_t i = 0; i < *(uint32_t *)(node->child->staticBuffer); ++i)
    {
        currentNode->sibling = calloc(1, sizeof(struct TreeNode));
        currentNode = currentNode->sibling;
        currentNode->description = description;
        currentNode->parent = node;
        description->read(stream, currentNode, flags);
    }
    return 0;
}

int genericMapRead(FILE *stream, struct TreeNode *node, uint32_t flags, const struct MetaClassDescription *description1, const struct MetaClassDescription *description2)
{
    node->child = malloc(sizeof(struct TreeNode));
    node->child->description = getMetaClassDescriptionByIndex(int_type);
    node->child->description->read(stream, node->child, flags);
    node->child->parent = node;
    node->child->serializeType = 0;
    node->child->memberName = "pairCount";
    node->child->isBlocked = 0;
    node->child->sibling = NULL;

    struct TreeNode *currentNode = node->child;

    for (uint32_t i = 0; i < *(uint32_t *)(node->child->staticBuffer) * 2; ++i)
    {
        currentNode->sibling = calloc(1, sizeof(struct TreeNode));
        currentNode = currentNode->sibling;
        currentNode->parent = node;
        if (i % 2) // if odd
        {
            currentNode->description = description2;
            description2->read(stream, currentNode, flags);
        }
        else
        {
            currentNode->description = description1;
            description1->read(stream, currentNode, flags);
        }
    }
    return 0;
}

int Set_Symbolless_Symbol__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(Symbol));
}