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
    node->dataSize = 2 * sizeof(float);
    fread(node->data.staticBuffer, node->dataSize, 1, stream);

    return 0;
}

int Set_FileName_SoundEventBankDummy_less_FileName_SoundEventBankDummy___Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(FileName_SoundEventBankDummy_));
}

int FileName_SoundEventBankDummy_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 1;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->isBlocked = 1;
    node->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->parent = node;
    node->children[0]->description = getMetaClassDescriptionByIndex(FileNameBase);
    node->children[0]->description->read(stream, node->children[0], flags);

    return 0;
}

int EnlightenModule__EnlightenPrimitiveSettingsRead(FILE *stream, struct TreeNode *node, uint32_t flags)
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
    node->children[0]->description = getMetaClassDescriptionByIndex(String);
    node->children[0]->description->read(stream, node->children[0], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[1]->isBlocked = 1;
    node->children[1]->description = getMetaClassDescriptionByIndex(EnlightenModule__EnumeInstanceType);
    node->children[1]->description->read(stream, node->children[1], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[2]->isBlocked = 1;
    node->children[2]->description = getMetaClassDescriptionByIndex(EnlightenModule__EnumeUpdateMethod);
    node->children[2]->description->read(stream, node->children[2], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[3]->isBlocked = 1;
    node->children[3]->description = getMetaClassDescriptionByIndex(EnlightenModule__EnumeQualityWithDefault);
    node->children[3]->description->read(stream, node->children[3], flags);

    return 0;
}

int EnlightenModule__EnlightenSystemSettingsRead(FILE *stream, struct TreeNode *node, uint32_t flags)
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
    node->children[0]->description = getMetaClassDescriptionByIndex(EnlightenModule__EnumeQuality);
    node->children[0]->description->read(stream, node->children[0], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[1]->isBlocked = 1;
    node->children[1]->description = getMetaClassDescriptionByIndex(EnlightenModule__EnumeProbeResolution);
    node->children[1]->description->read(stream, node->children[1], flags);

    node->children[2]->description = getMetaClassDescriptionByIndex(bool_type);
    node->children[2]->description->read(stream, node->children[2], flags);

    return 0;
}

int MeshSceneEnlightenDataRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 3;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    node->children[0]->description = getMetaClassDescriptionByIndex(Symbol);
    node->children[0]->description->read(stream, node->children[0], flags);

    node->children[1]->description = getMetaClassDescriptionByIndex(Vector4);
    node->children[1]->description->read(stream, node->children[1], flags);

    node->children[2]->description = getMetaClassDescriptionByIndex(Flags);
    node->children[2]->description->read(stream, node->children[2], flags);

    return 0;
}

static int MeshSceneLightmapData__EntryRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 6;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    node->children[0]->description = getMetaClassDescriptionByIndex(Symbol);
    node->children[0]->description->read(stream, node->children[0], flags);

    node->children[1]->description = getMetaClassDescriptionByIndex(unsignedlong);
    node->children[1]->description->read(stream, node->children[1], flags);

    node->children[2]->description = getMetaClassDescriptionByIndex(long_type);
    node->children[2]->description->read(stream, node->children[2], flags);

    node->children[3]->description = getMetaClassDescriptionByIndex(Vector2);
    node->children[3]->description->read(stream, node->children[3], flags);

    node->children[4]->description = getMetaClassDescriptionByIndex(Vector2);
    node->children[4]->description->read(stream, node->children[4], flags);

    node->children[5]->description = getMetaClassDescriptionByIndex(unsignedlong);
    node->children[5]->description->read(stream, node->children[5], flags);

    return 0;
}

int DCArray_unsignedshort_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(unsignedshort));
}

int DCArray_float_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(DCArray_float_));
}

int DCArray_MeshSceneLightmapData__Entry_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
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
        MeshSceneLightmapData__EntryRead(stream, node->children[i], flags); // TODO: Set description
    }
    return 0;
}

int MeshSceneLightmapDataRead(FILE *stream, struct TreeNode *node, uint32_t flags)
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
    node->children[0]->description = getMetaClassDescriptionByIndex(DCArray_MeshSceneLightmapData__Entry_);
    node->children[0]->description->read(stream, node->children[0], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[1]->isBlocked = 1;
    node->children[1]->description = getMetaClassDescriptionByIndex(DCArray_unsignedshort_);
    node->children[1]->description->read(stream, node->children[1], flags);

    node->children[2]->description = getMetaClassDescriptionByIndex(Flags);
    node->children[2]->description->read(stream, node->children[2], flags);

    return 0;
}

int SoundEventNameBaseRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 2;
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

    return 0;
}

int SoundEventNameRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 1;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->parent = node;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->isBlocked = 1;
    node->children[0]->description = getMetaClassDescriptionByIndex(SoundEventNameBase);
    node->children[0]->description->read(stream, node->children[0], flags);

    return 0;
}

int Footsteps__FootstepBankRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 2;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
        node->children[i]->isBlocked = 1;
    }

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->description = getMetaClassDescriptionByIndex(DCArray_Handle_SoundData__);
    node->children[0]->description->read(stream, node->children[0], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[1]->description = getMetaClassDescriptionByIndex(Map_SoundFootsteps__EnumMaterialDCArray_Handle_SoundData__less_SoundFootsteps__EnumMaterial__);
    node->children[1]->description->read(stream, node->children[1], flags);

    return 0;
}

int Footsteps2__FootstepBankRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 2;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
        node->children[i]->isBlocked = 1;
    }

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->description = getMetaClassDescriptionByIndex(SoundEventName_0_);
    node->children[0]->description->read(stream, node->children[0], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[1]->description = getMetaClassDescriptionByIndex(Map_SoundFootsteps__EnumMaterialSoundEventName_0_less_SoundFootsteps__EnumMaterial__);
    node->children[1]->description->read(stream, node->children[1], flags);

    return 0;
}

static int T3LightSceneInternalData__LightmapPageRead(FILE *stream, struct TreeNode *node, uint32_t flags)
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
    node->children[0]->description = getMetaClassDescriptionByIndex(Handle_T3Texture_);
    node->children[0]->description->read(stream, node->children[0], flags);

    node->children[1]->description = getMetaClassDescriptionByIndex(Flags);
    node->children[1]->description->read(stream, node->children[1], flags);

    return 0;
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
    node->dataSize = 2 * sizeof(uint32_t);
    fread(node->data.staticBuffer, node->dataSize, 1, stream); // TODO: Change to children?

    return 0;
}

int T3LightEnvInternalDataRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 4;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
        cfseek(stream, sizeof(uint32_t), SEEK_CUR);
        node->children[i]->isBlocked = 1;
        node->children[i]->description = getMetaClassDescriptionByIndex(T3LightEnvInternalData__QualityEntry);
        node->children[i]->description->read(stream, node->children[i], flags);
    }

    node->dataSize = sizeof(int32_t);
    fread(node->data.staticBuffer, node->dataSize, 1, stream); // TODO: Change to child?

    return 0;
}

int DCArray_T3LightSceneInternalData__LightmapPage_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
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
        T3LightSceneInternalData__LightmapPageRead(stream, node->children[i], flags); // TODO: Set description
    }
    return 0;
}

int T3LightSceneInternalData__QualityEntryRead(FILE *stream, struct TreeNode *node, uint32_t flags)
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
    node->children[0]->description = getMetaClassDescriptionByIndex(DCArray_T3LightSceneInternalData__LightmapPage_);
    node->children[0]->description->read(stream, node->children[0], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[1]->isBlocked = 1;
    node->children[1]->description = getMetaClassDescriptionByIndex(Handle_T3Texture_);
    node->children[1]->description->read(stream, node->children[1], flags);

    return 0;
}

int T3LightSceneInternalDataRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 6;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    for (uint16_t i = 0; i < 4; ++i)
    {
        cfseek(stream, sizeof(uint32_t), SEEK_CUR);
        node->children[i]->isBlocked = 1;
        node->children[i]->description = getMetaClassDescriptionByIndex(T3LightSceneInternalData__QualityEntry);
        node->children[i]->description->read(stream, node->children[i], flags);
    }

    node->children[4]->description = getMetaClassDescriptionByIndex(unsignedlong);
    node->children[4]->description->read(stream, node->children[4], flags);

    node->children[5]->description = getMetaClassDescriptionByIndex(unsignedlong);
    node->children[5]->description->read(stream, node->children[5], flags);

    return 0;
}

int LocationInfoRead(FILE *stream, struct TreeNode *node, uint32_t flags)
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
    node->children[0]->description = getMetaClassDescriptionByIndex(String);
    node->children[0]->description->read(stream, node->children[0], flags);

    node->children[1]->description = getMetaClassDescriptionByIndex(Symbol);
    node->children[1]->description->read(stream, node->children[1], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[2]->isBlocked = 1;
    node->children[2]->description = getMetaClassDescriptionByIndex(Transform);
    node->children[2]->description->read(stream, node->children[2], flags);

    return 0;
}

int SceneAgentInfoRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 2;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
        node->children[i]->isBlocked = 1;
    }

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->description = getMetaClassDescriptionByIndex(String);
    node->children[0]->description->read(stream, node->children[0], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[1]->description = getMetaClassDescriptionByIndex(PropertySet);
    node->children[1]->description->read(stream, node->children[1], flags);

    return 0;
}

int SceneRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 4;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    node->children[0]->description = getMetaClassDescriptionByIndex(bool_type);
    node->children[0]->description->read(stream, node->children[0], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[1]->isBlocked = 1;
    node->children[1]->description = getMetaClassDescriptionByIndex(String);
    node->children[1]->description->read(stream, node->children[1], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[2]->isBlocked = 1;
    node->children[2]->description = getMetaClassDescriptionByIndex(DCArray_HandleLock_Scene__);
    node->children[2]->description->read(stream, node->children[2], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[3]->isBlocked = 1;
    node->children[3]->description = getMetaClassDescriptionByIndex(LinkedList_Scene__AgentInfo0_);
    node->children[3]->description->read(stream, node->children[3], flags);

    return 0;
}