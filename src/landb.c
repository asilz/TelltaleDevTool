#include <tree.h>
#include <ttstring.h>
#include <intrinsic.h>
#include <stream.h>
#include <stdlib.h>
#include <container.h>
#include <types.h>
#include <meta.h>

int LanguageResLocalRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "mPrefix", .metaClassDescriptionIndex = String},
        {.isBlocked = 1, .memberName = "mText", .metaClassDescriptionIndex = String},
        {.isBlocked = 1, .memberName = "localizeInfo", .metaClassDescriptionIndex = LocalizeInfo},
    };
    return genericRead(stream, node, flags, 3, descriptions);
}

int DCArray_LanguageResLocal_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(LanguageResLocal));
}

int LanguageResRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 0, .memberName = "mResName", .metaClassDescriptionIndex = Symbol},
        {.isBlocked = 0, .memberName = "mID", .metaClassDescriptionIndex = unsignedlong},
        {.isBlocked = 0, .memberName = "mIDAlias", .metaClassDescriptionIndex = unsignedlong},
        {.isBlocked = 1, .memberName = "mhAnimation", .metaClassDescriptionIndex = Handle_Animation_},
        {.isBlocked = 1, .memberName = "mhVoiceData", .metaClassDescriptionIndex = Handle_SoundData_},
        {.isBlocked = 1, .memberName = "mLocalData", .metaClassDescriptionIndex = DCArray_LanguageResLocal_},
        {.isBlocked = 0, .memberName = "mLengthOverride", .metaClassDescriptionIndex = float_type},
        {.isBlocked = 1, .memberName = "mResolvedLocalData", .metaClassDescriptionIndex = LanguageResLocal},
        {.isBlocked = 1, .memberName = "mRecordingStatus", .metaClassDescriptionIndex = RecordingUtils__EnumRecordingStatus},
        {.isBlocked = 0, .memberName = "mFlags", .metaClassDescriptionIndex = Flags},
    };
    return genericRead(stream, node, flags, 10, descriptions);
}

int Map_intSymbolless_int__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericMapRead(stream, node, flags, getMetaClassDescriptionByIndex(int_type), getMetaClassDescriptionByIndex(Symbol));
}

int LocalizationRegistryRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "mFlagIndexMap", .metaClassDescriptionIndex = Map_Symbolintless_Symbol__},
        {.isBlocked = 1, .memberName = "mFlagIndexMapReverse", .metaClassDescriptionIndex = Map_intSymbolless_int__},
        {.isBlocked = 0, .memberName = "mToolProps", .metaClassDescriptionIndex = ToolProps},
    };
    return genericRead(stream, node, flags, 3, descriptions);
}

int ProjectDatabaseIDPairRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 0, .memberName = "mProjectID", .metaClassDescriptionIndex = unsignedlong},
        {.isBlocked = 0, .memberName = "mDBID", .metaClassDescriptionIndex = long_type},
    };
    return genericRead(stream, node, flags, 2, descriptions);
}

static int LangAnmWavTripletRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 0, .memberName = "langName", .metaClassDescriptionIndex = String},
        {.isBlocked = 0, .memberName = "anmName", .metaClassDescriptionIndex = String},
        {.isBlocked = 0, .memberName = "wavName", .metaClassDescriptionIndex = String},
    };
    return genericRead(stream, node, flags, 3, descriptions);
}

int DCArray_ProjectDatabaseIDPair_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(ProjectDatabaseIDPair));
}

int Map_unsignedintLanguageResless_unsignedint__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericMapRead(stream, node, flags, getMetaClassDescriptionByIndex(int_type), getMetaClassDescriptionByIndex(LanguageRes));
}

int LanguageDBRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_UID::Owner", .metaClassDescriptionIndex = UID__Owner},
        {.isBlocked = 1, .memberName = "Baseclass_UID::Generator", .metaClassDescriptionIndex = UID__Generator},
        {.isBlocked = 1, .memberName = "mLanguageResources", .metaClassDescriptionIndex = Map_unsignedintLanguageResless_unsignedint__},
        {.isBlocked = 1, .memberName = "mRegistry", .metaClassDescriptionIndex = LocalizationRegistry},
        {.isBlocked = 0, .memberName = "mFlags", .metaClassDescriptionIndex = Flags},
        {.isBlocked = 0, .memberName = "mProjectID", .metaClassDescriptionIndex = unsignedlong},
        {.isBlocked = 1, .memberName = "mExpandedIDRanges", .metaClassDescriptionIndex = DCArray_ProjectDatabaseIDPair_},
    };
    genericRead(stream, node, flags, 7, descriptions);

    struct TreeNode *currentNode = node->child->sibling->sibling->sibling->sibling->sibling->sibling;

    // node->children[7]->typeSymbol = 0; // TODO: Set description
    for (uint32_t i = 0; i < *(uint32_t *)(node->child->sibling->sibling->child->staticBuffer); ++i)
    {
        currentNode->sibling = calloc(1, sizeof(struct TreeNode));
        currentNode->sibling->parent = currentNode->parent;
        currentNode = currentNode->sibling;
        LangAnmWavTripletRead(stream, currentNode, flags);
    }

    return 0;
}