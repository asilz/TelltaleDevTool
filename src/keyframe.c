#include <tree.h>
#include <stream.h>
#include <container.h>
#include <stdlib.h>
#include <types.h>
#include <meta.h>
#include <assert.h>

int AnimationValueInterfaceBaseRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 0, .memberName = "mName", .metaClassDescriptionIndex = Symbol},
        {.isBlocked = 0, .memberName = "mFlags", .metaClassDescriptionIndex = Flags},
    };
    return genericRead(stream, node, flags, 2, descriptions);
}

int AnimatedValueInterfaceGenericRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "interfaceBase", .metaClassDescriptionIndex = AnimationValueInterfaceBase},
    };
    return genericRead(stream, node, flags, 1, descriptions);
}

static int KeyframedValueGenericSampleRead(FILE *stream, struct TreeNode *node, uint32_t flags, uint8_t isBlocked, uint16_t metaClassDescriptionIndex)
{
    struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 0, .memberName = "time", .metaClassDescriptionIndex = float_type},
        {.isBlocked = 0, .memberName = "interpolateToNextKey", .metaClassDescriptionIndex = bool_type},
        {.isBlocked = 0, .memberName = "tangentMode", .metaClassDescriptionIndex = long_type},
        {.isBlocked = isBlocked, .memberName = "value", .metaClassDescriptionIndex = metaClassDescriptionIndex},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

static int DCArray_KeyframedValue_Generic___Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags, uint8_t isBlocked, uint16_t metaClassDescriptionIndex)
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

    for (uint32_t i = 0; i < *(uint32_t *)(node->child->staticBuffer); ++i)
    {
        currentNode->sibling = calloc(1, sizeof(struct TreeNode));
        currentNode = currentNode->sibling;
        currentNode->parent = node;
        KeyframedValueGenericSampleRead(stream, currentNode, flags, isBlocked, metaClassDescriptionIndex);
    }
    return 0;
}

int DCArray_KeyframedValue_Transform___Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_Generic___Sample_Read(stream, node, flags, 1, Transform);
}

int DCArray_KeyframedValue_float___Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_Generic___Sample_Read(stream, node, flags, 0, float_type);
}

int DCArray_KeyframedValue_unsigned__int64___Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_Generic___Sample_Read(stream, node, flags, 0, unsigned__int64);
}

int DCArray_KeyframedValue_AnimOrChore___Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_Generic___Sample_Read(stream, node, flags, 1, AnimOrChore);
}

int DCArray_KeyframedValue_bool___Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_Generic___Sample_Read(stream, node, flags, 0, bool_type);
}

int KeyframedValue_Transform_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_Transform_},
        {.isBlocked = 1, .memberName = "minValue", .metaClassDescriptionIndex = Transform},
        {.isBlocked = 1, .memberName = "maxValue", .metaClassDescriptionIndex = Transform},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_Transform___Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int KeyframedValue_float_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_float_},
        {.isBlocked = 0, .memberName = "minValue", .metaClassDescriptionIndex = float_type},
        {.isBlocked = 0, .memberName = "maxValue", .metaClassDescriptionIndex = float_type},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_float___Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int KeyframedValue_unsigned__int64_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_unsigned__int64_},
        {.isBlocked = 0, .memberName = "minValue", .metaClassDescriptionIndex = unsigned__int64},
        {.isBlocked = 0, .memberName = "maxValue", .metaClassDescriptionIndex = unsigned__int64},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_unsigned__int64___Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int KeyframedValue_AnimOrChore_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_AnimOrChore_},
        {.isBlocked = 1, .memberName = "minValue", .metaClassDescriptionIndex = AnimOrChore},
        {.isBlocked = 1, .memberName = "maxValue", .metaClassDescriptionIndex = AnimOrChore},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_AnimOrChore___Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int KeyframedValue_bool_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_bool_},
        {.isBlocked = 0, .memberName = "minValue", .metaClassDescriptionIndex = bool_type},
        {.isBlocked = 0, .memberName = "maxValue", .metaClassDescriptionIndex = bool_type},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_bool___Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}