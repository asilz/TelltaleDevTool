#include <inttypes.h>

struct HandleLock
{
    struct HandleBase base;
};

struct MeshSceneLightMapData__Entry
{
    uint64_t meshNameSymbol;
    uint32_t LODIndex;
    int32_t lightQuality;
    struct Vector2 textureScale;
    struct Vector2 textureOffset;
    uint32_t texturePage;
};

struct MeshSceneLightMapData
{
    uint32_t entriesBlock;
    uint32_t entriesCount;
    struct MeshSceneLightMapData__Entry *entries;
    uint32_t stationaryLightIndicesBlock;
    uint32_t stationaryLightIndicesCount;
    uint16_t *stationaryLightIndices;
    uint32_t flags;
};

struct SoundFootSteps__EnumMaterial
{
    int32_t val; // default = 1, carpet, concrete, concrete_(Wet), grass, grass_(tall), gravel, leaves, linoleum, metal_(Thick), metal_(Thin), mud, puddle, sand, snow, tile_hard, wood
};

struct FileNameBase
{
    uint64_t fileNameSymbol;
};

struct FileName_SoundEventBankDummy_
{
    uint32_t baseBlock;
    struct FileNameBase base;
};

struct SoundEventNameBase
{
    uint64_t eventGuidSymbol;
    uint64_t eventDisplayName;
};

struct SoundEventName_0_
{
    struct SoundEventNameBase base;
};

struct FootSteps__FootstepBank
{
    uint32_t soundsBlock;
    uint32_t soundsCount;
    uint64_t *sounds;
};

struct FootSteps2__FootstepBank
{
    struct SoundEventName_0_ eventName;
    void *materialMap; // Map<SoundFootsteps::EnumMaterial,SoundEventName<0>,std::less<S oundFootsteps::EnumMaterial>_>
};

struct EnlightenModule__EnlightenPrimitiveSettings
{
    uint32_t systemIDBlock;
    struct String systemID;
    uint32_t enlightenLightingMode; // blocked
    uint32_t enlightenUpdateMethod; // blocked
    uint32_t enlightenQuality;      // blocked
};

struct EnlightenModule__EnlightenSystemSettings
{
    uint32_t defaultQuality;          // blocked
    uint32_t adaptiveProbeResolution; // blocked
    uint8_t disableEnlighten;
};

struct LocationInfo
{
    uint32_t attachmentAgentBlock;
    struct String attachmentAgent;
    uint64_t attachmentNodeSymbol;
    uint32_t initialLocalTransformBlock;
    struct Transform initialLocalTransform;
};

struct T3LightEnvInternalData__QualityEntry
{
    uint32_t shadowLayer;
    uint32_t flags;
};

struct Vector4
{
    float x;
    float y;
    float z;
    float w;
};

struct MeshSceneEnlightenData
{
    uint64_t systemNameSymbol;
    struct Vector4 UVTransform;
    uint32_t flags;
};

struct T3LightEnvInternalData
{
    struct T3LightEnvInternalData__QualityEntry entryForQuality[4];
    int32_t stationaryLightIndex;
};

struct T3LightSceneInternalData__LightmapPage
{
    uint32_t textureAtlasBlock;
    uint64_t textureAtlas; // Handle<T3Texture>
    uint32_t flags;
};

struct T3LightSceneInternalData__QualityEntry
{
    uint32_t lightMapPagesBlock;
    uint32_t lightMapPagesCount;
    struct T3LightSceneInternalData__LightmapPage *lightMapPages;
    uint32_t staticShadowVolumeTextureBlock;
    uint64_t staticShadowVolumeTexture // Handle<T3Texture>
};

struct T3LightSceneInternalData
{
    struct T3LightSceneInternalData__QualityEntry entryForQuality[4]; // Blocked
    uint32_t stationaryLightCount;
    uint32_t bakeVersion;
};

struct AgentInfo
{
    uint32_t agentNameBlock;
    struct String agentName;
    uint32_t agentScenePropsBlock;
    struct PropertySet agentSceneProps;
};

struct Scene
{
    float timeScale; // not serialized?
    uint8_t active;  // not serialized?
    uint8_t hidden;
    struct String name;
    uint32_t referencedScenesBlock;
    uint32_t referencedScenesCount;
    struct HandleLock *referencedScenes;
    uint32_t agentListBlock;
    uint32_t agentListCount;
    struct AgentInfo agentList;
};