#include <meta.h>
#include <types.h>
#include <stream.h>
#include <container.h>
#include <stdlib.h>
#include <intrinsic.h>

int DCArray_HandleLock_Scene__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(HandleLock_Scene_));
}

int LinkedList_Scene__AgentInfo0_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(Scene__AgentInfo));
}

int Map_SymbolSymbolless_Symbol__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericMapRead(stream, node, flags, getMetaClassDescriptionByIndex(Symbol), getMetaClassDescriptionByIndex(Symbol));
}

int Map_SoundFootsteps__EnumMaterialSoundEventName_0_less_SoundFootsteps__EnumMaterial__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericMapRead(stream, node, flags, getMetaClassDescriptionByIndex(long_type), getMetaClassDescriptionByIndex(SoundEventName_0_));
}

int Map_SymbolFootsteps2__FootstepBankless_Symbol__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericMapRead(stream, node, flags, getMetaClassDescriptionByIndex(Symbol), getMetaClassDescriptionByIndex(Footsteps2__FootstepBank));
}

int Map_SoundFootsteps__EnumMaterialDCArray_Handle_SoundData__less_SoundFootsteps__EnumMaterial__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericMapRead(stream, node, flags, getMetaClassDescriptionByIndex(long_type), getMetaClassDescriptionByIndex(DCArray_Handle_SoundData__));
}

int Map_SymbolFootSteps__FootstepBankless_Symbol__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericMapRead(stream, node, flags, getMetaClassDescriptionByIndex(Symbol), getMetaClassDescriptionByIndex(FootSteps__FootstepBank));
}

int DCArray_Vector2_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(Vector2));
}

int DCArray_Handle_SoundData__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(Handle_SoundData_));
}

int Vector2Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    (void)flags;
    node->dataSize = 2 * sizeof(float);
    fread(node->staticBuffer, node->dataSize, 1, stream);

    return 0;
}

int Set_FileName_SoundEventBankDummy_less_FileName_SoundEventBankDummy___Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(FileName_SoundEventBankDummy_));
}

int FileName_SoundEventBankDummy_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_FileNameBass", .metaClassDescriptionIndex = FileNameBase},
    };
    return genericRead(stream, node, flags, 1, descriptions);
}

int EnlightenModule__EnlightenPrimitiveSettingsRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "mSystemId", .metaClassDescriptionIndex = String},
        {.isBlocked = 1, .memberName = "mEnlightenLightingMode", .metaClassDescriptionIndex = EnlightenModule__EnumeInstanceType},
        {.isBlocked = 1, .memberName = "mEnlightenUpdateMethod", .metaClassDescriptionIndex = EnlightenModule__EnumeUpdateMethod},
        {.isBlocked = 1, .memberName = "mEnlightenQuality", .metaClassDescriptionIndex = EnlightenModule__EnumeQualityWithDefault},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int EnlightenModule__EnlightenSystemSettingsRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "mDefaultQuality", .metaClassDescriptionIndex = EnlightenModule__EnumeQuality},
        {.isBlocked = 1, .memberName = "mAdaptiveProbeResolution", .metaClassDescriptionIndex = EnlightenModule__EnumeProbeResolution},
        {.isBlocked = 0, .memberName = "mDisableEnlighten", .metaClassDescriptionIndex = bool_type},
    };
    return genericRead(stream, node, flags, 3, descriptions);
}

int MeshSceneEnlightenDataRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 0, .memberName = "mSystemName", .metaClassDescriptionIndex = Symbol},
        {.isBlocked = 0, .memberName = "mUVTransform", .metaClassDescriptionIndex = Vector4},
        {.isBlocked = 0, .memberName = "mFlags", .metaClassDescriptionIndex = Flags},
    };
    return genericRead(stream, node, flags, 3, descriptions);
}

static int MeshSceneLightmapData__EntryRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 0, .memberName = "mMeshName", .metaClassDescriptionIndex = Symbol},
        {.isBlocked = 0, .memberName = "mLODIndex", .metaClassDescriptionIndex = unsignedlong},
        {.isBlocked = 0, .memberName = "mLightQuality", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 0, .memberName = "mTextureScale", .metaClassDescriptionIndex = Vector2},
        {.isBlocked = 0, .memberName = "mTextureOffset", .metaClassDescriptionIndex = Vector2},
        {.isBlocked = 0, .memberName = "mTexturePage", .metaClassDescriptionIndex = unsignedlong},
    };
    return genericRead(stream, node, flags, 6, descriptions);
}

int DCArray_unsignedshort_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(unsignedshort));
}

int DCArray_float_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(float_type));
}

int DCArray_MeshSceneLightmapData__Entry_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
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
        MeshSceneLightmapData__EntryRead(stream, currentNode, flags);
    }
    return 0;
}

int MeshSceneLightmapDataRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "mEntries", .metaClassDescriptionIndex = DCArray_MeshSceneLightmapData__Entry_},
        {.isBlocked = 1, .memberName = "mStationaryLightIndices", .metaClassDescriptionIndex = DCArray_unsignedshort_},
        {.isBlocked = 0, .memberName = "mFlags", .metaClassDescriptionIndex = Flags},
    };
    return genericRead(stream, node, flags, 3, descriptions);
}

int SoundEventNameBaseRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 0, .memberName = "mEventGuid", .metaClassDescriptionIndex = Symbol},
        {.isBlocked = 0, .memberName = "mEventDisplayName", .metaClassDescriptionIndex = Symbol},
    };
    return genericRead(stream, node, flags, 2, descriptions);
}

int SoundEventNameRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_SoundEventNameBase", .metaClassDescriptionIndex = SoundEventNameBase},
    };
    return genericRead(stream, node, flags, 1, descriptions);
}

int Footsteps__FootstepBankRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "mSounds", .metaClassDescriptionIndex = DCArray_Handle_SoundData__},
        {.isBlocked = 1, .memberName = "mMaterialFootsteps", .metaClassDescriptionIndex = Map_SoundFootsteps__EnumMaterialDCArray_Handle_SoundData__less_SoundFootsteps__EnumMaterial__},
    };
    return genericRead(stream, node, flags, 2, descriptions);
}

int Footsteps2__FootstepBankRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "mEventName", .metaClassDescriptionIndex = SoundEventName_0_},
        {.isBlocked = 1, .memberName = "mMaterialMap", .metaClassDescriptionIndex = Map_SoundFootsteps__EnumMaterialSoundEventName_0_less_SoundFootsteps__EnumMaterial__},
    };
    return genericRead(stream, node, flags, 2, descriptions);
}

static int T3LightSceneInternalData__LightmapPageRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "mhTextureAtlas", .metaClassDescriptionIndex = Handle_T3Texture_},
        {.isBlocked = 0, .memberName = "mFlags", .metaClassDescriptionIndex = Flags},
    };
    return genericRead(stream, node, flags, 2, descriptions);
}

int DCArray_Symbol_Read(FILE *stream, struct TreeNode *node, uint32_t flags) // TODO: Move to different file
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(Symbol));
}

int DCArray_Handle_D3DMesh__Read(FILE *stream, struct TreeNode *node, uint32_t flags) // TODO: Move to different file
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(Handle_D3DMesh_));
}

int T3LightEnvInternalData__QualityEntryRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    (void)flags;
    node->dataSize = 2 * sizeof(uint32_t);
    fread(node->staticBuffer, node->dataSize, 1, stream); // TODO: Change to children?

    return 0;
}

int T3LightEnvInternalDataRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "mEntryForQuality[0]", .metaClassDescriptionIndex = T3LightEnvInternalData__QualityEntry},
        {.isBlocked = 1, .memberName = "mEntryForQuality[1]", .metaClassDescriptionIndex = T3LightEnvInternalData__QualityEntry},
        {.isBlocked = 1, .memberName = "mEntryForQuality[2]", .metaClassDescriptionIndex = T3LightEnvInternalData__QualityEntry},
        {.isBlocked = 1, .memberName = "mEntryForQuality[3]", .metaClassDescriptionIndex = T3LightEnvInternalData__QualityEntry},
        {.isBlocked = 0, .memberName = "mStationaryLightIndex", .metaClassDescriptionIndex = long_type},
    };
    return genericRead(stream, node, flags, 5, descriptions);
}

int DCArray_T3LightSceneInternalData__LightmapPage_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
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
        T3LightSceneInternalData__LightmapPageRead(stream, currentNode, flags);
    }
    return 0;
}

int T3LightSceneInternalData__QualityEntryRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "mLightmapPages", .metaClassDescriptionIndex = DCArray_T3LightSceneInternalData__LightmapPage_},
        {.isBlocked = 1, .memberName = "mhStaticShadowVolumeTexture", .metaClassDescriptionIndex = Handle_T3Texture_},
    };
    return genericRead(stream, node, flags, 2, descriptions);
}

int T3LightSceneInternalDataRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "mEntryForQuality[0]", .metaClassDescriptionIndex = T3LightSceneInternalData__QualityEntry},
        {.isBlocked = 1, .memberName = "mEntryForQuality[1]", .metaClassDescriptionIndex = T3LightSceneInternalData__QualityEntry},
        {.isBlocked = 1, .memberName = "mEntryForQuality[2]", .metaClassDescriptionIndex = T3LightSceneInternalData__QualityEntry},
        {.isBlocked = 1, .memberName = "mEntryForQuality[3]", .metaClassDescriptionIndex = T3LightSceneInternalData__QualityEntry},
        {.isBlocked = 0, .memberName = "mStationaryLightCount", .metaClassDescriptionIndex = unsignedlong},
        {.isBlocked = 0, .memberName = "mBakeVersion", .metaClassDescriptionIndex = unsignedlong},
    };
    return genericRead(stream, node, flags, 6, descriptions);
}

int LocationInfoRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "mAttachmentAgent", .metaClassDescriptionIndex = String},
        {.isBlocked = 0, .memberName = "mAttachmentNode", .metaClassDescriptionIndex = Symbol},
        {.isBlocked = 1, .memberName = "mInitialLocalTransform", .metaClassDescriptionIndex = Transform},
    };
    return genericRead(stream, node, flags, 3, descriptions);
}

int Scene__AgentInfoRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "mAgentName", .metaClassDescriptionIndex = String},
        {.isBlocked = 1, .memberName = "mAgentSceneProps", .metaClassDescriptionIndex = PropertySet},
    };
    return genericRead(stream, node, flags, 2, descriptions);
}

int SceneRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 0, .memberName = "mbActive", .metaClassDescriptionIndex = bool_type},
        {.isBlocked = 1, .memberName = "mName", .metaClassDescriptionIndex = String},
        {.isBlocked = 1, .memberName = "mReferencedScenes", .metaClassDescriptionIndex = DCArray_HandleLock_Scene__},
        {.isBlocked = 1, .memberName = "mAgentList", .metaClassDescriptionIndex = LinkedList_Scene__AgentInfo0_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}