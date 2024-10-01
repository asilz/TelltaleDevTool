#include <tree.h>
#include <stream.h>
#include <container.h>
#include <stdlib.h>
#include <types.h>
#include <meta.h>
#include <assert.h>

int AnimationValueInterfaceBaseRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 0, .memberName = "mName", .metaClassDescriptionIndex = Symbol},
        {.isBlocked = 0, .memberName = "mFlags", .metaClassDescriptionIndex = Flags},
    };
    return genericRead(stream, node, flags, 2, descriptions);
}

int AnimatedValueInterfaceGenericRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
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

// TODO: Use macros instead

int DCArray_KeyframedValue_Transform___Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_Generic___Sample_Read(stream, node, flags, 1, Transform);
}

int DCArray_KeyframedValue_String___Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_Generic___Sample_Read(stream, node, flags, 1, String);
}

int DCArray_KeyframedValue_Handle_Chore____Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_Generic___Sample_Read(stream, node, flags, 1, Handle_Chore_);
}

int DCArray_KeyframedValue_Handle_Scene____Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_Generic___Sample_Read(stream, node, flags, 1, Handle_Scene_);
}

int DCArray_KeyframedValue_Handle_PropertySet____Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_Generic___Sample_Read(stream, node, flags, 1, Handle_PropertySet_);
}

int DCArray_KeyframedValue_Handle_Font____Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_Generic___Sample_Read(stream, node, flags, 1, Handle_Font_);
}

int DCArray_KeyframedValue_Handle_Dlg____Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_Generic___Sample_Read(stream, node, flags, 1, Handle_Dlg_);
}

int DCArray_KeyframedValue_Handle_SoundData____Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_Generic___Sample_Read(stream, node, flags, 1, Handle_SoundData_);
}

int DCArray_KeyframedValue_Handle_SoundEventData____Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_Generic___Sample_Read(stream, node, flags, 1, Handle_SoundEventData_);
}

int DCArray_KeyframedValue_Handle_SoundEventSnapshotData____Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_Generic___Sample_Read(stream, node, flags, 1, Handle_SoundEventSnapshotData_);
}

int DCArray_KeyframedValue_Handle_SoundReverbDefinition____Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_Generic___Sample_Read(stream, node, flags, 1, Handle_SoundReverbDefinition_);
}

int DCArray_KeyframedValue_Handle_WalkBoxes____Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_Generic___Sample_Read(stream, node, flags, 1, Handle_WalkBoxes_);
}

int DCArray_KeyframedValue_Handle_D3DMesh____Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_Generic___Sample_Read(stream, node, flags, 1, Handle_D3DMesh_);
}

int DCArray_KeyframedValue_Handle_T3Texture____Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_Generic___Sample_Read(stream, node, flags, 1, Handle_T3Texture_);
}

int DCArray_KeyframedValue_Handle_PhonemeTable____Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_Generic___Sample_Read(stream, node, flags, 1, Handle_PhonemeTable_);
}

int DCArray_KeyframedValue_ScriptEnum___Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_Generic___Sample_Read(stream, node, flags, 1, ScriptEnum);
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

int DCArray_KeyframedValue_Quaternion___Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_Generic___Sample_Read(stream, node, flags, 0, Quaternion);
}

int DCArray_KeyframedValue_Symbol___Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_Generic___Sample_Read(stream, node, flags, 0, Symbol);
}

int DCArray_KeyframedValue_int___Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_Generic___Sample_Read(stream, node, flags, 0, int_type);
}

int DCArray_KeyframedValue_Vector2___Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_Generic___Sample_Read(stream, node, flags, 0, Vector2);
}

int DCArray_KeyframedValue_Vector3___Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_Generic___Sample_Read(stream, node, flags, 0, Vector3);
}

int DCArray_KeyframedValue_Vector4___Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_Generic___Sample_Read(stream, node, flags, 0, Vector4);
}

int DCArray_KeyframedValue_Handle_SoundBusSnapshot__Snapshot____Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_Generic___Sample_Read(stream, node, flags, 1, Handle_SoundBusSnapshot__Snapshot_);
}

int DCArray_KeyframedValue_Handle_SoundBusSnapshot__SnapshotSuite____Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_Generic___Sample_Read(stream, node, flags, 1, Handle_SoundBusSnapshot__SnapshotSuite_);
}

int DCArray_KeyframedValue_Handle_SoundAmbience__AmbienceDefinition____Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_Generic___Sample_Read(stream, node, flags, 1, Handle_SoundAmbience__AmbienceDefinition_);
}

int DCArray_KeyframedValue_SoundEventName_0____Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_Generic___Sample_Read(stream, node, flags, 1, SoundEventName_0_);
}

int DCArray_KeyframedValue_SoundEventName_1____Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_Generic___Sample_Read(stream, node, flags, 1, SoundEventName_1_);
}

int DCArray_KeyframedValue_SoundEventName_2____Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DCArray_KeyframedValue_Generic___Sample_Read(stream, node, flags, 1, SoundEventName_2_);
}

int KeyframedValue_Transform_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_Transform_},
        {.isBlocked = 1, .memberName = "minValue", .metaClassDescriptionIndex = Transform},
        {.isBlocked = 1, .memberName = "maxValue", .metaClassDescriptionIndex = Transform},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_Transform___Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int KeyframedValue_float_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_float_},
        {.isBlocked = 0, .memberName = "minValue", .metaClassDescriptionIndex = float_type},
        {.isBlocked = 0, .memberName = "maxValue", .metaClassDescriptionIndex = float_type},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_float___Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int KeyframedValue_unsigned__int64_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_unsigned__int64_},
        {.isBlocked = 0, .memberName = "minValue", .metaClassDescriptionIndex = unsigned__int64},
        {.isBlocked = 0, .memberName = "maxValue", .metaClassDescriptionIndex = unsigned__int64},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_unsigned__int64___Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int KeyframedValue_AnimOrChore_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_AnimOrChore_},
        {.isBlocked = 1, .memberName = "minValue", .metaClassDescriptionIndex = AnimOrChore},
        {.isBlocked = 1, .memberName = "maxValue", .metaClassDescriptionIndex = AnimOrChore},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_AnimOrChore___Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int KeyframedValue_bool_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_bool_},
        {.isBlocked = 0, .memberName = "minValue", .metaClassDescriptionIndex = bool_type},
        {.isBlocked = 0, .memberName = "maxValue", .metaClassDescriptionIndex = bool_type},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_bool___Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int KeyframedValue_String_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_String_},
        {.isBlocked = 1, .memberName = "minValue", .metaClassDescriptionIndex = String},
        {.isBlocked = 1, .memberName = "maxValue", .metaClassDescriptionIndex = String},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_String___Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int KeyframedValue_Handle_Chore__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_Handle_Chore__},
        {.isBlocked = 1, .memberName = "minValue", .metaClassDescriptionIndex = Handle_Chore_},
        {.isBlocked = 1, .memberName = "maxValue", .metaClassDescriptionIndex = Handle_Chore_},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_Handle_Chore____Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int KeyframedValue_Handle_Scene__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_Handle_Scene__},
        {.isBlocked = 1, .memberName = "minValue", .metaClassDescriptionIndex = Handle_Scene_},
        {.isBlocked = 1, .memberName = "maxValue", .metaClassDescriptionIndex = Handle_Scene_},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_Handle_Scene____Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int KeyframedValue_Handle_PropertySet__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_Handle_PropertySet__},
        {.isBlocked = 1, .memberName = "minValue", .metaClassDescriptionIndex = Handle_PropertySet_},
        {.isBlocked = 1, .memberName = "maxValue", .metaClassDescriptionIndex = Handle_PropertySet_},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_Handle_PropertySet____Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int KeyframedValue_Handle_Font__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_Handle_Font__},
        {.isBlocked = 1, .memberName = "minValue", .metaClassDescriptionIndex = Handle_Font_},
        {.isBlocked = 1, .memberName = "maxValue", .metaClassDescriptionIndex = Handle_Font_},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_Handle_Font____Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int KeyframedValue_Handle_Dlg__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_Handle_Dlg__},
        {.isBlocked = 1, .memberName = "minValue", .metaClassDescriptionIndex = Handle_Dlg_},
        {.isBlocked = 1, .memberName = "maxValue", .metaClassDescriptionIndex = Handle_Dlg_},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_Handle_Dlg____Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int KeyframedValue_Handle_SoundData__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_Handle_SoundData__},
        {.isBlocked = 1, .memberName = "minValue", .metaClassDescriptionIndex = Handle_SoundData_},
        {.isBlocked = 1, .memberName = "maxValue", .metaClassDescriptionIndex = Handle_SoundData_},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_Handle_SoundData____Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int KeyframedValue_Handle_SoundEventData__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_Handle_SoundEventData__},
        {.isBlocked = 1, .memberName = "minValue", .metaClassDescriptionIndex = Handle_SoundEventData_},
        {.isBlocked = 1, .memberName = "maxValue", .metaClassDescriptionIndex = Handle_SoundEventData_},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_Handle_SoundEventData____Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int KeyframedValue_Handle_SoundEventSnapshotData__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_Handle_SoundEventSnapshotData__},
        {.isBlocked = 1, .memberName = "minValue", .metaClassDescriptionIndex = Handle_SoundEventSnapshotData_},
        {.isBlocked = 1, .memberName = "maxValue", .metaClassDescriptionIndex = Handle_SoundEventSnapshotData_},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_Handle_SoundEventSnapshotData____Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int KeyframedValue_Handle_SoundReverbDefinition__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_Handle_SoundReverbDefinition__},
        {.isBlocked = 1, .memberName = "minValue", .metaClassDescriptionIndex = Handle_SoundReverbDefinition_},
        {.isBlocked = 1, .memberName = "maxValue", .metaClassDescriptionIndex = Handle_SoundReverbDefinition_},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_Handle_SoundReverbDefinition____Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int KeyframedValue_Handle_WalkBoxes__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_Handle_WalkBoxes__},
        {.isBlocked = 1, .memberName = "minValue", .metaClassDescriptionIndex = Handle_WalkBoxes_},
        {.isBlocked = 1, .memberName = "maxValue", .metaClassDescriptionIndex = Handle_WalkBoxes_},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_Handle_WalkBoxes____Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int KeyframedValue_Handle_D3DMesh__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_Handle_D3DMesh__},
        {.isBlocked = 1, .memberName = "minValue", .metaClassDescriptionIndex = Handle_D3DMesh_},
        {.isBlocked = 1, .memberName = "maxValue", .metaClassDescriptionIndex = Handle_D3DMesh_},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_Handle_D3DMesh____Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int KeyframedValue_Handle_T3Texture__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_Handle_T3Texture__},
        {.isBlocked = 1, .memberName = "minValue", .metaClassDescriptionIndex = Handle_T3Texture_},
        {.isBlocked = 1, .memberName = "maxValue", .metaClassDescriptionIndex = Handle_T3Texture_},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_Handle_T3Texture____Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int KeyframedValue_Handle_PhonemeTable__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_Handle_PhonemeTable__},
        {.isBlocked = 1, .memberName = "minValue", .metaClassDescriptionIndex = Handle_PhonemeTable_},
        {.isBlocked = 1, .memberName = "maxValue", .metaClassDescriptionIndex = Handle_PhonemeTable_},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_Handle_PhonemeTable____Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int KeyframedValue_ScriptEnum_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_ScriptEnum_},
        {.isBlocked = 1, .memberName = "minValue", .metaClassDescriptionIndex = ScriptEnum},
        {.isBlocked = 1, .memberName = "maxValue", .metaClassDescriptionIndex = ScriptEnum},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_ScriptEnum___Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int KeyframedValue_Quaternion_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_Quaternion_},
        {.isBlocked = 0, .memberName = "minValue", .metaClassDescriptionIndex = Quaternion},
        {.isBlocked = 0, .memberName = "maxValue", .metaClassDescriptionIndex = Quaternion},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_Quaternion___Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int KeyframedValue_Symbol_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_Symbol_},
        {.isBlocked = 0, .memberName = "minValue", .metaClassDescriptionIndex = Symbol},
        {.isBlocked = 0, .memberName = "maxValue", .metaClassDescriptionIndex = Symbol},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_Symbol___Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int KeyframedValue_int_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_int_},
        {.isBlocked = 0, .memberName = "minValue", .metaClassDescriptionIndex = int_type},
        {.isBlocked = 0, .memberName = "maxValue", .metaClassDescriptionIndex = int_type},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_int___Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int KeyframedValue_Vector2_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_Vector2_},
        {.isBlocked = 0, .memberName = "minValue", .metaClassDescriptionIndex = Vector2},
        {.isBlocked = 0, .memberName = "maxValue", .metaClassDescriptionIndex = Vector2},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_Vector2___Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int KeyframedValue_Vector3_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_Vector3_},
        {.isBlocked = 0, .memberName = "minValue", .metaClassDescriptionIndex = Vector3},
        {.isBlocked = 0, .memberName = "maxValue", .metaClassDescriptionIndex = Vector3},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_Vector3___Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int KeyframedValue_Vector4_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_Vector4_},
        {.isBlocked = 0, .memberName = "minValue", .metaClassDescriptionIndex = Vector4},
        {.isBlocked = 0, .memberName = "maxValue", .metaClassDescriptionIndex = Vector4},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_Vector4___Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int KeyframedValue_Handle_SoundBusSnapshot__Snapshot__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_Handle_SoundBusSnapshot__Snapshot__},
        {.isBlocked = 1, .memberName = "minValue", .metaClassDescriptionIndex = Handle_SoundBusSnapshot__Snapshot_},
        {.isBlocked = 1, .memberName = "maxValue", .metaClassDescriptionIndex = Handle_SoundBusSnapshot__Snapshot_},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_Handle_SoundBusSnapshot__Snapshot____Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int KeyframedValue_Handle_SoundBusSnapshot__SnapshotSuite__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_Handle_SoundBusSnapshot__SnapshotSuite__},
        {.isBlocked = 1, .memberName = "minValue", .metaClassDescriptionIndex = Handle_SoundBusSnapshot__SnapshotSuite_},
        {.isBlocked = 1, .memberName = "maxValue", .metaClassDescriptionIndex = Handle_SoundBusSnapshot__SnapshotSuite_},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_Handle_SoundBusSnapshot__SnapshotSuite____Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int KeyframedValue_Handle_SoundAmbience__AmbienceDefinition__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_Handle_SoundAmbience__AmbienceDefinition__},
        {.isBlocked = 1, .memberName = "minValue", .metaClassDescriptionIndex = Handle_SoundAmbience__AmbienceDefinition_},
        {.isBlocked = 1, .memberName = "maxValue", .metaClassDescriptionIndex = Handle_SoundAmbience__AmbienceDefinition_},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_Handle_SoundAmbience__AmbienceDefinition____Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int KeyframedValue_SoundEventName_0__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_SoundEventName_0__},
        {.isBlocked = 1, .memberName = "minValue", .metaClassDescriptionIndex = SoundEventName_0_},
        {.isBlocked = 1, .memberName = "maxValue", .metaClassDescriptionIndex = SoundEventName_0_},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_SoundEventName_0____Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int KeyframedValue_SoundEventName_1__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_SoundEventName_1__},
        {.isBlocked = 1, .memberName = "minValue", .metaClassDescriptionIndex = SoundEventName_1_},
        {.isBlocked = 1, .memberName = "maxValue", .metaClassDescriptionIndex = SoundEventName_1_},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_SoundEventName_1____Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int KeyframedValue_SoundEventName_2__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_AnimatedValueInterface_T_", .metaClassDescriptionIndex = AnimatedValueInterface_SoundEventName_2__},
        {.isBlocked = 1, .memberName = "minValue", .metaClassDescriptionIndex = SoundEventName_2_},
        {.isBlocked = 1, .memberName = "maxValue", .metaClassDescriptionIndex = SoundEventName_2_},
        {.isBlocked = 1, .memberName = "samples", .metaClassDescriptionIndex = DCArray_KeyframedValue_SoundEventName_2____Sample_},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}
