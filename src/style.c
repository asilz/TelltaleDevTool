#include <stream.h>
#include <stdio.h>
#include <stdlib.h>
#include <tree.h>
#include <types.h>
#include <container.h>
#include <meta.h>

int ActingOverridablePropOwnerRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->child = calloc(1, sizeof(struct TreeNode));
    struct TreeNode *currentNode = node->child;
    currentNode->parent = node;
    currentNode->description = getMetaClassDescriptionByIndex(unsignedlong);
    currentNode->description->read(stream, currentNode, flags);

    if (*(uint32_t *)(currentNode->staticBuffer) == 0xBEEFF00D)
    {
        currentNode->sibling = calloc(1, sizeof(struct TreeNode));
        currentNode->sibling->parent = currentNode->parent;
        currentNode = currentNode->sibling;
        currentNode->description = getMetaClassDescriptionByIndex(Flags);
        currentNode->description->read(stream, currentNode, flags);
        if ((*(uint32_t *)(currentNode->staticBuffer) & 1) == 0)
        {
            currentNode->sibling = calloc(1, sizeof(struct TreeNode));
            currentNode->sibling->parent = currentNode->parent;
            currentNode = currentNode->sibling;
            currentNode->description = getMetaClassDescriptionByIndex(PropertySet);
            currentNode->description->read(stream, currentNode, flags);
        }
    }
    else
    {
        currentNode->dataSize = 0;
        currentNode->description = getMetaClassDescriptionByIndex(PropertySet);
        currentNode->description->read(stream, currentNode, flags);
    }
    return 0;
}

static int ActingResourceRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_ActingOverridablePropOwner", .metaClassDescriptionIndex = ActingOverridablePropOwner},
        {.isBlocked = 1, .memberName = "mResource", .metaClassDescriptionIndex = AnimOrChore},
        {.isBlocked = 0, .memberName = "mValidIntensityRange", .metaClassDescriptionIndex = TRange_float_},
    };
    return genericRead(stream, node, flags, 3, descriptions);
}

static int ActingPaletteRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return 0;
}

static int ActingAccentPaletteRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{

    return 0;
}

int DCArray_Ptr_ActingAccentPalette__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return 0;
}

int DCArray_Ptr_ActingPalette__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return 0;
}

static int ActingPaletteGroup__ActingPaletteTransitionRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{

    return 0;
}

int List_ActingPaletteGroup__ActingPaletteTransition_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return 0;
}

int ActingPaletteGroupRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{

    return 0;
}

int DCArray_Ptr_ActingPaletteGroup__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(ActingPaletteGroup));
}

int ActingPaletteClassRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{

    return 0;
}

int DCArray_ActingPaletteClass_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(ActingPaletteClass));
}

int StyleGuideRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{

    return 0;
}

int ResourceGroupsRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{

    return 0;
}