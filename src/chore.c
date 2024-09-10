#include <stdlib.h>
#include <tree.h>
#include <ttstring.h>
#include <intrinsic.h>
#include <prop.h>
#include <dlg.h>
#include <types.h>
#include <meta.h>
#include <container.h>
#include <stream.h>
#include <string.h>
#include <stdarg.h>

enum ChoreFlags
{
    eResetNavCamsOnExit = 0x1,
    eBackgroundFade = 0x2,
    eBackgroundLoop = 0x4,
    eEndPause = 0x8,
    eAdditive_Depreciated = 0x10,
    eEmbedded = 0x20,
    eChoreCut = 0x40
};

enum ChoreResourceFlags
{
    eFilterMoverAnimation = 0x1,
    eFilterLipSyncAnimation = 0x2,
    ePlayAsMusic = 0x4,
    eAgentPropertiesResource = 0x8,
    eAgentBlockingResource = 0x10,
    eBlockingFilterXlate = 0x20,
    eBlockingFilterRotate = 0x40,
    eDisableTimeSync = 0x80,
    eFirstSubtitledResource = 0x100,
    eLastSubtitledResource = 0x200,
    eOverrideLipSyncAnimation = 0x400,
    eAgentPathBlockingResource = 0x800,
    eChoreCutResource = 0x1000,
    eChoreClipResource = 0x2000,
    eChoreMasterCutResource = 0x4000,
    eExtendEmbeddedGuides = 0x8000,
    eMirrorAnimation = 0x10000,
    eForceRelativeAnimation = 0x20000,
    eAddAgentPosKeyAtTime0 = 0x40000,
    eFilterPropertyAnimation = 0x80000
};

int Map_StringClipResourceFilterStringCompareCaseInsensitive_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericMapRead(stream, node, flags, getMetaClassDescriptionByIndex(String), getMetaClassDescriptionByIndex(ClipResourceFilter));
}

int Set_StringStringCompareCaseInsensitive_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(String));
}

int DCArray_Handle_Chore__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(Handle_Chore_));
}

int Map_SymbolWalkPathless_Symbol__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericMapRead(stream, node, flags, getMetaClassDescriptionByIndex(Symbol), getMetaClassDescriptionByIndex(WalkPath));
}

int PerAgentClipResourceFilterRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "mIncludedAgents", .metaClassDescriptionIndex = Map_StringClipResourceFilterStringCompareCaseInsensitive_},
        {.isBlocked = 1, .memberName = "mExcludedAgents", .metaClassDescriptionIndex = Set_StringStringCompareCaseInsensitive_},
        {.isBlocked = 0, .memberName = "mbExclusiveMode", .metaClassDescriptionIndex = bool_type},
    };
    return genericRead(stream, node, flags, 3, descriptions);
}

int WalkPathRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->child = calloc(1, sizeof(struct TreeNode));
    struct TreeNode *currentNode = node->child;
    currentNode->parent = node;
    currentNode->isBlocked = 1;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    currentNode->description = getMetaClassDescriptionByIndex(String);
    currentNode->description->read(stream, currentNode, flags);

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;

    currentNode->child = calloc(1, sizeof(struct TreeNode));
    currentNode->child->parent = currentNode;
    currentNode = currentNode->child;
    currentNode->description = getMetaClassDescriptionByIndex(int_type);
    currentNode->description->read(stream, currentNode, flags);

    uint32_t nodeCount = *(uint32_t *)(currentNode->staticBuffer);
    for (uint32_t i = 0; i < nodeCount; ++i)
    {
        currentNode->sibling = calloc(1, sizeof(struct TreeNode));
        currentNode->sibling->parent = currentNode->parent;
        currentNode = currentNode->sibling;

        uint64_t typeSymbol;
        fread(&typeSymbol, sizeof(typeSymbol), 1, stream);
        currentNode->serializeType = 1;
        currentNode->description = getMetaClassDescriptionBySymbol(typeSymbol);
        currentNode->description->read(stream, currentNode, flags);
    }

    return 0;
}

int HandleBaseRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    (void)flags;
    node->dataSize = sizeof(uint64_t);
    fread(node->staticBuffer, sizeof(uint64_t), 1, stream);

    if (*(uint64_t *)node->staticBuffer != 0)
    {
        node->dataSize += sizeof(uint64_t);
        uint8_t *data = malloc(node->dataSize);
        if (data == NULL)
        {
            exit(-1);
        }
        memcpy(data, node->staticBuffer, sizeof(node->staticBuffer));
        fread(data + 8, sizeof(uint64_t), 1, stream);
        node->dynamicBuffer = data;
    }

    return 0;
}

int ChoreAgentAttachmentRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 0, .memberName = "mbDoAttach", .metaClassDescriptionIndex = bool_type},
        {.isBlocked = 1, .memberName = "mAttachTo", .metaClassDescriptionIndex = String},
        {.isBlocked = 1, .memberName = "mAttachToNode", .metaClassDescriptionIndex = String},
        {.isBlocked = 0, .memberName = "mAttachPos", .metaClassDescriptionIndex = Vector3},
        {.isBlocked = 0, .memberName = "mAttachQuat", .metaClassDescriptionIndex = Quaternion},
        {.isBlocked = 0, .memberName = "mbAttachPreserveWorldPos", .metaClassDescriptionIndex = bool_type},
        {.isBlocked = 0, .memberName = "mbLeaveAttachedWhenComplete", .metaClassDescriptionIndex = bool_type},
    };
    return genericRead(stream, node, flags, 7, descriptions);
}

int DCArray_int_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(int_type));
}

int ChoreAgentRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "mAgentName", .metaClassDescriptionIndex = String},
        {.isBlocked = 0, .memberName = "mFlags", .metaClassDescriptionIndex = Flags},
        {.isBlocked = 1, .memberName = "mResources", .metaClassDescriptionIndex = DCArray_int_},
        {.isBlocked = 1, .memberName = "mAttachment", .metaClassDescriptionIndex = ChoreAgent__Attachment},
        {.isBlocked = 1, .memberName = "mAABinding", .metaClassDescriptionIndex = ActorAgentBinding}, // TODO: Fix read actorAgentBinding
        {.isBlocked = 1, .memberName = "mAgentEnabledRule", .metaClassDescriptionIndex = Rule},
    };
    return genericRead(stream, node, flags, 6, descriptions);
}

int ChoreResourceBlockRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    (void)flags;
    node->dataSize = sizeof(float) * 3 + sizeof(uint8_t);
    node->dynamicBuffer = malloc(node->dataSize);
    fread(node->dynamicBuffer, node->dataSize, 1, stream);

    return 0;
}

int DCArray_ChoreResource__Block_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(ChoreResource__Block));
}

int Map_Symbolfloatless_Symbol__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericMapRead(stream, node, flags, getMetaClassDescriptionByIndex(Symbol), getMetaClassDescriptionByIndex(float_type));
}

int ChoreResourceRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 0, .memberName = "mVersion", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 0, .memberName = "mResName", .metaClassDescriptionIndex = Symbol},
        {.isBlocked = 0, .memberName = "mResLength", .metaClassDescriptionIndex = float_type},
        {.isBlocked = 0, .memberName = "mPriority", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 0, .memberName = "mFlags", .metaClassDescriptionIndex = Flags},
        {.isBlocked = 1, .memberName = "mResourceGroup", .metaClassDescriptionIndex = String},
        {.isBlocked = 1, .memberName = "mhObject", .metaClassDescriptionIndex = HandleBase},
        {.isBlocked = 1, .memberName = "mControlAnimation", .metaClassDescriptionIndex = Animation},
        {.isBlocked = 1, .memberName = "mBlocks", .metaClassDescriptionIndex = DCArray_ChoreResource__Block_},
        {.isBlocked = 0, .memberName = "mbNoPose", .metaClassDescriptionIndex = bool_type},
        {.isBlocked = 0, .memberName = "mbEmbedded", .metaClassDescriptionIndex = bool_type},
        {.isBlocked = 0, .memberName = "mbEnabled", .metaClassDescriptionIndex = bool_type},
        {.isBlocked = 0, .memberName = "mbIsAgentResource", .metaClassDescriptionIndex = bool_type},
        {.isBlocked = 0, .memberName = "mbViewGraphs", .metaClassDescriptionIndex = bool_type},
        {.isBlocked = 0, .memberName = "mbViewEmptyGraphs", .metaClassDescriptionIndex = bool_type},
        {.isBlocked = 0, .memberName = "mbViewProperties", .metaClassDescriptionIndex = bool_type},
        {.isBlocked = 0, .memberName = "mbViewResourceGroups", .metaClassDescriptionIndex = bool_type},
        {.isBlocked = 1, .memberName = "mResourceProperties", .metaClassDescriptionIndex = PropertySet},
        {.isBlocked = 1, .memberName = "mResourceGroupInclude", .metaClassDescriptionIndex = Map_Symbolfloatless_Symbol__},
        {.isBlocked = 1, .memberName = "mAAStatus", .metaClassDescriptionIndex = AutoActStatus},
    };
    genericRead(stream, node, flags, 20, descriptions);

    struct TreeNode *currentNode = node->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling; // I don't have that many siblings, so this is misleading

    if (*(currentNode->staticBuffer) == '1')
    {
        currentNode = currentNode->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling;

        currentNode->sibling = calloc(1, sizeof(struct TreeNode));
        currentNode->sibling->parent = currentNode->parent;
        currentNode = currentNode->sibling;
        currentNode->description = getMetaClassDescriptionByIndex(Symbol);
        currentNode->description->read(stream, currentNode, flags);

        currentNode->sibling = calloc(1, sizeof(struct TreeNode));
        currentNode->sibling->parent = currentNode->parent;
        currentNode = currentNode->sibling;
        currentNode->description = getMetaClassDescriptionByIndex(Symbol);
        currentNode->description->read(stream, currentNode, flags);

        currentNode->sibling = calloc(1, sizeof(struct TreeNode));
        currentNode->sibling->parent = currentNode->parent;
        currentNode->sibling->description = getMetaClassDescriptionBySymbol(*(uint64_t *)currentNode->staticBuffer);
        currentNode = currentNode->sibling;
        currentNode->description->read(stream, currentNode, flags);
    }

    return 0;
}

int ChoreRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "mName", .metaClassDescriptionIndex = String},
        {.isBlocked = 0, .memberName = "mFlags", .metaClassDescriptionIndex = Flags},
        {.isBlocked = 0, .memberName = "mLength", .metaClassDescriptionIndex = float_type},
        {.isBlocked = 0, .memberName = "mNumResources", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 0, .memberName = "mNumAgents", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 1, .memberName = "mEditorProps", .metaClassDescriptionIndex = PropertySet},
        {.isBlocked = 1, .memberName = "mChoreSceneFile", .metaClassDescriptionIndex = String},
        {.isBlocked = 0, .memberName = "mRenderDelay", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 1, .memberName = "mSynchronizedToLocalization", .metaClassDescriptionIndex = LocalizeInfo},
        {.isBlocked = 1, .memberName = "mDependencies", .metaClassDescriptionIndex = DependencyLoader_1_},
        {.isBlocked = 0, .memberName = "mToolProps", .metaClassDescriptionIndex = ToolProps},
        {.isBlocked = 1, .memberName = "mWalkPaths", .metaClassDescriptionIndex = Map_SymbolWalkPathless_Symbol__},
    };
    genericRead(stream, node, flags, 12, descriptions);

    int32_t choreResourceCount = *(int32_t *)(node->child->sibling->sibling->sibling->staticBuffer);
    int32_t choreAgentCount = *(int32_t *)(node->child->sibling->sibling->sibling->sibling->staticBuffer);
    struct TreeNode *currentNode = node->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling;

    for (int32_t i = 0; i < choreResourceCount; ++i)
    {
        currentNode->sibling = calloc(1, sizeof(struct TreeNode));
        currentNode->sibling->parent = currentNode->parent;
        currentNode = currentNode->sibling;
        currentNode->description = getMetaClassDescriptionByIndex(ChoreResource);
        currentNode->description->read(stream, currentNode, flags);
    }

    for (int32_t i = 0; i < choreAgentCount; ++i)
    {
        currentNode->sibling = calloc(1, sizeof(struct TreeNode));
        currentNode->sibling->parent = currentNode->parent;
        currentNode = currentNode->sibling;
        currentNode->description = getMetaClassDescriptionByIndex(ChoreAgent);
        currentNode->description->read(stream, currentNode, flags);
    }

    return 0;
}
