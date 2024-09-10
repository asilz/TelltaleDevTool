#include <tree.h>
#include <stream.h>
#include <container.h>
#include <stdlib.h>
#include <types.h>
#include <meta.h>
#include <assert.h>

enum MeshFlags // T3MeshData
{
    eDeformable = 0x1,
    eHasLocalCameraFacing = 0x2,
    eHasLocalCameraFacingY = 0x4,
    eHasLocalCameraFacingLocalY = 0x8,
    eHasCPUSkinning = 0x10,
    eHasComputeSkinning = 0x20
};

int BoundingBoxRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 0, .memberName = "mMin", .metaClassDescriptionIndex = Vector3},
        {.isBlocked = 0, .memberName = "mMax", .metaClassDescriptionIndex = Vector3},
    };
    return genericRead(stream, node, flags, 2, descriptions);
}

int SphereRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 0, .memberName = "mCentre", .metaClassDescriptionIndex = Vector3},
        {.isBlocked = 0, .memberName = "mRadius", .metaClassDescriptionIndex = float_type},
    };
    return genericRead(stream, node, flags, 2, descriptions);
}

int BitSetBase_1_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    (void)flags;
    node->dataSize = sizeof(int32_t);
    fread(node->staticBuffer, node->dataSize, 1, stream);

    return 0;
}

int BitSetBase_2_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    (void)flags;
    node->dataSize = 2 * sizeof(int32_t);
    fread(node->staticBuffer, node->dataSize, 1, stream);

    return 0;
}

int BitSetBase_3_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    (void)flags;
    node->dataSize = 3 * sizeof(int32_t);
    node->dynamicBuffer = malloc(node->dataSize);
    fread(node->dynamicBuffer, node->dataSize, 1, stream);

    return 0;
}

int T3MeshTextureIndicesRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    (void)flags;
    uint32_t buffer;
    do
    {
        fread(&buffer, sizeof(buffer), 1, stream);
        node->dataSize += sizeof(uint32_t);
    } while (buffer != 0xFFFFFFFF && node->dataSize < 8 * sizeof(uint32_t));
    cfseek(stream, -(int64_t)(node->dataSize), SEEK_CUR);

    if (node->dataSize <= sizeof(node->staticBuffer))
    {
        fread(node->staticBuffer, node->dataSize, 1, stream);
    }
    else
    {
        node->dynamicBuffer = malloc(node->dataSize);
        fread(node->dynamicBuffer, node->dataSize, 1, stream);
    }

    return 0;
}

int T3MeshBatchRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 0, .memberName = "mBoundingBox", .metaClassDescriptionIndex = BoundingBox},
        {.isBlocked = 1, .memberName = "mBoundingSphere", .metaClassDescriptionIndex = Sphere},
        {.isBlocked = 0, .memberName = "mBatchUsage", .metaClassDescriptionIndex = Flags},
        {.isBlocked = 0, .memberName = "mMinVertIndex", .metaClassDescriptionIndex = unsignedlong},
        {.isBlocked = 0, .memberName = "mMaxVertIndex", .metaClassDescriptionIndex = unsignedlong},
        {.isBlocked = 0, .memberName = "mBaseIndex", .metaClassDescriptionIndex = unsignedlong},
        {.isBlocked = 0, .memberName = "mStartIndex", .metaClassDescriptionIndex = unsignedlong},
        {.isBlocked = 0, .memberName = "mNumPrimitives", .metaClassDescriptionIndex = unsignedlong},
        {.isBlocked = 0, .memberName = "mNumIndices", .metaClassDescriptionIndex = unsignedlong},
        {.isBlocked = 1, .memberName = "mTextureIndices", .metaClassDescriptionIndex = T3MeshTextureIndices},
        {.isBlocked = 0, .memberName = "mMaterialIndex", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 0, .memberName = "mAdjacencyStartIndex", .metaClassDescriptionIndex = unsignedlong},
    };
    return genericRead(stream, node, flags, 12, descriptions);
}

int DCArray_T3MeshBatch_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(T3MeshBatch));
}

int T3MeshLODRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "mBatches[0]", .metaClassDescriptionIndex = DCArray_T3MeshBatch_},
        {.isBlocked = 1, .memberName = "mBatches[1]", .metaClassDescriptionIndex = DCArray_T3MeshBatch_},
        {.isBlocked = 1, .memberName = "mVertexStreams", .metaClassDescriptionIndex = BitSetBase_1_},
        {.isBlocked = 0, .memberName = "mBoundingBox", .metaClassDescriptionIndex = BoundingBox},
        {.isBlocked = 1, .memberName = "mBoundingSphere", .metaClassDescriptionIndex = Sphere},
        {.isBlocked = 0, .memberName = "mFlags", .metaClassDescriptionIndex = Flags},
        {.isBlocked = 0, .memberName = "mVertexStateIndex", .metaClassDescriptionIndex = unsignedlong},
        {.isBlocked = 0, .memberName = "mNumPrimitives", .metaClassDescriptionIndex = unsignedlong},
        {.isBlocked = 0, .memberName = "mNumBatches", .metaClassDescriptionIndex = unsignedlong},
        {.isBlocked = 0, .memberName = "mVertexStart", .metaClassDescriptionIndex = unsignedlong},
        {.isBlocked = 0, .memberName = "mVertexCount", .metaClassDescriptionIndex = unsignedlong},
        {.isBlocked = 0, .memberName = "mTextureAtlasWidth", .metaClassDescriptionIndex = unsignedlong},
        {.isBlocked = 0, .memberName = "mTextureAtlasHeight", .metaClassDescriptionIndex = unsignedlong},
        {.isBlocked = 0, .memberName = "mPixelSize", .metaClassDescriptionIndex = float_type},
        {.isBlocked = 0, .memberName = "mDistance", .metaClassDescriptionIndex = float_type},
        {.isBlocked = 1, .memberName = "mBones", .metaClassDescriptionIndex = DCArray_Symbol_},
    };
    return genericRead(stream, node, flags, 16, descriptions);
}

static int T3MeshTextureRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 0, .memberName = "mTextureType", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 1, .memberName = "mhTexture", .metaClassDescriptionIndex = Handle_T3Texture_},
        {.isBlocked = 0, .memberName = "mNameSymbol", .metaClassDescriptionIndex = Symbol},
        {.isBlocked = 0, .memberName = "mBoundingBox", .metaClassDescriptionIndex = BoundingBox},
        {.isBlocked = 1, .memberName = "mBoundingSphere", .metaClassDescriptionIndex = Sphere},
        {.isBlocked = 0, .memberName = "mMaxObjAreaPerUVArea", .metaClassDescriptionIndex = float_type},
        {.isBlocked = 0, .memberName = "mAverageObjAreaPerUVArea", .metaClassDescriptionIndex = float_type},
    };
    return genericRead(stream, node, flags, 7, descriptions);
}

int T3MeshMaterialRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "mhMaterial", .metaClassDescriptionIndex = Handle_PropertySet_},
        {.isBlocked = 0, .memberName = "mBaseMaterialName", .metaClassDescriptionIndex = Symbol},
        {.isBlocked = 0, .memberName = "mLegacyRenderTextureProperty", .metaClassDescriptionIndex = Symbol},
        {.isBlocked = 0, .memberName = "mBoundingBox", .metaClassDescriptionIndex = BoundingBox},
        {.isBlocked = 1, .memberName = "mBoundingSphere", .metaClassDescriptionIndex = Sphere},
        {.isBlocked = 0, .memberName = "mFlags", .metaClassDescriptionIndex = Flags},
    };
    return genericRead(stream, node, flags, 6, descriptions);
}

static int T3MeshMaterialOverrideRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "mhOverrideMaterial", .metaClassDescriptionIndex = Handle_PropertySet_},
        {.isBlocked = 0, .memberName = "mMaterialIndex", .metaClassDescriptionIndex = unsignedlong},
    };
    return genericRead(stream, node, flags, 2, descriptions);
}

int T3MeshBoneEntryRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 0, .memberName = "mBoneName", .metaClassDescriptionIndex = Symbol},
        {.isBlocked = 0, .memberName = "mBoundingBox", .metaClassDescriptionIndex = BoundingBox},
        {.isBlocked = 1, .memberName = "mBoundingSphere", .metaClassDescriptionIndex = Sphere},
        {.isBlocked = 0, .memberName = "mNumVerts", .metaClassDescriptionIndex = long_type},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int T3MeshLocalTransformEntryRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "mTransform", .metaClassDescriptionIndex = Transform},
        {.isBlocked = 0, .memberName = "mCameraFacingType", .metaClassDescriptionIndex = long_type},
    };
    return genericRead(stream, node, flags, 2, descriptions);
}

static int T3MeshTexCoordTransformRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 0, .memberName = "mScale", .metaClassDescriptionIndex = Vector2},
        {.isBlocked = 0, .memberName = "mOffset", .metaClassDescriptionIndex = Vector2},
    };
    return genericRead(stream, node, flags, 2, descriptions);
}

static int T3MeshCPUSkinningDataRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 0, .memberName = "mPositionFormat", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 0, .memberName = "mWeightFormat", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 0, .memberName = "mNormalFormat", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 1, .memberName = "mVertexStreams", .metaClassDescriptionIndex = BitSetBase_1_},
        {.isBlocked = 0, .memberName = "mNormalCount", .metaClassDescriptionIndex = unsignedlong},
        {.isBlocked = 0, .memberName = "mWeightOffset", .metaClassDescriptionIndex = unsignedlong},
        {.isBlocked = 0, .memberName = "mVertexSize", .metaClassDescriptionIndex = unsignedlong},
        {.isBlocked = 0, .memberName = "mWeightSize", .metaClassDescriptionIndex = unsignedlong},
        {.isBlocked = 1, .memberName = "mData", .metaClassDescriptionIndex = BinaryBuffer},
    };
    return genericRead(stream, node, flags, 9, descriptions);
}

int GFXPlatformAttributeParamsRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    (void)flags;
    node->dataSize = sizeof(int32_t) * 5; // TODO: maybe change to children
    node->dynamicBuffer = malloc(node->dataSize);
    fread(node->dynamicBuffer, node->dataSize, 1, stream);

    return 0;
}

int T3GFXVertexStateRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->child = calloc(1, sizeof(struct TreeNode));
    struct TreeNode *currentNode = node->child;
    currentNode->parent = node;
    currentNode->memberName = "mVertexCountPerInstance";
    currentNode->description = getMetaClassDescriptionByIndex(unsignedlong);
    currentNode->description->read(stream, currentNode, flags);

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;
    currentNode->memberName = "mIndexBufferCount";
    currentNode->description = getMetaClassDescriptionByIndex(unsignedlong);
    currentNode->description->read(stream, currentNode, flags);
    uint32_t indexBufferCount = *(uint32_t *)(currentNode->staticBuffer);

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;
    currentNode->memberName = "mVertexBufferCount";
    currentNode->description = getMetaClassDescriptionByIndex(unsignedlong);
    currentNode->description->read(stream, currentNode, flags);
    uint32_t vertexBufferCount = *(uint32_t *)(currentNode->staticBuffer);

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;
    currentNode->memberName = "mAttributeCount";
    currentNode->description = getMetaClassDescriptionByIndex(unsignedlong);
    currentNode->description->read(stream, currentNode, flags);
    uint32_t attributeCount = *(uint32_t *)(currentNode->staticBuffer);

    for (uint32_t i = 0; i < attributeCount; ++i)
    {
        currentNode->sibling = calloc(1, sizeof(struct TreeNode));
        currentNode->sibling->parent = currentNode->parent;
        currentNode = currentNode->sibling;
        currentNode->memberName = "attribute";
        currentNode->description = getMetaClassDescriptionByIndex(GFXPlatformAttributeParams);
        currentNode->description->read(stream, currentNode, flags);
    }

    for (uint32_t i = 0; i < indexBufferCount; ++i)
    {
        currentNode->sibling = calloc(1, sizeof(struct TreeNode));
        currentNode->sibling->parent = currentNode->parent;
        currentNode = currentNode->sibling;
        currentNode->memberName = "indexBuffer";
        currentNode->description = getMetaClassDescriptionByIndex(T3GFXBuffer);
        currentNode->description->read(stream, currentNode, flags);
    }

    for (uint32_t i = 0; i < vertexBufferCount; ++i)
    {
        currentNode->sibling = calloc(1, sizeof(struct TreeNode));
        currentNode->sibling->parent = currentNode->parent;
        currentNode = currentNode->sibling;
        currentNode->memberName = "vertexBuffer";
        currentNode->description = getMetaClassDescriptionByIndex(T3GFXBuffer);
        currentNode->description->read(stream, currentNode, flags);
    }

    return 0;
}

int T3GFXBufferRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    (void)flags;
    node->dataSize = sizeof(int32_t) * 5; // TODO: maybe change to children
    node->dynamicBuffer = malloc(node->dataSize);
    fread(node->dynamicBuffer, node->dataSize, 1, stream);

    return 0;
}

int T3MeshEffectPreloadDynamicFeaturesRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "mDynamicFeatures", .metaClassDescriptionIndex = BitSetBase_1_},
        {.isBlocked = 0, .memberName = "mPriority", .metaClassDescriptionIndex = long_type},
    };
    return genericRead(stream, node, flags, 2, descriptions);
}

int DCArray_T3MeshEffectPreloadDynamicFeatures_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(T3MeshEffectPreloadDynamicFeatures));
}

int T3MeshEffectPreloadEntryRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 0, .memberName = "mEffectType", .metaClassDescriptionIndex = unsignedlong},
        {.isBlocked = 1, .memberName = "mStaticEffectFeatures", .metaClassDescriptionIndex = BitSetBase_3_},
        {.isBlocked = 0, .memberName = "mMaterialCRC", .metaClassDescriptionIndex = unsigned__int64},
        {.isBlocked = 1, .memberName = "mDynamicEffectFeatures", .metaClassDescriptionIndex = DCArray_T3MeshEffectPreloadDynamicFeatures_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int DCArray_T3MeshEffectPreloadEntry_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(T3MeshEffectPreloadEntry));
}

int T3MeshEffectPreloadRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 0, .memberName = "mEffectQuality", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 1, .memberName = "mEntries", .metaClassDescriptionIndex = DCArray_T3MeshEffectPreloadEntry_},
        {.isBlocked = 0, .memberName = "mTotalEffectCount", .metaClassDescriptionIndex = unsignedlong},
    };
    return genericRead(stream, node, flags, 3, descriptions);
}

int DCArray_T3MeshEffectPreload_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(T3MeshEffectPreload));
}

int T3MaterialRequirementsRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "mPasses", .metaClassDescriptionIndex = BitSetBase_1_},
        {.isBlocked = 1, .memberName = "mChannels", .metaClassDescriptionIndex = BitSetBase_2_},
        {.isBlocked = 1, .memberName = "mInputs", .metaClassDescriptionIndex = BitSetBase_3_},
    };
    return genericRead(stream, node, flags, 3, descriptions);
}

int DCArray_T3MeshLOD_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(T3MeshLOD));
}

int DCArray_T3MeshTexture_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
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
        T3MeshTextureRead(stream, currentNode, flags);
    }
    return 0;
}

int DCArray_T3MeshMaterial_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(T3MeshMaterial));
}

int DCArray_T3MeshMaterialOverride_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
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
        T3MeshMaterialOverrideRead(stream, currentNode, flags);
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
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "mLODs", .metaClassDescriptionIndex = DCArray_T3MeshLOD_},
        {.isBlocked = 1, .memberName = "mTextures", .metaClassDescriptionIndex = DCArray_T3MeshTexture_},
        {.isBlocked = 1, .memberName = "mMaterials", .metaClassDescriptionIndex = DCArray_T3MeshMaterial_},
        {.isBlocked = 1, .memberName = "mMaterialOverrides", .metaClassDescriptionIndex = DCArray_T3MeshMaterialOverride_},
        {.isBlocked = 1, .memberName = "mBones", .metaClassDescriptionIndex = DCArray_T3MeshBoneEntry_},
        {.isBlocked = 1, .memberName = "mLocalTransforms", .metaClassDescriptionIndex = DCArray_T3MeshLocalTransformEntry_},
        {.isBlocked = 1, .memberName = "mMaterialRequirements", .metaClassDescriptionIndex = T3MaterialRequirements},
        {.isBlocked = 1, .memberName = "mVertexStreams", .metaClassDescriptionIndex = BitSetBase_1_},
        {.isBlocked = 0, .memberName = "mBoundingBox", .metaClassDescriptionIndex = BoundingBox},
        {.isBlocked = 1, .memberName = "mBoundingSphere", .metaClassDescriptionIndex = Sphere},
        {.isBlocked = 0, .memberName = "mEndianType", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 0, .memberName = "mPositionScale", .metaClassDescriptionIndex = Vector3},
        {.isBlocked = 0, .memberName = "mPositionWScale", .metaClassDescriptionIndex = Vector3},
        {.isBlocked = 0, .memberName = "mPositionOffset", .metaClassDescriptionIndex = Vector3},
        {.isBlocked = 0, .memberName = "mLightmapTexelAreaPerSurfaceAre", .metaClassDescriptionIndex = float_type},
        {.isBlocked = 0, .memberName = "mPropertyKeyBase", .metaClassDescriptionIndex = Symbol},
        {.isBlocked = 0, .memberName = "mVertexCount", .metaClassDescriptionIndex = unsignedlong},
        {.isBlocked = 0, .memberName = "mFlags", .metaClassDescriptionIndex = Flags},
        {.isBlocked = 1, .memberName = "mMeshPreload", .metaClassDescriptionIndex = DCArray_T3MeshEffectPreload_},
    };
    genericRead(stream, node, flags, 19, descriptions);
    struct TreeNode *currentNode = node->child;
    currentNode = currentNode->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling;
    uint32_t meshDataFlags = *(uint32_t *)(currentNode->staticBuffer);
    currentNode = currentNode->sibling;

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;

    currentNode->child = malloc(sizeof(struct TreeNode));
    currentNode->child->description = getMetaClassDescriptionByIndex(int_type);
    currentNode->child->description->read(stream, currentNode->child, flags);
    currentNode->child->parent = node;
    currentNode->child->serializeType = 0;
    currentNode->child->memberName = "pairCount";
    currentNode->child->isBlocked = 0;
    currentNode->child->sibling = NULL;

    struct TreeNode *mapNode = currentNode->child;

    for (uint32_t i = 0; i < *(uint32_t *)(currentNode->child->staticBuffer) * 2; ++i)
    {
        mapNode->sibling = calloc(1, sizeof(struct TreeNode));
        mapNode = mapNode->sibling;
        mapNode->parent = node;
        if (i % 2) // if odd
        {
            T3MeshTexCoordTransformRead(stream, mapNode, flags);
        }
        else
        {
            mapNode->description = getMetaClassDescriptionByIndex(int_type);
            mapNode->description->read(stream, mapNode, flags);
        }
    }

    if (meshDataFlags & eHasCPUSkinning) // if there are no T3MeshCPUSkinningData, then T3SkinningStartegy will be eSkinningGPU_Vertex I think. Don't quote me on this since I did not completely look over the code in ghidra. I am partially making assumptions
    {
        currentNode->sibling = calloc(1, sizeof(struct TreeNode));
        currentNode->sibling->parent = currentNode->parent;
        currentNode = currentNode->sibling;
        currentNode->memberName = "mpCPUSkinningData";
        // node->children[20]->description; // TODO: Set description
        T3MeshCPUSkinningDataRead(stream, currentNode, flags);
    }

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;
    currentNode->memberName = "mVertexStates";
    // node->children[21]->description; // TODO: Set description
    genericArrayRead(stream, currentNode, flags, getMetaClassDescriptionByIndex(T3GFXVertexState));

    return 0;
}

static int T3OcclusionMeshBatchRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    (void)flags;
    node->dataSize = sizeof(uint32_t) * 3;
    node->dynamicBuffer = malloc(node->dataSize);
    fread(node->dynamicBuffer, node->dataSize, 1, stream);

    return 0;
}

int DCArray_T3OcclusionMeshBatch_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
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
        T3OcclusionMeshBatchRead(stream, currentNode, flags);
    }
    return 0;
}

int T3OcclusionMeshDataRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "mData", .metaClassDescriptionIndex = BinaryBuffer},
        {.isBlocked = 0, .memberName = "mBoundingBox", .metaClassDescriptionIndex = BoundingBox},
        {.isBlocked = 1, .memberName = "mBoundingSphere", .metaClassDescriptionIndex = Sphere},
        {.isBlocked = 1, .memberName = "mBatches", .metaClassDescriptionIndex = DCArray_T3OcclusionMeshBatch_},
        {.isBlocked = 0, .memberName = "mVertexCount", .metaClassDescriptionIndex = unsignedlong},
    };
    return genericRead(stream, node, flags, 5, descriptions);
}

int DCArray_HandleBase_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(HandleBase));
}

int D3DMeshRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "mName", .metaClassDescriptionIndex = String},
        {.isBlocked = 0, .memberName = "mVersion", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 0, .memberName = "mToolProps", .metaClassDescriptionIndex = ToolProps},
        {.isBlocked = 0, .memberName = "mLightmapGlobalScale", .metaClassDescriptionIndex = float_type},
        {.isBlocked = 0, .memberName = "mLightmapTexCoordVersion", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 0, .memberName = "mLODParamCRC", .metaClassDescriptionIndex = unsigned__int64},
    };
    genericRead(stream, node, flags, 6, descriptions);

    struct TreeNode *currentNode = node->child->sibling->sibling->sibling->sibling->sibling;

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;

    currentNode->child = malloc(sizeof(struct TreeNode));
    currentNode->child->description = getMetaClassDescriptionByIndex(int_type);
    currentNode->child->description->read(stream, currentNode->child, flags);
    currentNode->child->parent = currentNode;
    currentNode->child->serializeType = 0;
    currentNode->child->memberName = "pairCount";
    currentNode->child->isBlocked = 0;
    currentNode->child->sibling = NULL;

    struct TreeNode *mapNode = currentNode->child;

    for (uint32_t i = 0; i < *(uint32_t *)(currentNode->child->staticBuffer) * 2; ++i)
    {
        mapNode->sibling = calloc(1, sizeof(struct TreeNode));
        mapNode = mapNode->sibling;
        mapNode->parent = node;
        if (i % 2) // if odd
        {
            uint64_t typeSymbol;
            fread(&typeSymbol, sizeof(typeSymbol), 1, stream);
            mapNode->serializeType = 1;
            cfseek(stream, sizeof(uint32_t), SEEK_CUR);
            mapNode->isBlocked = 1;
            mapNode->description = getMetaClassDescriptionBySymbol(typeSymbol);
            mapNode->description->read(stream, mapNode, flags);
        }
        else
        {
            mapNode->description = getMetaClassDescriptionByIndex(Symbol);
            mapNode->description->read(stream, mapNode, flags);
        }
    }

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;
    currentNode->memberName = "unknown";
    currentNode->description = getMetaClassDescriptionByIndex(int_type);
    currentNode->description->read(stream, currentNode, flags);

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;
    currentNode->memberName = "isOcclusionMeshData";
    currentNode->description = getMetaClassDescriptionByIndex(bool_type);
    currentNode->description->read(stream, currentNode, flags);

    if (*(currentNode->staticBuffer) == '1')
    {
        currentNode->sibling = calloc(1, sizeof(struct TreeNode));
        currentNode->sibling->parent = currentNode->parent;
        currentNode = currentNode->sibling;
        cfseek(stream, sizeof(uint32_t), SEEK_CUR);
        currentNode->isBlocked = 1;
        currentNode->description = getMetaClassDescriptionByIndex(T3OcclusionMeshData);
        currentNode->description->read(stream, currentNode, flags);
    }
    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    currentNode->isBlocked = 1;
    currentNode->description = getMetaClassDescriptionByIndex(T3MeshData);
    currentNode->description->read(stream, currentNode, flags);

    // node->children[node->childCount - 1]->description; TODO: Set description
    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;
    int64_t startOfAsync = cftell(stream);
    cfseek(stream, 0, SEEK_END);
    currentNode->dataSize = cftell(stream) - startOfAsync;
    currentNode->dynamicBuffer = malloc(currentNode->dataSize);
    cfseek(stream, startOfAsync, SEEK_SET);
    fread(currentNode->dynamicBuffer, currentNode->dataSize, 1, stream);

    return 0;
}

int T3MaterialRuntimePropertyRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 0, .memberName = "mName", .metaClassDescriptionIndex = Symbol},
        {.isBlocked = 0, .memberName = "mRuntimeName", .metaClassDescriptionIndex = Symbol},
    };
    return genericRead(stream, node, flags, 2, descriptions);
}

int DCArray_T3MaterialRuntimeProperty_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(T3MaterialRuntimeProperty));
}

int T3MaterialParameterRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    (void)flags;
    node->dataSize = sizeof(uint64_t) + 6 * sizeof(int32_t); // TODO: Maybe change to children
    node->dynamicBuffer = malloc(node->dataSize);
    fread(node->dynamicBuffer, node->dataSize, 1, stream);

    return 0;
}

int DCArray_T3MaterialParameter_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(T3MaterialParameter));
}

int T3MaterialTextureRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 0, .memberName = "mName", .metaClassDescriptionIndex = Symbol},
        {.isBlocked = 0, .memberName = "mTextureName", .metaClassDescriptionIndex = Symbol},
        {.isBlocked = 0, .memberName = "mTextureNameWithoutExtension", .metaClassDescriptionIndex = Symbol},
        {.isBlocked = 0, .memberName = "mLayout", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 0, .memberName = "mPropertyType", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 1, .memberName = "mTextureTypes", .metaClassDescriptionIndex = BitSetBase_1_},
        {.isBlocked = 0, .memberName = "mFirstParamIndex", .metaClassDescriptionIndex = unsignedlong},
        {.isBlocked = 0, .memberName = "mParamCount", .metaClassDescriptionIndex = unsignedlong},
        {.isBlocked = 0, .memberName = "mTextureIndex", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 0, .memberName = "mNestedMaterialIndex", .metaClassDescriptionIndex = long_type},
    };
    return genericRead(stream, node, flags, 10, descriptions);
}

int DCArray_T3MaterialTexture_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(T3MaterialTexture));
}

int T3MaterialTransform2DRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    (void)flags;
    node->dataSize = sizeof(uint64_t) + 6 * sizeof(int32_t); // TODO: Maybe change to children
    node->dynamicBuffer = malloc(node->dataSize);
    fread(node->dynamicBuffer, node->dataSize, 1, stream);

    return 0;
}

int DCArray_T3MaterialTransform2D_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(T3MaterialTransform2D));
}

static int T3MaterialNestedMaterialRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "mhMaterial", .metaClassDescriptionIndex = Handle_PropertySet_},
    };
    return genericRead(stream, node, flags, 1, descriptions);
}

int DCArray_T3MaterialNestedMaterial_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
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
        T3MaterialNestedMaterialRead(stream, currentNode, flags); // TODO: Set description
    }
    return 0;
}

int T3MaterialPreShaderRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    (void)flags;
    node->dataSize = 4 * sizeof(int32_t); // TODO: Maybe change to children
    node->dynamicBuffer = malloc(node->dataSize);
    fread(node->dynamicBuffer, node->dataSize, 1, stream);

    return 0;
}

int DCArray_T3MaterialPreShader_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(T3MaterialPreShader));
}

int T3MaterialStaticParameterRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    (void)flags;
    node->dataSize = sizeof(uint64_t) + sizeof(int32_t); // TODO: Maybe change to children
    node->dynamicBuffer = malloc(node->dataSize);
    fread(node->dynamicBuffer, node->dataSize, 1, stream);

    return 0;
}

int DCArray_T3MaterialStaticParameter_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(T3MaterialStaticParameter));
}

static int T3MaterialTextureParamRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    (void)flags;
    node->dataSize = 4 * sizeof(int32_t); // TODO: Maybe change to children
    node->dynamicBuffer = malloc(node->dataSize);
    fread(node->dynamicBuffer, node->dataSize, 1, stream);

    return 0;
}

int DCArray_T3MaterialTextureParam_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
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
        T3MaterialTextureParamRead(stream, currentNode, flags); // TODO: Set description
    }
    return 0;
}

int T3MaterialPassDataRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    (void)flags;
    node->dataSize = 2 * sizeof(int32_t) + sizeof(uint64_t); // TODO: Maybe change to children
    node->dynamicBuffer = malloc(node->dataSize);
    fread(node->dynamicBuffer, node->dataSize, 1, stream);

    return 0;
}

int DCArray_T3MaterialPassData_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(T3MaterialPassData));
}

int T3MaterialCompiledDataRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "mParameters", .metaClassDescriptionIndex = DCArray_T3MaterialParameter_},
        {.isBlocked = 1, .memberName = "mTextures", .metaClassDescriptionIndex = DCArray_T3MaterialTexture_},
        {.isBlocked = 1, .memberName = "mTransforms", .metaClassDescriptionIndex = DCArray_T3MaterialTransform2D_},
        {.isBlocked = 1, .memberName = "mNestedMaterials", .metaClassDescriptionIndex = DCArray_T3MaterialNestedMaterial_},
        {.isBlocked = 1, .memberName = "mPreShaders", .metaClassDescriptionIndex = DCArray_T3MaterialPreShader_},
        {.isBlocked = 1, .memberName = "mStaticParameters", .metaClassDescriptionIndex = DCArray_T3MaterialStaticParameter_},
        {.isBlocked = 1, .memberName = "mTextureParams", .metaClassDescriptionIndex = DCArray_T3MaterialTextureParam_},
        {.isBlocked = 1, .memberName = "mPasses", .metaClassDescriptionIndex = DCArray_T3MaterialPassData_},
        {.isBlocked = 0, .memberName = "mMaterialQuality", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 1, .memberName = "mMaterialBlendModes", .metaClassDescriptionIndex = BitSetBase_1_},
        {.isBlocked = 1, .memberName = "mMaterialPasses", .metaClassDescriptionIndex = BitSetBase_1_},
        {.isBlocked = 1, .memberName = "mMaterialChannels", .metaClassDescriptionIndex = BitSetBase_2_},
        {.isBlocked = 1, .memberName = "mShaderInputs", .metaClassDescriptionIndex = BitSetBase_3_},
        {.isBlocked = 1, .memberName = "mSceneTextures", .metaClassDescriptionIndex = BitSetBase_1_},
        {.isBlocked = 1, .memberName = "mOptionalPropertyTypes", .metaClassDescriptionIndex = BitSetBase_1_},
        {.isBlocked = 1, .memberName = "mPreShaderBuffer", .metaClassDescriptionIndex = BinaryBuffer},
        {.isBlocked = 0, .memberName = "mFlags", .metaClassDescriptionIndex = Flags},
        {.isBlocked = 0, .memberName = "mParameterBufferScalarSize[0]", .metaClassDescriptionIndex = unsignedlong},
        {.isBlocked = 0, .memberName = "mParameterBufferScalarSize[1]", .metaClassDescriptionIndex = unsignedlong},
        {.isBlocked = 0, .memberName = "mPreShaderParameterBufferScalar", .metaClassDescriptionIndex = unsignedlong},
    };
    return genericRead(stream, node, flags, 20, descriptions);
}

int DCArray_T3MaterialCompiledData_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(T3MaterialCompiledData));
}

int T3MaterialDataRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 0, .memberName = "mMaterialName", .metaClassDescriptionIndex = Symbol},
        {.isBlocked = 0, .memberName = "mRuntimePropertiesName", .metaClassDescriptionIndex = Symbol},
        {.isBlocked = 0, .memberName = "mLegacyRenderTextureProperty", .metaClassDescriptionIndex = Symbol},
        {.isBlocked = 0, .memberName = "mLegacyBlendModeRuntimeProperty", .metaClassDescriptionIndex = Symbol},
        {.isBlocked = 0, .memberName = "mDomain", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 1, .memberName = "mRuntimeProperties", .metaClassDescriptionIndex = DCArray_T3MaterialRuntimeProperty_},
        {.isBlocked = 0, .memberName = "mFlags", .metaClassDescriptionIndex = Flags},
        {.isBlocked = 0, .memberName = "mRuntimeFlags", .metaClassDescriptionIndex = Flags},
        {.isBlocked = 0, .memberName = "mVersion", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 0, .memberName = "mCompiledData2", .metaClassDescriptionIndex = DCArray_T3MaterialCompiledData_}, // isBlocked is set to 0 intentionally
    };
    return genericRead(stream, node, flags, 10, descriptions);
}
