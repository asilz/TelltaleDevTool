#include <stream.h>
#include <stdio.h>
#include <stdlib.h>
#include <tree.h>
#include <types.h>
#include <container.h>
#include <meta.h>

int ActingOverridablePropOwnerRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    uint32_t magicLong;
    fread(&magicLong, sizeof(magicLong), 1, stream);

    if (magicLong == 0xBEEFF00D)
    {
        uint32_t serializationFlags;
        fread(&serializationFlags, sizeof(serializationFlags), 1, stream);
        cfseek(stream, -sizeof(serializationFlags), SEEK_CUR);
        if ((serializationFlags & 1) == 0)
        {
            node->childCount = 3;
        }
        else
        {
            node->childCount = 2;
        }
    }
    else
    {
        node->childCount = 1;
    }

    cfseek(stream, -sizeof(magicLong), SEEK_CUR);

    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    if (magicLong == 0xBEEFF00D)
    {
        node->children[0]->description = getMetaClassDescriptionByIndex(unsignedlong);
        node->children[0]->description->read(stream, node->children[0], flags);

        node->children[1]->description = getMetaClassDescriptionByIndex(Flags);
        node->children[1]->description->read(stream, node->children[1], flags);

        if (node->childCount == 3)
        {
            node->children[2]->description = getMetaClassDescriptionByIndex(PropertySet);
            node->children[2]->description->read(stream, node->children[2], flags);
        }

        return 0;
    }

    node->children[0]->description = getMetaClassDescriptionByIndex(PropertySet);
    node->children[0]->description->read(stream, node->children[0], flags);

    return 0;
}

static int ActingResourceRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 3;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->isBlocked = 1;
    node->children[0]->description = getMetaClassDescriptionByIndex(ActingOverridablePropOwner);
    node->children[0]->description->read(stream, node->children[0], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[1]->isBlocked = 1;
    node->children[1]->description = getMetaClassDescriptionByIndex(AnimOrChore);
    node->children[1]->description->read(stream, node->children[1], flags);

    node->children[2]->description = getMetaClassDescriptionByIndex(TRange_float_);
    node->children[2]->description->read(stream, node->children[2], flags);

    return 0;
}

static int ActingPaletteRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 13;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->isBlocked = 1;
    node->children[0]->description = getMetaClassDescriptionByIndex(ActingOverridablePropOwner);
    node->children[0]->description->read(stream, node->children[0], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[1]->isBlocked = 1;
    node->children[1]->description = getMetaClassDescriptionByIndex(UID__Owner);
    node->children[1]->description->read(stream, node->children[1], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[2]->isBlocked = 1;
    node->children[2]->description = getMetaClassDescriptionByIndex(String);
    node->children[2]->description->read(stream, node->children[2], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[3]->isBlocked = 1;
    node->children[3]->description = getMetaClassDescriptionByIndex(unsignedlong);
    node->children[3]->description->read(stream, node->children[3], flags);

    for (uint16_t i = 4; i < 10; ++i)
    {
        node->children[i]->description = getMetaClassDescriptionByIndex(TRange_float_);
        node->children[i]->description->read(stream, node->children[i], flags);
    }

    node->children[10]->description = getMetaClassDescriptionByIndex(long_type);
    node->children[10]->description->read(stream, node->children[10], flags);

    node->children[11]->description = getMetaClassDescriptionByIndex(Flags);
    node->children[11]->description->read(stream, node->children[11], flags);

    node->children[12]->description; // TODO: Set description and fix stupid problem
    const struct MetaClassDescription actingResourceDescription = {0, "ActingResource", ActingResourceRead, NULL};
    genericArrayRead(stream, node->children[12], flags, &actingResourceDescription);

    return 0;
}

static int ActingAccentPaletteRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 14;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->isBlocked = 1;
    node->children[0]->description = getMetaClassDescriptionByIndex(ActingOverridablePropOwner);
    node->children[0]->description->read(stream, node->children[0], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[1]->isBlocked = 1;
    node->children[1]->description = getMetaClassDescriptionByIndex(UID__Owner);
    node->children[1]->description->read(stream, node->children[1], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[2]->isBlocked = 1;
    node->children[2]->description = getMetaClassDescriptionByIndex(String);
    node->children[2]->description->read(stream, node->children[2], flags);

    node->children[3]->description = getMetaClassDescriptionByIndex(TRange_float_);
    node->children[3]->description->read(stream, node->children[3], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[4]->isBlocked = 1;
    node->children[4]->description = getMetaClassDescriptionByIndex(long_type); // EnumOverrun
    node->children[4]->description->read(stream, node->children[4], flags);

    node->children[5]->description = getMetaClassDescriptionByIndex(bool_type);
    node->children[5]->description->read(stream, node->children[5], flags);

    node->children[6]->description = getMetaClassDescriptionByIndex(TRange_float_);
    node->children[6]->description->read(stream, node->children[6], flags);

    node->children[7]->description = getMetaClassDescriptionByIndex(TRange_float_);
    node->children[7]->description->read(stream, node->children[7], flags);

    node->children[8]->description = getMetaClassDescriptionByIndex(float_type);
    node->children[8]->description->read(stream, node->children[8], flags);

    node->children[9]->description = getMetaClassDescriptionByIndex(long_type);
    node->children[9]->description->read(stream, node->children[9], flags);

    node->children[10]->description = getMetaClassDescriptionByIndex(long_type);
    node->children[10]->description->read(stream, node->children[10], flags);

    node->children[11]->description = getMetaClassDescriptionByIndex(Flags);
    node->children[11]->description->read(stream, node->children[11], flags);

    node->children[12]->description = getMetaClassDescriptionByIndex(long_type);
    node->children[12]->description->read(stream, node->children[12], flags);

    node->children[13]->description; // TODO: Fix
    const struct MetaClassDescription actingResourceDescription = {0, "ActingResource", ActingResourceRead, NULL};
    genericArrayRead(stream, node->children[13], flags, &actingResourceDescription);

    return 0;
}

int DCArray_Ptr_ActingAccentPalette__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const struct MetaClassDescription description = {0, "ActingAccentPalette", ActingAccentPaletteRead, NULL}; // TODO: Fix this
    return genericArrayRead(stream, node, flags, &description);
}

int DCArray_Ptr_ActingPalette__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const struct MetaClassDescription description = {0, "ActingPalette", ActingPaletteRead, NULL}; // TODO: Fix this
    return genericArrayRead(stream, node, flags, &description);
}

static int ActingPaletteGroup__ActingPaletteTransitionRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 6;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->isBlocked = 1;
    node->children[0]->description = getMetaClassDescriptionByIndex(String);
    node->children[0]->description->read(stream, node->children[0], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[1]->isBlocked = 1;
    node->children[1]->description = getMetaClassDescriptionByIndex(AnimOrChore);
    node->children[1]->description->read(stream, node->children[1], flags);

    for (uint16_t i = 2; i < node->childCount; ++i)
    {
        node->children[i]->description = getMetaClassDescriptionByIndex(float_type);
        node->children[i]->description->read(stream, node->children[i], flags);
    }

    return 0;
}

int List_ActingPaletteGroup__ActingPaletteTransition_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const struct MetaClassDescription description = {0, "ActingPaletteGroup::ActingPaletteTransition", ActingPaletteGroup__ActingPaletteTransitionRead, NULL}; // TODO: Fix this
    return genericArrayRead(stream, node, flags, &description);
}

int ActingPaletteGroupRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 14;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->isBlocked = 1;
    node->children[0]->description = getMetaClassDescriptionByIndex(UID__Owner);
    node->children[0]->description->read(stream, node->children[0], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[1]->isBlocked = 1;
    node->children[1]->description = getMetaClassDescriptionByIndex(String);
    node->children[1]->description->read(stream, node->children[1], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[2]->isBlocked = 1;
    node->children[2]->description = getMetaClassDescriptionByIndex(AnimOrChore);
    node->children[2]->description->read(stream, node->children[2], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[3]->isBlocked = 1;
    node->children[3]->description = getMetaClassDescriptionByIndex(AnimOrChore);
    node->children[3]->description->read(stream, node->children[3], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[4]->isBlocked = 1;
    node->children[4]->description = getMetaClassDescriptionByIndex(AnimOrChore);
    node->children[4]->description->read(stream, node->children[4], flags);

    node->children[5]->description = getMetaClassDescriptionByIndex(float_type);
    node->children[5]->description->read(stream, node->children[5], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[6]->isBlocked = 1;
    node->children[6]->description = getMetaClassDescriptionByIndex(AnimOrChore);
    node->children[6]->description->read(stream, node->children[6], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[7]->isBlocked = 1;
    node->children[7]->description = getMetaClassDescriptionByIndex(AnimOrChore);
    node->children[7]->description->read(stream, node->children[7], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[8]->isBlocked = 1;
    node->children[8]->description = getMetaClassDescriptionByIndex(List_ActingPaletteGroup__ActingPaletteTransition_);
    node->children[8]->description->read(stream, node->children[8], flags);

    node->children[9]->description = getMetaClassDescriptionByIndex(float_type);
    node->children[9]->description->read(stream, node->children[9], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[10]->isBlocked = 1;
    node->children[10]->description = getMetaClassDescriptionByIndex(Handle_TransitionMap_);
    node->children[10]->description->read(stream, node->children[10], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[11]->isBlocked = 1;
    node->children[11]->description = getMetaClassDescriptionByIndex(ActingPaletteGroup__EnumIdleTransition);
    node->children[11]->description->read(stream, node->children[11], flags);

    node->children[12]->description = getMetaClassDescriptionByIndex(float_type);
    node->children[12]->description->read(stream, node->children[12], flags);

    node->children[13]->description = getMetaClassDescriptionByIndex(float_type);
    node->children[13]->description->read(stream, node->children[13], flags);

    return 0;
}

int DCArray_Ptr_ActingPaletteGroup__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(ActingPaletteGroup));
}

int ActingPaletteClassRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 11;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->isBlocked = 1;
    node->children[0]->description = getMetaClassDescriptionByIndex(UID__Generator);
    node->children[0]->description->read(stream, node->children[0], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[1]->isBlocked = 1;
    node->children[1]->description = getMetaClassDescriptionByIndex(UID__Owner);
    node->children[1]->description->read(stream, node->children[1], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[2]->isBlocked = 1;
    node->children[2]->description = getMetaClassDescriptionByIndex(ActingOverridablePropOwner);
    node->children[2]->description->read(stream, node->children[2], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[3]->isBlocked = 1;
    node->children[3]->description = getMetaClassDescriptionByIndex(String);
    node->children[3]->description->read(stream, node->children[3], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[4]->isBlocked = 1;
    node->children[4]->description = getMetaClassDescriptionByIndex(DCArray_String_);
    node->children[4]->description->read(stream, node->children[4], flags);

    node->children[5]->description = getMetaClassDescriptionByIndex(long_type);
    node->children[5]->description->read(stream, node->children[5], flags);

    node->children[6]->description = getMetaClassDescriptionByIndex(Flags);
    node->children[6]->description->read(stream, node->children[6], flags);

    node->children[7]->description = getMetaClassDescriptionByIndex(bool_type);
    node->children[7]->description->read(stream, node->children[7], flags);

    node->children[8]->description = getMetaClassDescriptionByIndex(DCArray_Ptr_ActingPalette__);
    node->children[8]->description->read(stream, node->children[8], flags);

    node->children[9]->description = getMetaClassDescriptionByIndex(DCArray_Ptr_ActingAccentPalette__);
    node->children[9]->description->read(stream, node->children[9], flags);

    node->children[10]->description = getMetaClassDescriptionByIndex(DCArray_Ptr_ActingPaletteGroup__);
    node->children[10]->description->read(stream, node->children[10], flags);

    return 0;
}

int DCArray_ActingPaletteClass_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(ActingPaletteClass));
}

int StyleGuideRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 8;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->isBlocked = 1;
    node->children[0]->description = getMetaClassDescriptionByIndex(UID__Generator);
    node->children[0]->description->read(stream, node->children[0], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[1]->isBlocked = 1;
    node->children[1]->description = getMetaClassDescriptionByIndex(ActingOverridablePropOwner);
    node->children[1]->description->read(stream, node->children[1], flags);

    node->children[2]->description = getMetaClassDescriptionByIndex(long_type);
    node->children[2]->description->read(stream, node->children[2], flags);

    node->children[3]->description = getMetaClassDescriptionByIndex(bool_type);
    node->children[3]->description->read(stream, node->children[3], flags);

    node->children[4]->description = getMetaClassDescriptionByIndex(Flags);
    node->children[4]->description->read(stream, node->children[4], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[5]->isBlocked = 1;
    node->children[5]->description = getMetaClassDescriptionByIndex(DCArray_ActingPaletteClass_);
    node->children[5]->description->read(stream, node->children[5], flags);

    node->children[6]->description = getMetaClassDescriptionByIndex(long_type);
    node->children[6]->description->read(stream, node->children[6], flags);

    node->children[7]->description = getMetaClassDescriptionByIndex(DCArray_Ptr_ActingPaletteClass__);
    node->children[7]->description->read(stream, node->children[7], flags);

    return 0;
}

int ResourceGroupsRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 1;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->parent = node;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->isBlocked = 1;
    node->children[0]->description = getMetaClassDescriptionByIndex(Map_Symbolfloatless_Symbol__);
    node->children[0]->description->read(stream, node->children[0], flags);

    return 0;
}