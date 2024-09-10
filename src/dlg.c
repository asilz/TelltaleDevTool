#include <stdlib.h>
#include <meta.h>
#include <assert.h>
#include <dlg.h>
#include <prop.h>
#include <intrinsic.h>
#include <stddef.h>
#include <container.h>
#include <ttstring.h>
#include <stream.h>
#include <types.h>

enum DlgObjectClassFlag
{
    eObjClassInvalid = 0x0,
    eObjClassDialog = 0x1,
    eObjClassFolder = 0x2,
    eObjClassNodeChoices = 0x4,
    eObjClassNodeConditional = 0x8,
    eObjClassNodeChore = 0x10,
    eObjClassNodeExchange = 0x20,
    eObjClassNodeExit = 0x40,
    eObjClassNodeIdle = 0x80,
    eObjClassNodeJump = 0x100,
    eObjClassNodeLogic = 0x200,
    eObjClassNodeScript = 0x400,
    eObjClassNodeSequence = 0x800,
    eObjClassNodeStart = 0x1000,
    eObjClassNodeText = 0x2000,
    eObjClassNodeWait = 0x4000,
    eObjClassNodeNotes = 0x8000,
    eObjClassNodeCancelChoices = 0x10000,
    eObjClassNodeParallel = 0x20000,
    eObjClassNodeMarker = 0x40000,
    eObjClassNodeStoryBoard = 0x80000,
    eObjClassNodeStats = 0x100000,
    eObjClassChildChoices = 0x200000,
    eObjClassChildChoicesPre = 0x400000,
    eObjClassChildChoicesPost = 0x800000,
    eObjClassChildSequence = 0x1000000,
    eObjClassChildConditional = 0x2000000,
    eObjClassChildFolder = 0x4000000,
    eObjClassChildParallel = 0x8000000,
    eObjClassChildStats = 0x10000000
};

enum ClassFlags
{
    eAction = 0x1,
    eOrganization = 0x2,
    eData = 0x4,
    eProduction = 0x8,
    eLangType = 0x10,
    eParentType = 0x20,
    eParsableType = 0x40,
};

int Vector3Read(FILE *stream, struct TreeNode *node, uint32_t flags) // TODO: Move this function to a different file
{
    (void)flags;
    node->dataSize = sizeof(float) * 3;
    node->dynamicBuffer = malloc(node->dataSize);
    fread(node->dynamicBuffer, node->dataSize, 1, stream);

    return 0;
}

int AnimOrChoreRead(FILE *stream, struct TreeNode *node, uint32_t flags) // TODO: Move this function to a different file
{
    const static struct MetaMemberDescription const descriptions[] = {{.isBlocked = 1, .memberName = "animation", .metaClassDescriptionIndex = Handle_Animation_},
                                                                      {.isBlocked = 1, .memberName = "chore", .metaClassDescriptionIndex = Handle_Chore_}};
    return genericRead(stream, node, flags, 2, descriptions);
}

int Set_Colorless_Color__Read(FILE *stream, struct TreeNode *node, uint32_t flags) // TODO: Move to different file
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(Color));
}

int Vector4Read(FILE *stream, struct TreeNode *node, uint32_t flags) // TODO: Move to different file
{
    (void)flags;
    node->dataSize = sizeof(float) * 4;
    node->dynamicBuffer = malloc(node->dataSize);
    fread(node->dynamicBuffer, node->dataSize, 1, stream);

    return 0;
}

int ScriptEnumRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "curValue", .metaClassDescriptionIndex = String}};
    return genericRead(stream, node, flags, 1, descriptions);

    return 0;
}

int DependencyLoaderRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->child = calloc(1, sizeof(struct TreeNode));

    node->child->parent = node;
    node->child->description = getMetaClassDescriptionByIndex(bool_type);
    node->child->description->read(stream, node->child, flags);

    if (node->child->staticBuffer[0] == '1')
    {
        node->child->sibling = calloc(1, sizeof(struct TreeNode));
        node->child->sibling->serializeType = 1;
        uint64_t typeSymbol;
        fread(&typeSymbol, sizeof(typeSymbol), 1, stream);
        node->child->sibling->parent = node;
        node->child->sibling->description = getMetaClassDescriptionBySymbol(typeSymbol);
        node->child->sibling->description->read(stream, node->child->sibling, flags);
    }
    return 0;
}

uint64_t *DlgGetNextID(struct TreeNode *node)
{
    if (node->dataSize > 0)
    {
        printf("Error: DlgGetNextID: Node has no children %lx\n", node->description->crc);
        return 0;
    }
    if (node->description->crc == 0x85d954f4c0c69e97) // DlgNodeExchange
    {
        return (uint64_t *)(node->child->sibling->sibling->child->sibling->sibling->sibling->sibling->child->child->staticBuffer);
    }
    if (node->description->crc == 0x2a5aaf50752ffed9) // DlgNodeChore
    {
        return (uint64_t *)(node->child->sibling->sibling->sibling->child->sibling->sibling->sibling->sibling->child->child->staticBuffer);
    }
    if (node->child->description->crc == 0x8940087148bf4c61) // DlgNode
    {
        return (uint64_t *)(node->child->child->sibling->sibling->sibling->sibling->child->child->staticBuffer);
    }
    if (node->child->description->crc == 0xbfb0ce2bd1f38792) // DlgChild
    {
        return (uint64_t *)(node->child->child->child->sibling->child->child->staticBuffer);
    }
    printf("Error: DlgGetNextID: TreeNode is not a DlgNode/DlgChild %lx\n", node->description->crc);

    return NULL;
}

uint64_t *DlgGetPrevID(struct TreeNode *node)
{
    if (node->dataSize > 0)
    {
        printf("Error: DlgGetPrevID: Node has no children %lx\n", node->description->crc);
        return 0;
    }
    if (node->description->crc == 0x85d954f4c0c69e97) // DlgNodeExchange
    {
        return (uint64_t *)(node->child->sibling->sibling->child->sibling->sibling->sibling->child->child->staticBuffer);
    }
    if (node->description->crc == 0x2a5aaf50752ffed9) // DlgNodeChore
    {
        return (uint64_t *)(node->child->sibling->sibling->child->sibling->sibling->sibling->child->child->staticBuffer);
    }
    if (node->child->description->crc == 0x8940087148bf4c61) // DlgNode
    {
        return (uint64_t *)(node->child->child->sibling->sibling->sibling->child->child->staticBuffer);
    }
    if (node->child->description->crc == 0xbfb0ce2bd1f38792) // DlgChild
    {
        return (uint64_t *)(node->child->child->sibling->sibling->sibling->child->child->staticBuffer);
    }
    printf("Error: DlgGetPrevID: TreeNode is not a DlgNode/DlgChild %lx\n", node->description->crc);

    return NULL;
}

uint64_t *DlgGetID(struct TreeNode *node)
{
    // printf("%lx\n", node->typeSymbol);
    if (node->dataSize > 0)
    {
        printf("Error: DlgGetID: Node has no children %lx\n", node->description->crc);
        return 0;
    }
    if (node->description->crc == 0x85d954f4c0c69e97) // DlgNodeExchange
    {
        return (uint64_t *)(node->child->sibling->sibling->child->child->staticBuffer);
    }

    if (node->description->crc == 0x2a5aaf50752ffed9) // DlgNodeChore
    {
        return (uint64_t *)(node->child->sibling->sibling->sibling->child->child->staticBuffer);
    }

    if (node->child->description->crc == 0x8940087148bf4c61) // DlgNode
    {
        return (uint64_t *)(node->child->child->child->staticBuffer);
    }
    if (node->child->description->crc == 0xbfb0ce2bd1f38792) // DlgChild
    {
        return (uint64_t *)(node->child->child->child->child->staticBuffer);
    }
    printf("Error: DlgGetID: TreeNode is not a DlgNode/DlgChild %lx\n", node->description->crc);
    return NULL;
}

int DlgFolderRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_DlgObjIDOwner", .metaClassDescriptionIndex = DlgObjIDOwner},
        {.isBlocked = 1, .memberName = "Baseclass_DlgObjectPropsOwner", .metaClassDescriptionIndex = DlgObjectPropsOwner},
        {.isBlocked = 1, .memberName = "Baseclass_DlgChildSet", .metaClassDescriptionIndex = DlgChildSet},
        {.isBlocked = 0, .memberName = "mName", .metaClassDescriptionIndex = Symbol},
        {.isBlocked = 1, .memberName = "mProdReportProps", .metaClassDescriptionIndex = PropertySet},
    };
    return genericRead(stream, node, flags, 5, descriptions);
}

int JiraRecordManagerRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return 0;
}

int DlgRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_DlgObjIDOwner", .metaClassDescriptionIndex = DlgObjIDOwner},
        {.isBlocked = 1, .memberName = "mName", .metaClassDescriptionIndex = String},
        {.isBlocked = 0, .memberName = "mVersion", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 1, .memberName = "mDefFolderID", .metaClassDescriptionIndex = DlgObjID},
        {.isBlocked = 1, .memberName = "mLangDB", .metaClassDescriptionIndex = landb},
        {.isBlocked = 0, .memberName = "mProjectID", .metaClassDescriptionIndex = unsignedlong},
        {.isBlocked = 0, .memberName = "mResourceLocationID", .metaClassDescriptionIndex = Symbol},
        {.isBlocked = 0, .memberName = "mChronology", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 0, .memberName = "mFlags", .metaClassDescriptionIndex = Flags},
        {.isBlocked = 1, .memberName = "mDependencies", .metaClassDescriptionIndex = DependencyLoader_1_},
        {.isBlocked = 1, .memberName = "mProdReportProps", .metaClassDescriptionIndex = PropertySet},
        {.isBlocked = 1, .memberName = "mJiraRecordManager", .metaClassDescriptionIndex = JiraRecordManager},
        {.isBlocked = 0, .memberName = "mbHasToolOnlyData", .metaClassDescriptionIndex = bool_type},
    };
    genericRead(stream, node, flags, 13, descriptions);

    struct TreeNode *currentNode = node->child;
    do
    {
        currentNode = currentNode->sibling;
    } while (currentNode->sibling != NULL);

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode = currentNode->sibling;
    currentNode->parent = node;
    // dlg->children[13]->typeSymbol = 0; // TODO: Set symbol
    genericArrayRead(stream, currentNode, flags, getMetaClassDescriptionByIndex(DlgFolder));

    int64_t tell = cftell(stream);
    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode = currentNode->sibling;
    currentNode->parent = node;
    currentNode->description = NULL; // TODO: Set description

    currentNode->child = calloc(1, sizeof(struct TreeNode));
    currentNode->child->parent = currentNode;
    currentNode = currentNode->child;
    currentNode->description = getMetaClassDescriptionByIndex(int_type);
    currentNode->description->read(stream, currentNode, flags);

    uint32_t dlgNodeCount = *(uint32_t *)(currentNode->staticBuffer);
    for (uint32_t i = 0; i < dlgNodeCount; ++i)
    {
        currentNode->sibling = calloc(1, sizeof(struct TreeNode));
        currentNode->sibling->parent = currentNode->parent;
        currentNode = currentNode->sibling;
        currentNode->serializeType = 1;

        uint64_t typeSymbol;
        fread(&typeSymbol, sizeof(typeSymbol), 1, stream);

        const struct MetaClassDescription *description = getMetaClassDescriptionBySymbol(typeSymbol);
        currentNode->description = description;
        currentNode->description->read(stream, currentNode, flags);
    }

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;
    currentNode->description = getMetaClassDescriptionByIndex(bool_type);
    currentNode->description->read(stream, currentNode, flags);

    return 0;
}

int Map_Symbolboolless_Symbol__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericMapRead(stream, node, flags, getMetaClassDescriptionByIndex(Symbol), getMetaClassDescriptionByIndex(bool_type));
}

int Map_Symbolintless_Symbol__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericMapRead(stream, node, flags, getMetaClassDescriptionByIndex(Symbol), getMetaClassDescriptionByIndex(int_type));
}

int LogicItemRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "prop", .metaClassDescriptionIndex = PropertySet},
        {.isBlocked = 1, .memberName = "mName", .metaClassDescriptionIndex = String},
        {.isBlocked = 1, .memberName = "mKeyNegateList", .metaClassDescriptionIndex = Map_Symbolboolless_Symbol__},
        {.isBlocked = 1, .memberName = "mKeyComparisonList", .metaClassDescriptionIndex = Map_Symbolintless_Symbol__},
        {.isBlocked = 1, .memberName = "mKeyActionList", .metaClassDescriptionIndex = Map_Symbolintless_Symbol__},
        {.isBlocked = 1, .memberName = "mReferenceKeyList", .metaClassDescriptionIndex = DCArray_String_},
    };
    return genericRead(stream, node, flags, 6, descriptions);
}

int Map_StringLogicGroup__LogicItemless_String__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericMapRead(stream, node, flags, getMetaClassDescriptionByIndex(String), getMetaClassDescriptionByIndex(LogicGroup__LogicItem));
}

int DCArray_LogicGroup_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(LogicGroup));
}

int LogicGroupRead(FILE *stream, struct TreeNode *node, uint32_t flags) // TODO: Fix to allow general write function
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 0, .memberName = "operator", .metaClassDescriptionIndex = int_type},
        {.isBlocked = 1, .memberName = "mItems", .metaClassDescriptionIndex = Map_StringLogicGroup__LogicItemless_String__},
        {.isBlocked = 1, .memberName = "mLogicGroups", .metaClassDescriptionIndex = DCArray_LogicGroup_},
        {.isBlocked = 0, .memberName = "mGroupOperator", .metaClassDescriptionIndex = int_type},
        {.isBlocked = 0, .memberName = "mType", .metaClassDescriptionIndex = int_type},
        {.isBlocked = 1, .memberName = "mName", .metaClassDescriptionIndex = String},
    };
    return genericRead(stream, node, flags, 6, descriptions);
}

int RuleRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "mName", .metaClassDescriptionIndex = String},
        {.isBlocked = 1, .memberName = "mRuntimePropName", .metaClassDescriptionIndex = String},
        {.isBlocked = 0, .memberName = "flags", .metaClassDescriptionIndex = Flags},
        {.isBlocked = 1, .memberName = "mConditions", .metaClassDescriptionIndex = LogicGroup},
        {.isBlocked = 1, .memberName = "mActions", .metaClassDescriptionIndex = LogicGroup},
        {.isBlocked = 1, .memberName = "mElse", .metaClassDescriptionIndex = LogicGroup},
        {.isBlocked = 1, .memberName = "mAgentCategory", .metaClassDescriptionIndex = String},
    };
    return genericRead(stream, node, flags, 7, descriptions);
}

int DlgDownstreamVisibilityConditionsRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 0, .memberName = "mNodeTypeFlags", .metaClassDescriptionIndex = Flags},
        {.isBlocked = 0, .memberName = "mMaxNumNodeEvals", .metaClassDescriptionIndex = int_type},
    };
    return genericRead(stream, node, flags, 2, descriptions);
}

int DlgNodeChoreRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "mChore", .metaClassDescriptionIndex = Handle_Chore_},
        {.isBlocked = 0, .memberName = "mPriority", .metaClassDescriptionIndex = int_type},
        {.isBlocked = 0, .memberName = "mLooping", .metaClassDescriptionIndex = bool_type},
        {.isBlocked = 1, .memberName = "Baseclass_DlgNode", .metaClassDescriptionIndex = DlgNode},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int DlgVisibilityConditionsRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    uint32_t visCondFlags;
    cfseek(stream, sizeof(uint8_t), SEEK_CUR);
    fread(&visCondFlags, sizeof(visCondFlags), 1, stream);
    cfseek(stream, -(int64_t)(sizeof(visCondFlags) + sizeof(uint8_t)), SEEK_CUR);

    node->child = calloc(1, sizeof(struct TreeNode));
    struct TreeNode *currentNode = node->child;
    currentNode->parent = node;
    currentNode->memberName = "mbDiesOff";
    currentNode->description = getMetaClassDescriptionByIndex(bool_type);
    currentNode->description->read(stream, currentNode, flags);

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;
    currentNode->memberName = "mFlags";
    currentNode->description = getMetaClassDescriptionByIndex(Flags);
    currentNode->description->read(stream, currentNode, flags);

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;
    currentNode->isBlocked = 1;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    currentNode->memberName = "mDownstreamVisCond";
    currentNode->description = getMetaClassDescriptionByIndex(DlgDownstreamVisibilityConditions);
    currentNode->description->read(stream, currentNode, flags);

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;
    currentNode->isBlocked = 1;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    currentNode->memberName = "mScriptVisCond";
    currentNode->description = getMetaClassDescriptionByIndex(String);
    currentNode->description->read(stream, currentNode, flags);
    if (*(uint32_t *)currentNode->parent->child->sibling->staticBuffer & 1)
    {
        currentNode->sibling = calloc(1, sizeof(struct TreeNode));
        currentNode->sibling->parent = currentNode->parent;
        currentNode = currentNode->sibling;
        currentNode->memberName = "mpRule";
        currentNode->description = getMetaClassDescriptionByIndex(Rule);
        currentNode->description->read(stream, currentNode, flags);
    }
    return 0;
}

int DlgVisibilityConditionsOwnerRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "mVisCond", .metaClassDescriptionIndex = DlgVisibilityConditions},
    };
    return genericRead(stream, node, flags, 1, descriptions);
}

int DlgObjectPropsRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->child = calloc(1, sizeof(struct TreeNode));
    struct TreeNode *currentNode = node->child;
    currentNode->description = getMetaClassDescriptionByIndex(Flags);
    currentNode->description->read(stream, currentNode, flags);
    currentNode->parent = node;

    uint32_t propFlags = *(uint32_t *)(currentNode->staticBuffer);
    uint32_t propCount = ((propFlags & 0x1) > 0) + ((propFlags & 0x2) > 0) + ((propFlags & 0x4) > 0);

    for (uint32_t i = 0; i < propCount; ++i)
    {
        currentNode->sibling = calloc(1, sizeof(struct TreeNode));
        currentNode->sibling->parent = currentNode->parent;
        currentNode = currentNode->sibling;

        currentNode->description = getMetaClassDescriptionByIndex(PropertySet);
        currentNode->description->read(stream, currentNode, flags);
    }

    return 0;
}
int DlgObjectPropsOwnerRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "mDlgObjectProps", .metaClassDescriptionIndex = DlgObjectProps},
    };
    return genericRead(stream, node, flags, 1, descriptions);
}

int DlgObjIDOwnerRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "mDlgObjID", .metaClassDescriptionIndex = DlgObjID},
    };
    return genericRead(stream, node, flags, 1, descriptions);
}

int DlgNodeLinkRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_DlgObjIDOwner", .metaClassDescriptionIndex = DlgObjIDOwner},
        {.isBlocked = 0, .memberName = "mRequiredCCType", .metaClassDescriptionIndex = long_type}, // blocked is wrong?
    };
    return genericRead(stream, node, flags, 2, descriptions);
}

int DlgNodeRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_DlgObjIDOwner", .metaClassDescriptionIndex = DlgObjIDOwner},
        {.isBlocked = 1, .memberName = "Baseclass_DlgVisibilityConditionsOwner", .metaClassDescriptionIndex = DlgVisibilityConditionsOwner},
        {.isBlocked = 1, .memberName = "Baseclass_DlgObjectPropsOwner", .metaClassDescriptionIndex = DlgObjectPropsOwner},
        {.isBlocked = 1, .memberName = "mPrev", .metaClassDescriptionIndex = DlgNodeLink},
        {.isBlocked = 1, .memberName = "mNext", .metaClassDescriptionIndex = DlgNodeLink},
        {.isBlocked = 0, .memberName = "mName", .metaClassDescriptionIndex = Symbol},
        {.isBlocked = 0, .memberName = "mFlags", .metaClassDescriptionIndex = Flags},
        {.isBlocked = 0, .memberName = "mChainContextTypeID", .metaClassDescriptionIndex = long_type},
    };
    return genericRead(stream, node, flags, 8, descriptions);
}

int DlgNodeTextRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_DlgNode", .metaClassDescriptionIndex = DlgNode},
        {.isBlocked = 1, .memberName = "mLangResProxy", .metaClassDescriptionIndex = LanguageResProxy},
    };
    return genericRead(stream, node, flags, 2, descriptions);
}

int DlgNodeLogicRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_DlgNode", .metaClassDescriptionIndex = DlgNode},
        {.isBlocked = 1, .memberName = "mRule", .metaClassDescriptionIndex = Rule},
    };
    return genericRead(stream, node, flags, 2, descriptions);
}

int DlgNodeScriptRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_DlgNode", .metaClassDescriptionIndex = DlgNode},
        {.isBlocked = 1, .memberName = "mScriptText", .metaClassDescriptionIndex = String},
        {.isBlocked = 0, .memberName = "mbBlocking", .metaClassDescriptionIndex = bool_type},
        {.isBlocked = 0, .memberName = "mbExecuteOnInstanceRetire", .metaClassDescriptionIndex = bool_type},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int DlgNodeNotesRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_DlgNode", .metaClassDescriptionIndex = DlgNode},
        {.isBlocked = 1, .memberName = "mNoteText", .metaClassDescriptionIndex = String},
    };
    return genericRead(stream, node, flags, 2, descriptions);
}

int DlgNodeStartRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_DlgNode", .metaClassDescriptionIndex = DlgNode},
        {.isBlocked = 1, .memberName = "mProdReportProps", .metaClassDescriptionIndex = PropertySet},
    };
    return genericRead(stream, node, flags, 2, descriptions);
}

int DlgNodeJumpRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_DlgNode", .metaClassDescriptionIndex = DlgNode},
        {.isBlocked = 1, .memberName = "jumpToLink", .metaClassDescriptionIndex = DlgNodeLink},
        {.isBlocked = 0, .memberName = "jumpToName", .metaClassDescriptionIndex = Symbol},
        {.isBlocked = 0, .memberName = "jumpTargetClass", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 0, .memberName = "jumpBehaviour", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 0, .memberName = "visibilityBehaviour", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 0, .memberName = "choiceTransparency", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 1, .memberName = "jumpToDlgHandle", .metaClassDescriptionIndex = Handle_Dlg_},
    };
    return genericRead(stream, node, flags, 8, descriptions);
}

int DlgChainHeadRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_DlgObjIDOwner", .metaClassDescriptionIndex = DlgObjIDOwner},
        {.isBlocked = 1, .memberName = "mLink", .metaClassDescriptionIndex = DlgNodeLink},
    };
    return genericRead(stream, node, flags, 2, descriptions);
}

int DlgChildRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_DlgChainHead", .metaClassDescriptionIndex = DlgChainHead},
        {.isBlocked = 0, .memberName = "mName", .metaClassDescriptionIndex = Symbol},
        {.isBlocked = 1, .memberName = "Baseclass_DlgVisibilityConditionsOwner", .metaClassDescriptionIndex = DlgVisibilityConditionsOwner},
        {.isBlocked = 1, .memberName = "Baseclass_DlgObjectPropsOwner", .metaClassDescriptionIndex = DlgObjectPropsOwner},
        {.isBlocked = 1, .memberName = "mParent", .metaClassDescriptionIndex = DlgNodeLink},
    };
    return genericRead(stream, node, flags, 5, descriptions);
}

int DlgNodeIdleRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_DlgNode", .metaClassDescriptionIndex = DlgNode},
        {.isBlocked = 1, .memberName = "mhIdle", .metaClassDescriptionIndex = Handle_Chore_},
        {.isBlocked = 0, .memberName = "mOverrideOptionTime", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 0, .memberName = "mOverrideOptionStyle", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 0, .memberName = "mTransitionTimeOverride", .metaClassDescriptionIndex = float_type},
        {.isBlocked = 0, .memberName = "mTransitionStyleOverride", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 0, .memberName = "mIdleSlot", .metaClassDescriptionIndex = long_type},
    };
    return genericRead(stream, node, flags, 7, descriptions);
}

int DlgNodeMarkerRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_DlgNode", .metaClassDescriptionIndex = DlgNode},
    };
    return genericRead(stream, node, flags, 1, descriptions);
}

int DlgNodeExitRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DlgNodeMarkerRead(stream, node, flags);
}

int DlgLineRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_UID::Generator", .metaClassDescriptionIndex = UID__Generator},
        {.isBlocked = 1, .memberName = "Baseclass_DlgObjIDOwner", .metaClassDescriptionIndex = DlgObjIDOwner},
        {.isBlocked = 1, .memberName = "mLangResProxy", .metaClassDescriptionIndex = LanguageResProxy},
    };
    return genericRead(stream, node, flags, 3, descriptions);
}

int Map_intDlgLineless_int__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericMapRead(stream, node, flags, getMetaClassDescriptionByIndex(int_type), getMetaClassDescriptionByIndex(DlgLine));
}

int DlgLineCollectionRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_UID::Generator", .metaClassDescriptionIndex = UID__Generator},
        {.isBlocked = 1, .memberName = "mLines", .metaClassDescriptionIndex = Map_intDlgLineless_int__},
    };
    return genericRead(stream, node, flags, 2, descriptions);
}

int DCArray_DlgNodeExchange__Entry_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(DlgNodeExchange__Entry));
}

int DlgNodeExchangeRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->child = calloc(1, sizeof(struct TreeNode));
    struct TreeNode *currentNode = node->child;
    currentNode->parent = node;
    currentNode->memberName = "mPriority";
    currentNode->description = getMetaClassDescriptionByIndex(float_type);
    currentNode->description->read(stream, currentNode, flags);

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;
    currentNode->isBlocked = 1;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    currentNode->memberName = "mhChore";
    currentNode->description = getMetaClassDescriptionByIndex(Handle_Chore_);
    currentNode->description->read(stream, currentNode, flags);

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;
    currentNode->isBlocked = 1;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    currentNode->memberName = "Baseclass_DlgNode";
    currentNode->description = getMetaClassDescriptionByIndex(DlgNode);
    currentNode->description->read(stream, currentNode, flags);

    uint32_t nodeFlags = *((uint32_t *)(currentNode->child->sibling->sibling->sibling->sibling->sibling->sibling->staticBuffer));

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;
    currentNode->isBlocked = 1;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    currentNode->memberName = "mEntries";
    currentNode->description = getMetaClassDescriptionByIndex(DCArray_DlgNodeExchange__Entry_);
    currentNode->description->read(stream, currentNode, flags);

    int64_t tell = cftell(stream);
    if (nodeFlags & 0x1)
    {
        printf("Error: NoteCollection Read not implemented"); // TODO: Implement this, but wait until you run into a file that uses a note collection before trying to implement it.
        return -1;
    }

    if (nodeFlags & 0x2)
    {
        currentNode->sibling = calloc(1, sizeof(struct TreeNode));
        currentNode->sibling->parent = currentNode->parent;
        currentNode = currentNode->sibling;
        currentNode->memberName = "mpLines";
        currentNode->description = getMetaClassDescriptionByIndex(DlgLineCollection);
        currentNode->description->read(stream, currentNode, flags);
    }
    tell = cftell(stream);
    return 0;
}

int DlgConditionRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "mDlgObjID", .metaClassDescriptionIndex = DlgObjIDOwner},
    };
    return genericRead(stream, node, flags, 1, descriptions);
}

int DlgConditionRuleRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "BaseClass_DlgCondition", .metaClassDescriptionIndex = DlgCondition},
        {.isBlocked = 1, .memberName = "mRule", .metaClassDescriptionIndex = Rule},
    };
    return genericRead(stream, node, flags, 2, descriptions);
}

int DlgConditionTimeRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_DlgCondition", .metaClassDescriptionIndex = DlgCondition},
        {.isBlocked = 0, .memberName = "mDurationClass", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 0, .memberName = "mSeconds", .metaClassDescriptionIndex = float_type},
    };
    return genericRead(stream, node, flags, 3, descriptions);
}

int DlgConditionInputRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_DlgCondition", .metaClassDescriptionIndex = DlgCondition},
    };
    return genericRead(stream, node, flags, 1, descriptions);
}

int DlgConditionSetRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->child = calloc(1, sizeof(struct TreeNode));
    struct TreeNode *currentNode = node->child;
    currentNode->parent = node;
    currentNode->memberName = "mConditions";

    currentNode->child = calloc(1, sizeof(struct TreeNode));
    currentNode->child->parent = currentNode;
    currentNode = currentNode->child;
    currentNode->description = getMetaClassDescriptionByIndex(int_type);
    currentNode->description->read(stream, currentNode, flags);

    for (uint32_t i = 0; i < *(uint32_t *)node->child->child->staticBuffer; ++i)
    {
        currentNode->sibling = calloc(1, sizeof(struct TreeNode));
        currentNode->sibling->parent = currentNode->parent;
        currentNode = currentNode->sibling;
        currentNode->serializeType = 1;
        uint64_t typeSymbol;
        fread(&typeSymbol, sizeof(typeSymbol), 1, stream);
        currentNode->description = getMetaClassDescriptionBySymbol(typeSymbol);
        currentNode->description->read(stream, currentNode, flags);
    }

    return 0;
}

int DlgNodeCancelChoicesRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_DlgNode", .metaClassDescriptionIndex = DlgNode},
        {.isBlocked = 0, .memberName = "mCancelGroup", .metaClassDescriptionIndex = long_type},
    };
    return genericRead(stream, node, flags, 2, descriptions);
}

int DlgNodeWaitRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_DlgNode", .metaClassDescriptionIndex = DlgNode},
        {.isBlocked = 1, .memberName = "Baseclass_DlgConditionSet", .metaClassDescriptionIndex = DlgConditionSet},
    };
    return genericRead(stream, node, flags, 2, descriptions);
}

int DCArray_Ptr_DlgChild__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
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
        currentNode->serializeType = 1;

        uint64_t typeSymbol;
        fread(&typeSymbol, sizeof(typeSymbol), 1, stream);

        currentNode->description = getMetaClassDescriptionBySymbol(typeSymbol);
        currentNode->parent = node;
        currentNode->description->read(stream, currentNode, flags);
    }
    return 0;
}

int DlgChildSetRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->child = calloc(1, sizeof(struct TreeNode));
    node->child->parent = node;
    node->child->description = getMetaClassDescriptionByIndex(DCArray_Ptr_DlgChild__);
    return node->child->description->read(stream, node->child, flags);
}

int DlgChildSetChoiceRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_DlgChildSet", .metaClassDescriptionIndex = DlgChildSet},
    };
    return genericRead(stream, node, flags, 1, descriptions);
}

int DlgChildSetChoicesChildPreRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DlgChildSetChoiceRead(stream, node, flags);
}

int DlgChildSetChoicesChildPostRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DlgChildSetChoiceRead(stream, node, flags);
}

int DlgNodeChoicesRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_DlgNode", .metaClassDescriptionIndex = DlgNode},
        {.isBlocked = 1, .memberName = "mChoices", .metaClassDescriptionIndex = DlgChildSetChoice},
        {.isBlocked = 1, .memberName = "mPreChoice", .metaClassDescriptionIndex = DlgChildSetChoicesChildPre},
        {.isBlocked = 1, .memberName = "mPostChoice", .metaClassDescriptionIndex = DlgChildSetChoicesChildPost},
    };
    return genericRead(stream, node, flags, 4, descriptions);
}

int DlgFolderChildRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_DlgChild", .metaClassDescriptionIndex = DlgChild},
    };
    return genericRead(stream, node, flags, 1, descriptions);
}

int DlgChoicesChildPreRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DlgFolderChildRead(stream, node, flags);
}

int DlgChoicesChildPostRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DlgFolderChildRead(stream, node, flags);
}

int DlgChoiceRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_DlgChild", .metaClassDescriptionIndex = DlgChild},
        {.isBlocked = 1, .memberName = "Baseclass_DlgConditionSet", .metaClassDescriptionIndex = DlgConditionSet},
    };
    return genericRead(stream, node, flags, 2, descriptions);
}

int DlgNodeParallelPElementRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DlgFolderChildRead(stream, node, flags);
}

int DlgConditionalCaseRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DlgFolderChildRead(stream, node, flags);
}

int DlgNodeCriteriaRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "mTestType", .metaClassDescriptionIndex = DlgNodeCriteria__EnumTestT},
        {.isBlocked = 1, .memberName = "mFlagsThreshold", .metaClassDescriptionIndex = DlgNodeCriteria__EnumThresholdT},
        {.isBlocked = 1, .memberName = "mCriteriaThreshold", .metaClassDescriptionIndex = DlgNodeCriteria__EnumThresholdT},
        {.isBlocked = 1, .memberName = "mDefaultResult", .metaClassDescriptionIndex = DlgNodeCriteria__EnumDefaultResultT},
        {.isBlocked = 0, .memberName = "mClassFlags", .metaClassDescriptionIndex = Flags},
        {.isBlocked = 1, .memberName = "mClassIDs", .metaClassDescriptionIndex = Set_intless_int__},
    };
    return genericRead(stream, node, flags, 6, descriptions);
}

int DlgChildSetElementRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_DlgChildSet", .metaClassDescriptionIndex = DlgChildSet},
    };
    return genericRead(stream, node, flags, 1, descriptions);
}

int DlgNodeSequenceElementRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_DlgChild", .metaClassDescriptionIndex = DlgChild},
        {.isBlocked = 0, .memberName = "mRepeat", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 0, .memberName = "mPlayPosition", .metaClassDescriptionIndex = long_type},
    };
    return genericRead(stream, node, flags, 3, descriptions);
}

int DlgNodeSequenceRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_DlgNode", .metaClassDescriptionIndex = DlgNode},
        {.isBlocked = 1, .memberName = "mElements", .metaClassDescriptionIndex = DlgNodeSequence__DlgChildSetElement},
        {.isBlocked = 0, .memberName = "mPlaybackMode", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 0, .memberName = "mLifetimeMode", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 1, .memberName = "mElemUseCriteria", .metaClassDescriptionIndex = DlgNodeCriteria},
    };
    return genericRead(stream, node, flags, 5, descriptions);
}

int DlgNodeParallelRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_DlgNode", .metaClassDescriptionIndex = DlgNode},
        {.isBlocked = 1, .memberName = "mPElements", .metaClassDescriptionIndex = DlgNodeParallel__DlgChildSetElement},
        {.isBlocked = 1, .memberName = "mElemUseCriteria", .metaClassDescriptionIndex = DlgNodeCriteria},
    };
    return genericRead(stream, node, flags, 5, descriptions);
}

int DlgChildSetConditionalCaseRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DlgChildSetElementRead(stream, node, flags);
}

int DlgNodeConditionalRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "Baseclass_DlgNode", .metaClassDescriptionIndex = DlgNode},
        {.isBlocked = 1, .memberName = "mCases", .metaClassDescriptionIndex = DlgChildSetConditionalCase},
    };
    return genericRead(stream, node, flags, 2, descriptions);
}
