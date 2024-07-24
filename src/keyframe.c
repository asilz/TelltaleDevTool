#include <tree.h>
#include <stream.h>
#include <container.h>
#include <stdlib.h>
#include <types.h>
#include <meta.h>
#include <assert.h>

int AnimationValueInterfaceBaseRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 2;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->parent = node;
    node->children[0]->description = getMetaClassDescriptionByIndex(Symbol);
    node->children[0]->description->read(stream, node->children[0], flags);

    node->children[1] = calloc(1, sizeof(struct TreeNode));
    node->children[1]->parent = node;
    node->children[1]->description = getMetaClassDescriptionByIndex(Flags);
    node->children[1]->description->read(stream, node->children[1], flags);

    return 0;
}

int AnimatedValueInterfaceGenericRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 1;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->isBlocked = 1;
    node->children[0]->parent = node;
    node->children[0]->description = getMetaClassDescriptionByIndex(AnimationValueInterfaceBase);
    node->children[0]->description->read(stream, node->children[0], flags);

    return 0;
}

static int KeyframedValueGenericSampleRead(FILE *stream, struct TreeNode *node, uint32_t flags, const struct MetaClassDescription *description)
{
    node->childCount = 4;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    node->children[0]->description = getMetaClassDescriptionByIndex(float_type);
    node->children[0]->description->read(stream, node->children[0], flags);

    node->children[1]->description = getMetaClassDescriptionByIndex(bool_type);
    node->children[1]->description->read(stream, node->children[1], flags);

    node->children[2]->description = getMetaClassDescriptionByIndex(long_type);
    node->children[2]->description->read(stream, node->children[2], flags);

    node->children[3]->description = description;
    node->children[3]->description->read(stream, node->children[3], flags);

    return 0;
}

static int KeyframedValueGenericBlockedSampleRead(FILE *stream, struct TreeNode *node, uint32_t flags, const struct MetaClassDescription *description)
{
    node->childCount = 4;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    node->children[0]->description = getMetaClassDescriptionByIndex(float_type);
    node->children[0]->description->read(stream, node->children[0], flags);

    node->children[1]->description = getMetaClassDescriptionByIndex(bool_type);
    node->children[1]->description->read(stream, node->children[1], flags);

    node->children[2]->description = getMetaClassDescriptionByIndex(long_type);
    node->children[2]->description->read(stream, node->children[2], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[3]->isBlocked = 1;
    node->children[3]->description = description;
    node->children[3]->description->read(stream, node->children[3], flags);

    return 0;
}

static int DCArray_KeyframedValue_Generic___Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags, const struct MetaClassDescription *description)
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
        node->children[i]->description;
        node->children[i]->parent = node;
        KeyframedValueGenericSampleRead(stream, node->children[i], flags, description);
    }
    return 0;
}

static int DCArray_KeyframedValue_GenericBlocked___Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags, const struct MetaClassDescription *description)
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
        node->children[i]->description;
        node->children[i]->parent = node;
        KeyframedValueGenericBlockedSampleRead(stream, node->children[i], flags, description);
    }
    return 0;
}

int DCArray_KeyframedValue_Transform___Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_GenericBlocked___Sample_Read(stream, node, flags, getMetaClassDescriptionByIndex(Transform));
}

int DCArray_KeyframedValue_float___Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_Generic___Sample_Read(stream, node, flags, getMetaClassDescriptionByIndex(float_type));
}

int DCArray_KeyframedValue_unsigned__int64___Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_Generic___Sample_Read(stream, node, flags, getMetaClassDescriptionByIndex(unsigned__int64));
}

int DCArray_KeyframedValue_AnimOrChore___Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_GenericBlocked___Sample_Read(stream, node, flags, getMetaClassDescriptionByIndex(AnimOrChore));
}

int DCArray_KeyframedValue_bool___Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_Generic___Sample_Read(stream, node, flags, getMetaClassDescriptionByIndex(bool_type));
}

int KeyframedValue_Transform_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 4;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->isBlocked = 1;
    node->children[0]->description = getMetaClassDescriptionByIndex(AnimatedValueInterface_Transform_);
    node->children[0]->description->read(stream, node->children[0], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[1]->isBlocked = 1;
    node->children[1]->description = getMetaClassDescriptionByIndex(Transform);
    node->children[1]->description->read(stream, node->children[1], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[2]->isBlocked = 1;
    node->children[2]->description = getMetaClassDescriptionByIndex(Transform);
    node->children[2]->description->read(stream, node->children[2], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[3]->isBlocked = 1;
    node->children[3]->description = getMetaClassDescriptionByIndex(DCArray_KeyframedValue_Transform___Sample_);
    node->children[3]->description->read(stream, node->children[3], flags);

    return 0;
}

int KeyframedValue_float_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 4;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->isBlocked = 1;
    node->children[0]->description = getMetaClassDescriptionByIndex(AnimatedValueInterface_float_);
    node->children[0]->description->read(stream, node->children[0], flags);

    node->children[1]->description = getMetaClassDescriptionByIndex(float_type);
    node->children[1]->description->read(stream, node->children[1], flags);

    node->children[2]->description = getMetaClassDescriptionByIndex(float_type);
    node->children[2]->description->read(stream, node->children[2], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[3]->isBlocked = 1;
    node->children[3]->description = getMetaClassDescriptionByIndex(DCArray_KeyframedValue_float___Sample_);
    node->children[3]->description->read(stream, node->children[3], flags);

    return 0;
}

int KeyframedValue_unsigned__int64_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 4;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->isBlocked = 1;
    node->children[0]->description = getMetaClassDescriptionByIndex(AnimatedValueInterface_unsigned__int64_);
    node->children[0]->description->read(stream, node->children[0], flags);

    node->children[1]->description = getMetaClassDescriptionByIndex(unsigned__int64);
    node->children[1]->description->read(stream, node->children[1], flags);

    node->children[2]->description = getMetaClassDescriptionByIndex(unsigned__int64);
    node->children[2]->description->read(stream, node->children[2], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[3]->isBlocked = 1;
    node->children[3]->description = getMetaClassDescriptionByIndex(DCArray_KeyframedValue_unsigned__int64___Sample_);
    node->children[3]->description->read(stream, node->children[3], flags);

    return 0;
}

int KeyframedValue_AnimOrChore_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 4;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->isBlocked = 1;
    node->children[0]->description = getMetaClassDescriptionByIndex(AnimatedValueInterface_AnimOrChore_);
    node->children[0]->description->read(stream, node->children[0], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[1]->isBlocked = 1;
    node->children[1]->description = getMetaClassDescriptionByIndex(AnimOrChore);
    node->children[1]->description->read(stream, node->children[1], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[2]->isBlocked = 1;
    node->children[2]->description = getMetaClassDescriptionByIndex(AnimOrChore);
    node->children[2]->description->read(stream, node->children[2], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[3]->isBlocked = 1;
    node->children[3]->description = getMetaClassDescriptionByIndex(DCArray_KeyframedValue_AnimOrChore___Sample_);
    node->children[3]->description->read(stream, node->children[3], flags);

    return 0;
}

int KeyframedValue_bool_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 4;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->isBlocked = 1;
    node->children[0]->description = getMetaClassDescriptionByIndex(AnimatedValueInterface_bool_);
    node->children[0]->description->read(stream, node->children[0], flags);

    node->children[1]->description = getMetaClassDescriptionByIndex(bool_type);
    node->children[1]->description->read(stream, node->children[1], flags);

    node->children[2]->description = getMetaClassDescriptionByIndex(bool_type);
    node->children[2]->description->read(stream, node->children[2], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[3]->isBlocked = 1;
    node->children[3]->description = getMetaClassDescriptionByIndex(DCArray_KeyframedValue_bool___Sample_);
    node->children[3]->description->read(stream, node->children[3], flags);

    return 0;
}