#include <ttstring.h>
#include <types.h>

/*
sk62_clementine_head.d3dmesh (S3E1)
0x158 is start
0x4C2D is end of T3MeshData

*/
struct BoundingBox
{
    struct Vector3 min;
    struct Vector3 max;
};

struct Sphere
{
    struct Vector3 center;
    float radius;
};

struct T3MeshBatch
{
    /// <summary>
    /// [24 bytes] The bounding box defined for the mesh.
    /// </summary>
    struct BoundingBox mBoundingBox;

    /// <summary>
    /// [20 bytes] The bounding sphere defined for the mesh.
    /// </summary>
    struct Sphere mBoundingSphere;

    /// <summary>
    /// [4 bytes]
    /// </summary>
    unsigned int mBatchUsage_BlockSize;

    /// <summary>
    /// [4 bytes] mFlags
    /// </summary>
    unsigned int mBatchUsage;

    /// <summary>
    /// [4 bytes]
    /// </summary>
    unsigned int mMinVertIndex;

    /// <summary>
    /// [4 bytes] (Seems to always be 0?)
    /// </summary>
    unsigned int mMaxVertIndex;

    /// <summary>
    /// [4 bytes]
    /// <para> This starts at 0, but progressively towards the end with more batch elements, this gets incremented by mNumPrimitives. </para>
    /// <para> NOTE: This seems to be related with the index buffer. </para>
    /// </summary>
    unsigned int mBaseIndex;

    /// <summary>
    /// [4 bytes]
    /// <para> NOTE 1: This seems to be related with the index buffer. </para>
    /// <para> NOTE 2: error.mdl noted that this aligns with (mNumPrimitives / 3). </para>
    /// </summary>
    unsigned int mStartIndex;

    /// <summary>
    /// [4 bytes] The number of triangles in the batch mesh.
    /// <para> NOTE 2: this seems to be (mStartIndex * 3). </para>
    /// </summary>
    unsigned int mNumPrimitives;

    /// <summary>
    /// [4 bytes] (Seems to always be 8?)
    /// <para> NOTE 1: error.mdl noted that this might actually be a flag field. Where 8 is 1<<3, triangles = 3 triangles </para>
    /// </summary>
    unsigned int mNumIndices;

    /// <summary>
    /// [4 bytes] Unsure what this is for... but it's value seems to always be 4294967295
    /// </summary>
    unsigned int mTextureIndices;

    /// <summary>
    /// [4 bytes]
    /// </summary>
    unsigned int mMaterialIndex;

    /// <summary>
    /// [4 bytes]
    /// </summary>
    unsigned int mAdjacencyStartIndex;
};

struct T3MeshLOD
{
    uint32_t meshBatchCount;
    struct T3MeshBatch *batches[2];
};

struct T3MaterialRequirements
{
    struct BitSetBase_1_ passes;
    struct BitSetBase_2_ channels; // Fix
    struct BitSetBase_3_ inputs;
};

struct T3MeshEffectPreloadDynamicFeatures
{
    struct BitSetBase_1_ dynamicFeatures;
    int32_t priority;
};

struct T3MeshEffectPreloadEntry
{
    uint32_t effectType;
    struct BitSetBase_3_ staticEffectFeatures;
    uint64_t materialCRC;
    uint32_t dynamicEffectFeaturesCount;
    struct T3MeshEffectPreloadDynamicFeatures *dynamicEffectFeatures;
};

struct T3MeshEffectPreload
{
    int32_t effectQuality;
    uint32_t entriesCount;
    struct T3MeshEffectPreloadEntry *entries;
    uint32_t totalEffectCount;
};

struct T3MeshTextureIndices
{
    struct IndexIntPair
    {
        int32_t index;
        int32_t val;
    } *pairs;
};

struct T3MeshBatch
{
    struct BoundingBox boundingBox;
    struct Sphere boundingSphere;
    uint32_t flags;
    uint32_t minVertIndex;
    uint32_t maxVertIndex;
    uint32_t baseIndex;
    uint32_t startIndex;
    uint32_t numPrimitives;
    uint32_t numIndices;
    struct T3MeshTextureIndices textureIndices;
    int32_t materialIndex;
    uint32_t adjacencyStartIndex;
};

struct T3MeshLOD
{
    uint32_t batches_0_count;
    struct T3MeshBatch *batches_0_;
    uint32_t batches_1_count;
    struct T3MeshBatch *batches_1_;
    struct BitSetBase_1_ vertexStreams;
    struct BoundingBox boundingBox;
    struct Sphere boundingSphere;
    uint32_t flags;
    uint32_t vertexStateIndex;
    uint32_t numPrimitives;
    uint32_t numBatches;
    uint32_t vertexStart;
    uint32_t vertexCount;
    uint32_t textureAtlasWidth;
    uint32_t textureAtlasHeight;
    float pixelSize;
    float distance;
    uint32_t bonesSymbolsCount;
    uint64_t *bonesSymbols;
};

struct T3MeshTexture
{
    int32_t textureType;
    uint64_t textureHandle;
    uint64_t nameSymbol;
    struct BoundingBox boundingBox;
    struct Sphere boundingSphere;
    float maxObjAreaPerUVArea;
    float averageObjAreaPerUVArea;
};

struct T3MeshMaterial
{
    uint64_t mateiral;                    // Handle<PropertySet>
    uint64_t baseMaterialName;            // Symbol
    uint64_t legacyRenderTextureProperty; // Symbol
    struct BoundingBox boundingBox;
    struct Sphere boundingSphere;
    uint32_t flags;
};

struct T3MeshMaterialOverride
{
    uint64_t overrrideMaterial; // Handle<PropertySet>
    uint32_t materialIndex;
};

struct T3MeshBoneEntry
{
    uint64_t nameSymbol;
    struct BoundingBox boundingBox;
    struct Sphere boundingSphere;
    int32_t numVerts;
};

struct T3MeshLocalTransformEntry
{
    struct Transform transform;
    int32_t cameraFacingType;
};

struct T3MeshTexCoordTransform
{
    struct Vector2 scale;
    struct Vector2 offset;
};

struct T3MeshTexCoordTransformIndexPair
{
    uint32_t index;
    struct T3MeshTexCoordTransform transform;
};

struct BinaryBuffer
{
    uint32_t dataSize;
    uint8_t *data;
};

struct T3MeshCPUSkinningData
{
    int32_t positionFormat;
    int32_t weightFormat;
    int32_t normalFormat;
    struct BitSetBase_1_ vertexStreams;
    uint32_t normalCount;
    uint32_t weightOffset;
    uint32_t vertexSize;
    uint32_t weightSize;
    struct BinaryBuffer data;
};

struct GFXPlatformAttributeParams
{
    int32_t attribute;
    int32_t format;
    uint32_t attributeIndex;
    uint32_t bufferIndex;
    uint32_t bufferOffset;
};

struct T3GFXBuffer
{
    int32_t resourceUsage;
    int32_t bufferFormat;
    uint32_t bufferUsage;
    uint32_t count;
    uint32_t stride;
};

struct T3GFXVertexState
{
    uint32_t vertexCountPerInstance;
    uint32_t indexBufferCount;
    uint32_t vertexBufferCount;
    uint32_t attributeCount;
    struct GFXPlatformAttributesParams *attributes;
    struct T3GFXBuffer *indexBuffer;
    struct T3GFXBuffer *vertexBuffer;
};

struct T3MeshData
{
    uint32_t lodsCount;
    struct T3MeshLOD *lods;
    uint32_t texturesCount;
    struct T3MeshTexture *textures;
    uint32_t materialsCount;
    struct T3MeshMaterial *materials;
    uint32_t materialOverridesCount;
    struct T3MeshMaterialOverride *materialOverrides;
    uint32_t bonesCount;
    struct T3MeshBoneEntry *bones;
    uint32_t localTransformsCount;
    struct T3MeshLocalTransformEntry *localTransforms;
    struct T3MaterialRequirements materialRequirements;
    struct BitSetBase_1_ vertexStreams;
    struct BoundingBox boundingBox;
    struct Sphere boundingSphere;
    int32_t endianType;
    struct Vector3 positionScale;
    struct Vector3 positionWScale;
    struct Vector3 positionOffset;
    float lightmapTexelAreaPerSurfaceAre;
    uint64_t propertyKeyBaseSymbol;
    uint32_t vertexCount;
    uint32_t flags;
    uint32_t meshPreLoadCount;
    struct T3MeshEffectPreload *meshPreload;

    uint32_t texCoordTransformCount;
    struct T3MeshTexCoordTransformIndexPair mTexCoordTransform[4];
    struct T3MeshCPUSkinningData CPUSkinningData;
    uint32_t vertexStatesCount;
    struct T3GFXVertexState *vertexStates;
};

struct T3OcclusionMeshBatch
{
    uint32_t flags;
    uint32_t startIndex;
    uint32_t numTriangles;
};

struct T3OcclusionMeshData
{
    struct BinaryBuffer data;
    struct BoundingBox boundingBox;
    struct Sphere boundingSphere;
    void *batches; // DCArray<T3OcclusionMeshBatch>
    uint32_t vertexCount;
};

struct T3MaterialRuntimeProperty
{
    uint64_t nameSymbol;
    uint64_t runTimeNameSymbol;
};

struct T3MaterialParameter
{
    uint64_t nameSymbol;
    int32_t propertyType;
    int32_t valueType;
    uint32_t flags; // unsigned_long
    int32_t scalarOffset;
    int32_t preShaderScalarOffset;
    int32_t nestedMaterialIndex;
};

struct T3MaterialTexture
{
    uint64_t nameSymbol;
    uint64_t textureNameSymbol;
    uint64_t textureNameWithoutExtensionSymbol;
    int32_t layout;
    int32_t propertyType;
    struct BitSetBase_1_ textureTypes;
    uint32_t firstParamIndex;
    uint32_t paramCount;
    int32_t textureIndex;
    int32_t nestedMaterialIndex;
};

struct T3MaterialTransform2D
{
    uint64_t parameterPrefixSymbol;
    uint32_t flags; // unsigned_long
    int32_t scalarOffset0;
    int32_t scalarOffset1;
    int32_t preShaderScalarOffset0;
    int32_t preShaderScalarOffset1;
    int32_t nestedMaterialIndex;
};

struct T3MaterialNestedMaterial
{
    uint64_t material; // Handle<PropertySet>
};

struct T3MaterialPreShader
{
    int32_t valueType;
    uint32_t flags;
    int32_t preShaderOffset;
    int32_t scalarParameterOffset;
};

struct T3MaterialStaticParameter
{
    uint64_t nameSymbol;
    int32_t nestedMaterialIndex;
};

struct T3MaterialTextureParam
{
    int32_t paramType;
    int32_t valueType;
    uint32_t flags; // unsigned_long
    int32_t scalarOffset;
};

struct T3MaterialPassData
{
    int32_t passType;
    int32_t blendMode;
    uint64_t materialCrc;
};

struct T3MaterialCompiledData
{
    uint32_t parametersCount;
    struct T3MaterialParameter *parameters;
    uint32_t texturesCount;
    struct T3MaterialTexture *textures;
    uint32_t transformsCount;
    struct T3MaterialTransform2D *transforms;
    uint32_t nestedMaterialsCount;
    struct T3MaterialNestedMaterial *nestedMaterials;
    uint32_t preShadersCount;
    struct T3MaterialPreShader *preShaders;
    uint32_t staticParametersCount;
    struct T3MaterialStaticParameter *staticParameters;
    uint32_t textureParamsCount;
    struct T3MaterialTextureParam *textureParams;
    uint32_t passesCount;
    struct T3MaterialPassData *passes;
    int32_t materialQuality;
    struct BitSetBase_1_ materialBlendModes;
    struct BitSetBase_1_ materialPasses;
    void *materialChannels; // BitSet<enum_T3MaterialChannelType,46,0>
    struct BitSetBase_3_ shaderInputs;
    struct BitSetBase_1_ sceneTextures;
    struct BitSetBase_1_ optionalPropertyTypes;
    struct BinaryBuffer preShaderBuffer;
    uint32_t flags;
    uint32_t parameterBufferScalarSize[2];
    uint32_t preShaderParameterBufferScalar;
};

struct T3MaterialData
{
    uint64_t materialNameSymbol;
    uint64_t runTimePropertiesNameSymbol;
    uint64_t legacyRenderTexturePropertySymbol;
    uint64_t legacyBlendModeRuntimePropertySymbol;
    int32_t domain;
    uint32_t runTimeProperitesCount;
    struct T3MaterialRuntimeProperty *runTimeProperties;
    uint32_t flags;
    uint32_t runTimeFlags;
    int32_t version;
    int32_t compiledData2Count;
    struct T3MaterialCompiledData *compiledData2;
};

struct D3DMesh
{
    struct String name;
    uint32_t version;
    struct T3MeshData data;
    uint32_t internalResourceCount;
    struct HandleBase *internalResources;
    uint8_t toolProps;
    float lightmapGlobalScale;
    int32_t lightmapTexCoordVersion;
    uint64_t LODParamCRC;

    int32_t mapSizes;
    void *symboli64HandleBaseMap; // handle is blocked
    int32_t a;
    uint8_t isOcclusionMeshData;
    struct T3OcclusionMeshData occlusionMeshData; // blocked
    struct T3MeshData meshData;                   // blocked
};