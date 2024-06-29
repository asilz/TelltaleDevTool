#include <tree.h>
#include <stream.h>
#include <container.h>
#include <stdlib.h>
#include <types.h>
#include <meta.h>
#include <assert.h>

int AnimatedValueInterface_Transform_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 1;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->parent = node;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->isBlocked = 1;
    node->children[0]->description = getMetaClassDescriptionByIndex(AnimationValueInterfaceBase);
    node->children[0]->description->read(stream, node->children[0], flags);

    return 0;
}

static int KeyframedValue_Transform___SampleRead(FILE *stream, struct TreeNode *node, uint32_t flags)
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
    node->children[3]->description = getMetaClassDescriptionByIndex(Transform);
    node->children[3]->description->read(stream, node->children[3], flags);

    return 0;
}

int DCArray_KeyframedValue_Transform___Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
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
        KeyframedValue_Transform___SampleRead(stream, node->children[i], flags);
    }
    return 0;
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

static int AnimationValueRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->dataSize = sizeof(uint64_t) + 2 * sizeof(uint16_t);
    node->data.dynamicBuffer = malloc(node->dataSize);
    fread(node->data.dynamicBuffer, node->dataSize, 1, stream);

    return 0;
}

static int AnimationValueArrayRead(FILE *stream, struct TreeNode *node, uint32_t flags)
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
        AnimationValueRead(stream, node->children[i], flags);
    }
    return 0;
}

static int AnimationCoreRead(FILE *stream, struct TreeNode *node, uint32_t flags) // Don't know what to name this
{
    uint32_t interfaceCount;
    fread(&interfaceCount, sizeof(interfaceCount), 1, stream);
    cfseek(stream, -(int64_t)sizeof(interfaceCount), SEEK_CUR);

    node->childCount = 4 + 2 * interfaceCount;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    node->children[0]->description = getMetaClassDescriptionByIndex(long_type);
    node->children[0]->description->read(stream, node->children[0], flags);

    node->children[1]->description = getMetaClassDescriptionByIndex(long_type);
    node->children[1]->description->read(stream, node->children[1], flags);

    node->children[2]->description;
    AnimationValueArrayRead(stream, node->children[2], flags);

    uint32_t childIndex = 2;
    for (uint32_t i = 1; i < node->children[2]->childCount; ++i)
    {
        for (uint32_t j = 0; j < ((uint16_t *)node->children[2]->children[i]->data.dynamicBuffer)[4]; ++j)
        {
            uint64_t symbol = *(uint64_t *)node->children[2]->children[i]->data.dynamicBuffer;
            node->children[++childIndex]->description = getMetaClassDescriptionBySymbol(*(uint64_t *)node->children[2]->children[i]->data.dynamicBuffer);
            node->children[childIndex]->description->read(stream, node->children[childIndex], flags);
        }
    }

    while (childIndex < node->childCount - 2)
    {
        node->children[++childIndex]->description = getMetaClassDescriptionByIndex(Flags);
        node->children[childIndex]->description->read(stream, node->children[childIndex], flags);
    }

    node->children[++childIndex]->description = getMetaClassDescriptionByIndex(unsignedshort);
    node->children[childIndex]->description->read(stream, node->children[childIndex], flags);

    if (*(uint16_t *)(node->children[childIndex]->data.staticBuffer) == 0)
    {
        node->childCount += interfaceCount;
        node->children = realloc(node->children, node->childCount * sizeof(struct TreeNode *));

        while (childIndex < node->childCount - 1)
        {
            node->children[++childIndex] = calloc(1, sizeof(struct TreeNode));
            node->children[childIndex]->parent = node;

            node->children[childIndex]->description = getMetaClassDescriptionByIndex(Symbol);
            node->children[childIndex]->description->read(stream, node->children[childIndex], flags);
        }
    }

    return 0;
}

int AnimationRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 7;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    node->children[0]->description = getMetaClassDescriptionByIndex(long_type);
    node->children[0]->description->read(stream, node->children[0], flags);

    node->children[1]->description = getMetaClassDescriptionByIndex(Flags);
    node->children[1]->description->read(stream, node->children[1], flags);

    node->children[2]->description = getMetaClassDescriptionByIndex(Symbol);
    node->children[2]->description->read(stream, node->children[2], flags);

    node->children[3]->description = getMetaClassDescriptionByIndex(float_type);
    node->children[3]->description->read(stream, node->children[3], flags);

    node->children[4]->description = getMetaClassDescriptionByIndex(float_type);
    node->children[4]->description->read(stream, node->children[4], flags);

    node->children[5]->description = getMetaClassDescriptionByIndex(ToolProps);
    node->children[5]->description->read(stream, node->children[5], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[6]->isBlocked = 1;
    node->children[6]->description;
    AnimationCoreRead(stream, node->children[6], flags);

    return 0;
}