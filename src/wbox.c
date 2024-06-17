#include <tree.h>
#include <stream.h>
#include <container.h>
#include <stdlib.h>
#include <types.h>
#include <meta.h>

static int WalkBoxes__EdgeRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->dataSize = 5 * sizeof(int32_t) + sizeof(float);
    node->data.dynamicBuffer = malloc(node->dataSize);
    fread(node->data.dynamicBuffer, node->dataSize, 1, stream); // TODO: Maybe change to children

    return 0;
}

static int SArray_int3_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 3;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
        node->children[i]->description = getMetaClassDescriptionByIndex(int_type);
        node->children[i]->description->read(stream, node->children[i], flags);
    }

    return 0;
}

static int SArray_int4_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 4;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
        node->children[i]->description = getMetaClassDescriptionByIndex(int_type);
        node->children[i]->description->read(stream, node->children[i], flags);
    }

    return 0;
}

static int SArray_float3_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 3;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
        node->children[i]->description = getMetaClassDescriptionByIndex(float_type);
        node->children[i]->description->read(stream, node->children[i], flags);
    }

    return 0;
}

static int SArray_WalkBoxes__Edge3_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 3;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
        node->children[i]->description; // TODO: set description
        WalkBoxes__EdgeRead(stream, node->children[i], flags);
    }

    return 0;
}

static int WalkBoxes__TriRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 9;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->isBlocked = 1;
    node->children[0]->description = getMetaClassDescriptionByIndex(long_type); // SoundFootsteps::EnumMaterial
    node->children[0]->description->read(stream, node->children[0], flags);

    node->children[1]->description = getMetaClassDescriptionByIndex(Flags);
    node->children[1]->description->read(stream, node->children[1], flags);

    node->children[2]->description = getMetaClassDescriptionByIndex(long_type);
    node->children[2]->description->read(stream, node->children[2], flags);

    node->children[3]->description = getMetaClassDescriptionByIndex(long_type);
    node->children[3]->description->read(stream, node->children[3], flags);

    node->children[4]->description = getMetaClassDescriptionByIndex(float_type);
    node->children[4]->description->read(stream, node->children[4], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[5]->isBlocked = 1;
    node->children[5]->description; // TODO: Set description
    SArray_int3_Read(stream, node->children[5], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[6]->isBlocked = 1;
    node->children[6]->description; // TODO: Set description
    SArray_WalkBoxes__Edge3_Read(stream, node->children[6], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[7]->isBlocked = 1;
    node->children[7]->description; // TODO: Set description
    SArray_int3_Read(stream, node->children[7], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[8]->isBlocked = 1;
    node->children[8]->description; // TODO: Set description
    SArray_float3_Read(stream, node->children[8], flags);

    return 0;
}

static int WalkBoxes__VertRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 2;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    node->children[0]->description = getMetaClassDescriptionByIndex(Flags);
    node->children[0]->description->read(stream, node->children[0], flags);

    node->children[1]->description = getMetaClassDescriptionByIndex(Vector3);
    node->children[1]->description->read(stream, node->children[1], flags);

    return 0;
}

static int WalkBoxes__QuadRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 1;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->parent = node;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->isBlocked = 1;
    node->children[0]->description; // TODO: Set description
    SArray_int4_Read(stream, node->children[0], flags);

    return 0;
}

int DCArray_WalkBoxes__Tri_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const struct MetaClassDescription description = {0, "WalkBoxes::Tri", WalkBoxes__TriRead, NULL}; // TODO: Fix this
    return genericArrayRead(stream, node, flags, &description);
}

int DCArray_WalkBoxes__Vert_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const struct MetaClassDescription description = {0, "WalkBoxes::Vert", WalkBoxes__VertRead, NULL}; // TODO: Fix this
    return genericArrayRead(stream, node, flags, &description);
}

int DCArray_WalkBoxes__Quad_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const struct MetaClassDescription description = {0, "WalkBoxes::Quad", WalkBoxes__QuadRead, NULL}; // TODO: Fix this
    return genericArrayRead(stream, node, flags, &description);
}

int DCArray_Vector3_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(Vector3));
}

int WalkBoxesRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 5;
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
    node->children[1]->description = getMetaClassDescriptionByIndex(DCArray_WalkBoxes__Tri_);
    node->children[1]->description->read(stream, node->children[1], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[2]->isBlocked = 1;
    node->children[2]->description = getMetaClassDescriptionByIndex(DCArray_WalkBoxes__Vert_);
    node->children[2]->description->read(stream, node->children[2], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[3]->isBlocked = 1;
    node->children[3]->description = getMetaClassDescriptionByIndex(DCArray_Vector3_);
    node->children[3]->description->read(stream, node->children[3], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[4]->isBlocked = 1;
    node->children[4]->description = getMetaClassDescriptionByIndex(DCArray_WalkBoxes__Quad_);
    node->children[4]->description->read(stream, node->children[4], flags);

    return 0;
}
