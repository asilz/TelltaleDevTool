#include <tree.h>
#include <stream.h>
#include <container.h>
#include <stdlib.h>
#include <types.h>
#include <meta.h>

int BoundingBoxRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 2;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    node->children[0]->description = getMetaClassDescriptionByIndex(Vector3);
    node->children[0]->description->read(stream, node->children[0], flags);

    node->children[1]->description = getMetaClassDescriptionByIndex(Vector3);
    node->children[1]->description->read(stream, node->children[1], flags);

    return 0;
}

int SphereRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 2;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    node->children[0]->description = getMetaClassDescriptionByIndex(Vector3);
    node->children[0]->description->read(stream, node->children[0], flags);

    node->children[1]->description = getMetaClassDescriptionByIndex(float_type);
    node->children[1]->description->read(stream, node->children[1], flags);

    return 0;
}

int BitSetBase_1_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 1;
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

int BitSetBase_2_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 2;
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

int BitSetBase_3_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
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

int T3MeshTextureIndicesRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    uint32_t buffer;
    do
    {
        fread(&buffer, sizeof(buffer), 1, stream);
        node->dataSize += sizeof(uint32_t);
    } while (buffer != 0xFFFFFFFF && node->dataSize < 8 * sizeof(uint32_t));
    cfseek(stream, -(int64_t)(node->dataSize), SEEK_CUR);

    if (node->dataSize <= sizeof(node->data))
    {
        fread(node->data.staticBuffer, node->dataSize, 1, stream);
    }
    else
    {
        node->data.dynamicBuffer = malloc(node->dataSize);
        fread(node->data.dynamicBuffer, node->dataSize, 1, stream);
    }

    return 0;
}

int T3MeshBatchRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 12;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    node->children[0]->description = getMetaClassDescriptionByIndex(BoundingBox);
    node->children[0]->description->read(stream, node->children[0], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[1]->isBlocked = 1;
    node->children[1]->description = getMetaClassDescriptionByIndex(Sphere);
    node->children[1]->description->read(stream, node->children[1], flags);

    node->children[2]->description = getMetaClassDescriptionByIndex(Flags);
    node->children[2]->description->read(stream, node->children[2], flags);

    node->children[3]->description = getMetaClassDescriptionByIndex(unsignedlong);
    node->children[3]->description->read(stream, node->children[3], flags);

    node->children[4]->description = getMetaClassDescriptionByIndex(unsignedlong);
    node->children[4]->description->read(stream, node->children[4], flags);

    node->children[5]->description = getMetaClassDescriptionByIndex(unsignedlong);
    node->children[5]->description->read(stream, node->children[5], flags);

    node->children[6]->description = getMetaClassDescriptionByIndex(unsignedlong);
    node->children[6]->description->read(stream, node->children[6], flags);

    node->children[7]->description = getMetaClassDescriptionByIndex(unsignedlong);
    node->children[7]->description->read(stream, node->children[7], flags);

    node->children[8]->description = getMetaClassDescriptionByIndex(unsignedlong);
    node->children[8]->description->read(stream, node->children[8], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[9]->isBlocked = 1;
    node->children[9]->description = getMetaClassDescriptionByIndex(T3MeshTextureIndices);
    node->children[9]->description->read(stream, node->children[9], flags);

    node->children[10]->description = getMetaClassDescriptionByIndex(long_type);
    node->children[10]->description->read(stream, node->children[10], flags);

    node->children[11]->description = getMetaClassDescriptionByIndex(unsignedlong);
    node->children[11]->description->read(stream, node->children[11], flags);

    return 0;
}

int DCArray_T3MeshBatch_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(T3MeshBatch));
}

int T3MeshLODRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 16;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->isBlocked = 1;
    node->children[0]->description = getMetaClassDescriptionByIndex(DCArray_T3MeshBatch_);
    node->children[0]->description->read(stream, node->children[0], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[1]->isBlocked = 1;
    node->children[1]->description = getMetaClassDescriptionByIndex(DCArray_T3MeshBatch_);
    node->children[1]->description->read(stream, node->children[1], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[2]->isBlocked = 1;
    node->children[2]->description = getMetaClassDescriptionByIndex(BitSetBase_1_);
    node->children[2]->description->read(stream, node->children[2], flags);

    node->children[3]->description = getMetaClassDescriptionByIndex(BoundingBox);
    node->children[3]->description->read(stream, node->children[3], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[4]->isBlocked = 1;
    node->children[4]->description = getMetaClassDescriptionByIndex(Sphere);
    node->children[4]->description->read(stream, node->children[4], flags);

    node->children[5]->description = getMetaClassDescriptionByIndex(Flags);
    node->children[5]->description->read(stream, node->children[5], flags);

    for (uint16_t i = 6; i < 13; ++i)
    {
        node->children[i]->description = getMetaClassDescriptionByIndex(unsignedlong);
        node->children[i]->description->read(stream, node->children[i], flags);
    }

    node->children[13]->description = getMetaClassDescriptionByIndex(float_type);
    node->children[13]->description->read(stream, node->children[13], flags);

    node->children[14]->description = getMetaClassDescriptionByIndex(float_type);
    node->children[14]->description->read(stream, node->children[14], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[15]->isBlocked = 1;
    node->children[15]->description = getMetaClassDescriptionByIndex(DCArray_Symbol_);
    node->children[15]->description->read(stream, node->children[15], flags);

    return 0;
}

static int T3MeshTextureRead(FILE *stream, struct TreeNode *node, uint32_t flags)
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

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[1]->isBlocked = 1;
    node->children[1]->description = getMetaClassDescriptionByIndex(Handle_T3Texture_);
    node->children[1]->description->read(stream, node->children[1], flags);

    node->children[2]->description = getMetaClassDescriptionByIndex(Symbol);
    node->children[2]->description->read(stream, node->children[2], flags);

    node->children[3]->description = getMetaClassDescriptionByIndex(BoundingBox);
    node->children[3]->description->read(stream, node->children[3], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[4]->isBlocked = 1;
    node->children[4]->description = getMetaClassDescriptionByIndex(Sphere);
    node->children[4]->description->read(stream, node->children[4], flags);

    node->children[5]->description = getMetaClassDescriptionByIndex(float_type);
    node->children[5]->description->read(stream, node->children[5], flags);

    node->children[6]->description = getMetaClassDescriptionByIndex(float_type);
    node->children[6]->description->read(stream, node->children[6], flags);

    return 0;
}

int T3MeshMaterialRead(FILE *stream, struct TreeNode *node, uint32_t flags)
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
    node->children[0]->description = getMetaClassDescriptionByIndex(Handle_PropertySet_);
    node->children[0]->description->read(stream, node->children[0], flags);

    node->children[1]->description = getMetaClassDescriptionByIndex(Symbol);
    node->children[1]->description->read(stream, node->children[1], flags);

    node->children[2]->description = getMetaClassDescriptionByIndex(Symbol);
    node->children[2]->description->read(stream, node->children[2], flags);

    node->children[3]->description = getMetaClassDescriptionByIndex(BoundingBox);
    node->children[3]->description->read(stream, node->children[3], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[4]->isBlocked = 1;
    node->children[4]->description = getMetaClassDescriptionByIndex(Sphere);
    node->children[4]->description->read(stream, node->children[4], flags);

    node->children[5]->description = getMetaClassDescriptionByIndex(Flags);
    node->children[5]->description->read(stream, node->children[5], flags);

    return 0;
}

static int T3MeshMaterialOverrideRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 2;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->isBlocked = 1;
    node->children[0]->description = getMetaClassDescriptionByIndex(Handle_PropertySet_);
    node->children[0]->description->read(stream, node->children[0], flags);

    node->children[1]->description = getMetaClassDescriptionByIndex(unsignedlong);
    node->children[1]->description->read(stream, node->children[1], flags);

    return 0;
}

int T3MeshBoneEntryRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 4;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    node->children[0]->description = getMetaClassDescriptionByIndex(Symbol);
    node->children[0]->description->read(stream, node->children[0], flags);

    node->children[1]->description = getMetaClassDescriptionByIndex(BoundingBox);
    node->children[1]->description->read(stream, node->children[1], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[2]->isBlocked = 1;
    node->children[2]->description = getMetaClassDescriptionByIndex(Sphere);
    node->children[2]->description->read(stream, node->children[2], flags);

    node->children[3]->description = getMetaClassDescriptionByIndex(long_type);
    node->children[3]->description->read(stream, node->children[3], flags);

    return 0;
}

int T3MeshLocalTransformEntryRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 2;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->isBlocked = 1;
    node->children[0]->description = getMetaClassDescriptionByIndex(Transform);
    node->children[0]->description->read(stream, node->children[0], flags);

    node->children[1]->description = getMetaClassDescriptionByIndex(long_type);
    node->children[1]->description->read(stream, node->children[1], flags);

    return 0;
}

static int T3MeshTexCoordTransformRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 2;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
        node->children[i]->description = getMetaClassDescriptionByIndex(Vector2);
        node->children[i]->description->read(stream, node->children[i], flags);
    }

    return 0;
}

static int T3MeshCPUSkinningDataRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 9;
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

    node->children[2]->description = getMetaClassDescriptionByIndex(long_type);
    node->children[2]->description->read(stream, node->children[2], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[3]->isBlocked = 1;
    node->children[3]->description = getMetaClassDescriptionByIndex(BitSetBase_1_);
    node->children[3]->description->read(stream, node->children[3], flags);

    node->children[4]->description = getMetaClassDescriptionByIndex(unsignedlong);
    node->children[4]->description->read(stream, node->children[4], flags);

    node->children[5]->description = getMetaClassDescriptionByIndex(unsignedlong);
    node->children[5]->description->read(stream, node->children[5], flags);

    node->children[6]->description = getMetaClassDescriptionByIndex(unsignedlong);
    node->children[6]->description->read(stream, node->children[6], flags);

    node->children[7]->description = getMetaClassDescriptionByIndex(unsignedlong);
    node->children[7]->description->read(stream, node->children[7], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[8]->isBlocked = 1;
    node->children[8]->description = getMetaClassDescriptionByIndex(BinaryBuffer);
    node->children[8]->description->read(stream, node->children[8], flags);

    return 0;
}

int GFXPlatformAttributeParamsRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->dataSize = sizeof(int32_t) * 5; // TODO: maybe change to children
    node->data.dynamicBuffer = malloc(node->dataSize);
    fread(node->data.dynamicBuffer, node->dataSize, 1, stream);

    return 0;
}

int T3GFXVertexStateRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{

    uint32_t buf[4];
    fread(buf, sizeof(buf), 1, stream);
    cfseek(stream, -sizeof(buf), SEEK_CUR);

    node->childCount = 4 + buf[1] + buf[2] + buf[3];
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    node->children[0]->description = getMetaClassDescriptionByIndex(unsignedlong);
    node->children[0]->description->read(stream, node->children[0], flags);

    node->children[1]->description = getMetaClassDescriptionByIndex(unsignedlong);
    node->children[1]->description->read(stream, node->children[1], flags);

    node->children[2]->description = getMetaClassDescriptionByIndex(unsignedlong);
    node->children[2]->description->read(stream, node->children[2], flags);

    node->children[3]->description = getMetaClassDescriptionByIndex(unsignedlong);
    node->children[3]->description->read(stream, node->children[3], flags);

    for (uint16_t i = 4; i < 4 + buf[3]; ++i)
    {
        node->children[i]->description = getMetaClassDescriptionByIndex(GFXPlatformAttributeParams);
        node->children[i]->description->read(stream, node->children[i], flags);
    }
    for (uint16_t i = 4 + buf[3]; i < 4 + buf[3] + buf[1]; ++i)
    {
        node->children[i]->description = getMetaClassDescriptionByIndex(T3GFXBuffer);
        node->children[i]->description->read(stream, node->children[i], flags);
    }
    for (uint16_t i = 4 + buf[3] + buf[1]; i < node->childCount; ++i)
    {
        node->children[i]->description = getMetaClassDescriptionByIndex(T3GFXBuffer);
        node->children[i]->description->read(stream, node->children[i], flags);
    }

    return 0;
}

int T3GFXBufferRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->dataSize = sizeof(int32_t) * 5; // TODO: maybe change to children
    node->data.dynamicBuffer = malloc(node->dataSize);
    fread(node->data.dynamicBuffer, node->dataSize, 1, stream);

    return 0;
}

int T3MeshEffectPreloadDynamicFeaturesRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 2;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->isBlocked = 1;
    node->children[0]->description = getMetaClassDescriptionByIndex(BitSetBase_1_);
    node->children[0]->description->read(stream, node->children[0], flags);

    node->children[1]->description = getMetaClassDescriptionByIndex(long_type);
    node->children[1]->description->read(stream, node->children[1], flags);

    return 0;
}

int DCArray_T3MeshEffectPreloadDynamicFeatures_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(T3MeshEffectPreloadDynamicFeatures));
}

int T3MeshEffectPreloadEntryRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 4;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    node->children[0]->description = getMetaClassDescriptionByIndex(unsignedlong);
    node->children[0]->description->read(stream, node->children[0], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[1]->isBlocked = 1;
    node->children[1]->description = getMetaClassDescriptionByIndex(BitSetBase_3_);
    node->children[1]->description->read(stream, node->children[1], flags);

    node->children[2]->description = getMetaClassDescriptionByIndex(unsigned__int64);
    node->children[2]->description->read(stream, node->children[2], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[3]->isBlocked = 1;
    node->children[3]->description = getMetaClassDescriptionByIndex(DCArray_T3MeshEffectPreloadDynamicFeatures_);
    node->children[3]->description->read(stream, node->children[3], flags);

    return 0;
}

int DCArray_T3MeshEffectPreloadEntry_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(T3MeshEffectPreloadEntry));
}

int T3MeshEffectPreloadRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 3;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    node->children[0]->description = getMetaClassDescriptionByIndex(long_type);
    node->children[0]->description->read(stream, node->children[0], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[1]->isBlocked = 1;
    node->children[1]->description = getMetaClassDescriptionByIndex(DCArray_T3MeshEffectPreloadEntry_);
    node->children[1]->description->read(stream, node->children[1], flags);

    node->children[2]->description = getMetaClassDescriptionByIndex(long_type);
    node->children[2]->description->read(stream, node->children[2], flags);

    return 0;
}

int DCArray_T3MeshEffectPreload_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(T3MeshEffectPreload));
}

int T3MaterialRequirementsRead(FILE *stream, struct TreeNode *node, uint32_t flags)
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
    node->children[0]->description = getMetaClassDescriptionByIndex(BitSetBase_1_);
    node->children[0]->description->read(stream, node->children[0], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[1]->isBlocked = 1;
    node->children[1]->description = getMetaClassDescriptionByIndex(BitSetBase_2_);
    node->children[1]->description->read(stream, node->children[1], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[2]->isBlocked = 1;
    node->children[2]->description = getMetaClassDescriptionByIndex(BitSetBase_3_);
    node->children[2]->description->read(stream, node->children[2], flags);

    return 0;
}

int DCArray_T3MeshLOD_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(T3MeshLOD));
}

int DCArray_T3MeshTexture_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
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
        node->children[i]->parent = node;
        T3MeshTextureRead(stream, node->children[i], flags); // TODO: Set description
    }
    return 0;
}

int DCArray_T3MeshMaterial_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(T3MeshMaterial));
}

int DCArray_T3MeshMaterialOverride_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
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
        node->children[i]->parent = node;
        T3MeshMaterialOverrideRead(stream, node->children[i], flags); // TODO: Set description
    }
    return 0;
}

int DCArray_T3MeshBoneEntry_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(T3MeshBoneEntry));
}

int DCArray_T3MeshLocalTransformEntry_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(T3MeshLocalTransformEntry));
}

int T3MeshDataRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 22;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->isBlocked = 1;
    node->children[0]->description = getMetaClassDescriptionByIndex(DCArray_T3MeshLOD_);
    node->children[0]->description->read(stream, node->children[0], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[1]->isBlocked = 1;
    node->children[1]->description = getMetaClassDescriptionByIndex(DCArray_T3MeshTexture_);
    node->children[1]->description->read(stream, node->children[1], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[2]->isBlocked = 1;
    node->children[2]->description = getMetaClassDescriptionByIndex(DCArray_T3MeshMaterial_);
    node->children[2]->description->read(stream, node->children[2], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[3]->isBlocked = 1;
    node->children[3]->description = getMetaClassDescriptionByIndex(DCArray_T3MeshMaterialOverride_);
    node->children[3]->description->read(stream, node->children[3], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[4]->isBlocked = 1;
    node->children[4]->description = getMetaClassDescriptionByIndex(DCArray_T3MeshBoneEntry_);
    node->children[4]->description->read(stream, node->children[4], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[5]->isBlocked = 1;
    node->children[5]->description = getMetaClassDescriptionByIndex(DCArray_T3MeshLocalTransformEntry_);
    node->children[5]->description->read(stream, node->children[5], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[6]->isBlocked = 1;
    node->children[6]->description = getMetaClassDescriptionByIndex(T3MaterialRequirements);
    node->children[6]->description->read(stream, node->children[6], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[7]->isBlocked = 1;
    node->children[7]->description = getMetaClassDescriptionByIndex(BitSetBase_1_);
    node->children[7]->description->read(stream, node->children[7], flags);

    node->children[8]->description = getMetaClassDescriptionByIndex(BoundingBox);
    node->children[8]->description->read(stream, node->children[8], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[9]->isBlocked = 1;
    node->children[9]->description = getMetaClassDescriptionByIndex(Sphere);
    node->children[9]->description->read(stream, node->children[9], flags);

    node->children[10]->description = getMetaClassDescriptionByIndex(long_type);
    node->children[10]->description->read(stream, node->children[10], flags);

    node->children[11]->description = getMetaClassDescriptionByIndex(Vector3);
    node->children[11]->description->read(stream, node->children[11], flags);

    node->children[12]->description = getMetaClassDescriptionByIndex(Vector3);
    node->children[12]->description->read(stream, node->children[12], flags);

    node->children[13]->description = getMetaClassDescriptionByIndex(Vector3);
    node->children[13]->description->read(stream, node->children[13], flags);

    node->children[14]->description = getMetaClassDescriptionByIndex(float_type);
    node->children[14]->description->read(stream, node->children[14], flags);

    node->children[15]->description = getMetaClassDescriptionByIndex(Symbol);
    node->children[15]->description->read(stream, node->children[15], flags);

    node->children[16]->description = getMetaClassDescriptionByIndex(unsignedlong);
    node->children[16]->description->read(stream, node->children[16], flags);

    node->children[17]->description = getMetaClassDescriptionByIndex(Flags);
    node->children[17]->description->read(stream, node->children[17], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[18]->isBlocked = 1;
    node->children[18]->description = getMetaClassDescriptionByIndex(DCArray_T3MeshEffectPreload_);
    node->children[18]->description->read(stream, node->children[18], flags);

    node->children[19]->childCount = 1;
    node->children[19]->children = malloc(node->children[19]->childCount * sizeof(struct TreeNode *));
    node->children[19]->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[19]->children[0]->parent = node;
    node->children[19]->children[0]->description = getMetaClassDescriptionByIndex(int_type);
    node->children[19]->children[0]->description->read(stream, node->children[19]->children[0], flags);
    node->children[19]->childCount += (*(uint32_t *)(node->children[19]->children[0]->data.staticBuffer)) * 2;
    node->children[19]->children = realloc(node->children[19]->children, node->children[19]->childCount * sizeof(struct TreeNode *));

    for (uint32_t i = 1; i < node->children[19]->childCount; ++i)
    {
        node->children[19]->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[19]->children[i]->parent = node;
        if (i % 2) // if odd
        {
            node->children[19]->children[i]->description = getMetaClassDescriptionByIndex(int_type);
            node->children[19]->children[i]->description->read(stream, node->children[19]->children[i], flags);
        }
        else
        {
            node->children[19]->children[i]->description; // TODO: set description
            T3MeshTexCoordTransformRead(stream, node->children[19]->children[i], flags);
        }
    }

    if ((((*(uint32_t *)node->children[17]->data.staticBuffer) >> 4) & 1) != 0)
    {
        node->children[20]->description; // TODO: Set description
        T3MeshCPUSkinningDataRead(stream, node->children[20], flags);
    }

    node->children[21]->description; // TODO: Set description
    genericArrayRead(stream, node->children[21], flags, getMetaClassDescriptionByIndex(T3GFXVertexState));

    return 0;
}

static int T3OcclusionMeshBatchRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->dataSize = sizeof(uint32_t) * 3;
    node->data.dynamicBuffer = malloc(node->dataSize);
    fread(node->data.dynamicBuffer, node->dataSize, 1, stream);

    return 0;
}

int DCArray_T3OcclusionMeshBatch_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
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
        node->children[i]->parent = node;
        T3OcclusionMeshBatchRead(stream, node->children[i], flags); // TODO: Set description
    }
    return 0;
}

int T3OcclusionMeshDataRead(FILE *stream, struct TreeNode *node, uint32_t flags)
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
    node->children[0]->description = getMetaClassDescriptionByIndex(BinaryBuffer);
    node->children[0]->description->read(stream, node->children[0], flags);

    node->children[1]->description = getMetaClassDescriptionByIndex(BoundingBox);
    node->children[1]->description->read(stream, node->children[1], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[2]->isBlocked = 1;
    node->children[2]->description = getMetaClassDescriptionByIndex(Sphere);
    node->children[2]->description->read(stream, node->children[2], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[3]->isBlocked = 1;
    node->children[3]->description = getMetaClassDescriptionByIndex(DCArray_T3OcclusionMeshBatch_);
    node->children[3]->description->read(stream, node->children[3], flags);

    node->children[4]->description = getMetaClassDescriptionByIndex(unsignedlong);
    node->children[4]->description->read(stream, node->children[4], flags);

    return 0;
}

int DCArray_HandleBase_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(HandleBase));
}

int D3DMeshRead(FILE *stream, struct TreeNode *node, uint32_t flags)
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
    node->children[0]->description = getMetaClassDescriptionByIndex(String);
    node->children[0]->description->read(stream, node->children[0], flags);

    node->children[1]->description = getMetaClassDescriptionByIndex(unsignedlong);
    node->children[1]->description->read(stream, node->children[1], flags);

    node->children[2]->description = getMetaClassDescriptionByIndex(ToolProps);
    node->children[2]->description->read(stream, node->children[2], flags);

    node->children[3]->description = getMetaClassDescriptionByIndex(float_type);
    node->children[3]->description->read(stream, node->children[3], flags);

    node->children[4]->description = getMetaClassDescriptionByIndex(long_type);
    node->children[4]->description->read(stream, node->children[4], flags);

    node->children[5]->description = getMetaClassDescriptionByIndex(unsigned__int64);
    node->children[5]->description->read(stream, node->children[5], flags);

    uint32_t mapCount;
    fread(&mapCount, sizeof(mapCount), 1, stream);
    cfseek(stream, -sizeof(mapCount), SEEK_CUR);

    node->children[6]->childCount = (mapCount * 2) + 1;
    node->children[6]->children = malloc(node->children[6]->childCount * sizeof(struct TreeNode *));

    node->children[6]->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[6]->children[0]->parent = node->children[6];
    node->children[6]->children[0]->description = getMetaClassDescriptionByIndex(int_type);
    node->children[6]->children[0]->description->read(stream, node->children[6]->children[0], flags);

    for (uint16_t i = 1; i < node->children[6]->childCount; ++i)
    {
        node->children[6]->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[6]->children[i]->parent = node->children[6];
        if (i % 2)
        {
            node->children[6]->children[i]->description = getMetaClassDescriptionByIndex(Symbol);
            node->children[6]->children[i]->description->read(stream, node->children[6]->children[i], flags);
        }
        else
        {
            uint64_t typeSymbol;
            fread(&typeSymbol, sizeof(typeSymbol), 1, stream);
            node->children[6]->children[i]->serializeType = 1;

            cfseek(stream, sizeof(uint32_t), SEEK_CUR);
            node->children[6]->children[i]->isBlocked = 1;
            node->children[6]->children[i]->description = getMetaClassDescriptionBySymbol(typeSymbol);
            node->children[6]->children[i]->description->read(stream, node->children[6]->children[i], flags);
        }
    }

    node->children[7]->description = getMetaClassDescriptionByIndex(int_type);
    node->children[7]->description->read(stream, node->children[7], flags);

    node->children[8]->description = getMetaClassDescriptionByIndex(bool_type);
    node->children[8]->description->read(stream, node->children[8], flags);

    if (*node->children[8]->data.staticBuffer == '1')
    {
        node->children = realloc(node->children, (++node->childCount) * sizeof(struct TreeNode *));
        node->children[node->childCount - 1] = calloc(1, sizeof(struct TreeNode));
        node->children[node->childCount - 1]->parent = node;

        cfseek(stream, sizeof(uint32_t), SEEK_CUR);
        node->children[9]->isBlocked = 1;
        node->children[9]->description = getMetaClassDescriptionByIndex(T3OcclusionMeshData);
        node->children[9]->description->read(stream, node->children[9], flags);
    }

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[node->childCount - 2]->isBlocked = 1;
    node->children[node->childCount - 2]->description = getMetaClassDescriptionByIndex(T3MeshData);
    node->children[node->childCount - 2]->description->read(stream, node->children[node->childCount - 2], flags);

    node->children[node->childCount - 1]->description;
    int64_t startOfAsync = ftell(stream);
    cfseek(stream, 0, SEEK_END);
    node->children[node->childCount - 1]->dataSize = cftell(stream) - startOfAsync;
    node->children[node->childCount - 1]->data.dynamicBuffer = malloc(node->children[node->childCount - 1]->dataSize);
    cfseek(stream, startOfAsync, SEEK_SET);
    fread(node->children[node->childCount - 1]->data.dynamicBuffer, node->children[node->childCount - 1]->dataSize, 1, stream);

    return 0;
}

int T3MaterialRuntimePropertyRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 2;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
        node->children[i]->description = getMetaClassDescriptionByIndex(Symbol);
        node->children[i]->description->read(stream, node->children[i], flags);
    }

    return 0;
}

int DCArray_T3MaterialRuntimeProperty_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(T3MaterialRuntimeProperty));
}

int T3MaterialParameterRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->dataSize = sizeof(uint64_t) + 6 * sizeof(int32_t); // TODO: Maybe change to children
    node->data.dynamicBuffer = malloc(node->dataSize);
    fread(node->data.dynamicBuffer, node->dataSize, 1, stream);

    return 0;
}

int DCArray_T3MaterialParameter_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(T3MaterialParameter));
}

int T3MaterialTextureRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 10;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    node->children[0]->description = getMetaClassDescriptionByIndex(Symbol);
    node->children[0]->description->read(stream, node->children[0], flags);

    node->children[1]->description = getMetaClassDescriptionByIndex(Symbol);
    node->children[1]->description->read(stream, node->children[1], flags);

    node->children[2]->description = getMetaClassDescriptionByIndex(Symbol);
    node->children[2]->description->read(stream, node->children[2], flags);

    node->children[3]->description = getMetaClassDescriptionByIndex(long_type);
    node->children[3]->description->read(stream, node->children[3], flags);

    node->children[4]->description = getMetaClassDescriptionByIndex(long_type);
    node->children[4]->description->read(stream, node->children[4], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[5]->isBlocked = 1;
    node->children[5]->description = getMetaClassDescriptionByIndex(BitSetBase_1_);
    node->children[5]->description->read(stream, node->children[5], flags);

    node->children[6]->description = getMetaClassDescriptionByIndex(unsignedlong);
    node->children[6]->description->read(stream, node->children[6], flags);

    node->children[7]->description = getMetaClassDescriptionByIndex(unsignedlong);
    node->children[7]->description->read(stream, node->children[7], flags);

    node->children[8]->description = getMetaClassDescriptionByIndex(long_type);
    node->children[8]->description->read(stream, node->children[8], flags);

    node->children[9]->description = getMetaClassDescriptionByIndex(long_type);
    node->children[9]->description->read(stream, node->children[9], flags);

    return 0;
}

int DCArray_T3MaterialTexture_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(T3MaterialTexture));
}

int T3MaterialTransform2DRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->dataSize = sizeof(uint64_t) + 6 * sizeof(int32_t); // TODO: Maybe change to children
    node->data.dynamicBuffer = malloc(node->dataSize);
    fread(node->data.dynamicBuffer, node->dataSize, 1, stream);

    return 0;
}

int DCArray_T3MaterialTransform2D_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(T3MaterialTransform2D));
}

static int T3MaterialNestedMaterialRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 1;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->parent = node;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->isBlocked = 1;
    node->children[0]->description = getMetaClassDescriptionByIndex(Handle_PropertySet_);
    node->children[0]->description->read(stream, node->children[0], flags);

    return 0;
}

int DCArray_T3MaterialNestedMaterial_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
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
        node->children[i]->parent = node;
        T3MaterialNestedMaterialRead(stream, node->children[i], flags); // TODO: Set description
    }
    return 0;
}

int T3MaterialPreShaderRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->dataSize = 4 * sizeof(int32_t); // TODO: Maybe change to children
    node->data.dynamicBuffer = malloc(node->dataSize);
    fread(node->data.dynamicBuffer, node->dataSize, 1, stream);

    return 0;
}

int DCArray_T3MaterialPreShader_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(T3MaterialPreShader));
}

int T3MaterialStaticParameterRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->dataSize = sizeof(uint64_t) + sizeof(int32_t); // TODO: Maybe change to children
    node->data.dynamicBuffer = malloc(node->dataSize);
    fread(node->data.dynamicBuffer, node->dataSize, 1, stream);

    return 0;
}

int DCArray_T3MaterialStaticParameter_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(T3MaterialStaticParameter));
}

static int T3MaterialTextureParamRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->dataSize = 4 * sizeof(int32_t); // TODO: Maybe change to children
    node->data.dynamicBuffer = malloc(node->dataSize);
    fread(node->data.dynamicBuffer, node->dataSize, 1, stream);

    return 0;
}

int DCArray_T3MaterialTextureParam_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
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
        node->children[i]->parent = node;
        T3MaterialTextureParamRead(stream, node->children[i], flags); // TODO: Set description
    }
    return 0;
}

int T3MaterialPassDataRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->dataSize = 2 * sizeof(int32_t) + sizeof(uint64_t); // TODO: Maybe change to children
    node->data.dynamicBuffer = malloc(node->dataSize);
    fread(node->data.dynamicBuffer, node->dataSize, 1, stream);

    return 0;
}

int DCArray_T3MaterialPassData_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(T3MaterialPassData));
}

int T3MaterialCompiledDataRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 20;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->isBlocked = 1;
    node->children[0]->description = getMetaClassDescriptionByIndex(DCArray_T3MaterialParameter_);
    node->children[0]->description->read(stream, node->children[0], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[1]->isBlocked = 1;
    node->children[1]->description = getMetaClassDescriptionByIndex(DCArray_T3MaterialTexture_);
    node->children[1]->description->read(stream, node->children[1], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[2]->isBlocked = 1;
    node->children[2]->description = getMetaClassDescriptionByIndex(DCArray_T3MaterialTransform2D_);
    node->children[2]->description->read(stream, node->children[2], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[3]->isBlocked = 1;
    node->children[3]->description = getMetaClassDescriptionByIndex(DCArray_T3MaterialNestedMaterial_);
    node->children[3]->description->read(stream, node->children[3], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[4]->isBlocked = 1;
    node->children[4]->description = getMetaClassDescriptionByIndex(DCArray_T3MaterialPreShader_);
    node->children[4]->description->read(stream, node->children[4], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[5]->isBlocked = 1;
    node->children[5]->description = getMetaClassDescriptionByIndex(DCArray_T3MaterialStaticParameter_);
    node->children[5]->description->read(stream, node->children[5], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[6]->isBlocked = 1;
    node->children[6]->description = getMetaClassDescriptionByIndex(DCArray_T3MaterialTextureParam_);
    node->children[6]->description->read(stream, node->children[6], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[7]->isBlocked = 1;
    node->children[7]->description = getMetaClassDescriptionByIndex(DCArray_T3MaterialPassData_);
    node->children[7]->description->read(stream, node->children[7], flags);

    node->children[8]->description = getMetaClassDescriptionByIndex(long_type);
    node->children[8]->description->read(stream, node->children[8], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[9]->isBlocked = 1;
    node->children[9]->description = getMetaClassDescriptionByIndex(BitSetBase_1_);
    node->children[9]->description->read(stream, node->children[9], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[10]->isBlocked = 1;
    node->children[10]->description = getMetaClassDescriptionByIndex(BitSetBase_1_);
    node->children[10]->description->read(stream, node->children[10], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[11]->isBlocked = 1;
    node->children[11]->description = getMetaClassDescriptionByIndex(BitSetBase_2_);
    node->children[11]->description->read(stream, node->children[11], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[12]->isBlocked = 1;
    node->children[12]->description = getMetaClassDescriptionByIndex(BitSetBase_3_);
    node->children[12]->description->read(stream, node->children[12], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[13]->isBlocked = 1;
    node->children[13]->description = getMetaClassDescriptionByIndex(BitSetBase_1_);
    node->children[13]->description->read(stream, node->children[13], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[14]->isBlocked = 1;
    node->children[14]->description = getMetaClassDescriptionByIndex(BitSetBase_1_);
    node->children[14]->description->read(stream, node->children[14], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[15]->isBlocked = 1;
    node->children[15]->description = getMetaClassDescriptionByIndex(BinaryBuffer);
    node->children[15]->description->read(stream, node->children[15], flags);

    node->children[16]->description = getMetaClassDescriptionByIndex(Flags);
    node->children[16]->description->read(stream, node->children[16], flags);

    node->children[17]->description = getMetaClassDescriptionByIndex(unsignedlong);
    node->children[17]->description->read(stream, node->children[17], flags);

    node->children[18]->description = getMetaClassDescriptionByIndex(unsignedlong);
    node->children[18]->description->read(stream, node->children[18], flags);

    node->children[19]->description = getMetaClassDescriptionByIndex(unsignedlong);
    node->children[19]->description->read(stream, node->children[19], flags);

    return 0;
}

int DCArray_T3MaterialCompiledData_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(T3MaterialCompiledData));
}

int T3MaterialDataRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 10;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    for (uint16_t i = 0; i < 4; ++i)
    {
        node->children[i]->description = getMetaClassDescriptionByIndex(Symbol);
        node->children[i]->description->read(stream, node->children[i], flags);
    }

    node->children[4]->description = getMetaClassDescriptionByIndex(long_type);
    node->children[4]->description->read(stream, node->children[4], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[5]->isBlocked = 1;
    node->children[5]->description = getMetaClassDescriptionByIndex(DCArray_T3MaterialRuntimeProperty_);
    node->children[5]->description->read(stream, node->children[5], flags);

    node->children[6]->description = getMetaClassDescriptionByIndex(Flags);
    node->children[6]->description->read(stream, node->children[6], flags);

    node->children[7]->description = getMetaClassDescriptionByIndex(Flags);
    node->children[7]->description->read(stream, node->children[7], flags);

    node->children[8]->description = getMetaClassDescriptionByIndex(long_type);
    node->children[8]->description->read(stream, node->children[8], flags);

    node->children[9]->description = getMetaClassDescriptionByIndex(DCArray_T3MaterialCompiledData_);
    node->children[9]->description->read(stream, node->children[9], flags);

    return 0;
}
