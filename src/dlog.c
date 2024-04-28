#include <inttypes.h>
#include "string.h"
#include "landb.h"
#include "types.h"
#include "tree.h"
#include "crc64.h"

/*
env_airportExterior.dlog
0x2D8 is end of Metastream
0x359 is end of landb (B1) !
0x38A is end of prop (04) !
0x3C3 is start of first child (2C) !
0x3EF is end of first child chain head (00) !
0x448 is start of second child (0x2C) !
0x6D5 is end of first childset (Symbol) !
0x1093 is end of second folder
0x11E0 is end of third folder
0x1543 is end of folders (7E) !
0x631e is prev node death (DlgNodeStart)
Attempted change (address = 0x3c60, nodeIDprev = 0xa98446baf288280f, change nodeID to 834d722d0c22174d) (Changing address 0x34a0) Result is nothing, but upon quitting removing the mod and reloading the save, the save became stuck.
Attempted change (Change all death references (7ac1df47b0ea45af and a98446baf288280f) to 0x834d722d0c22174d) Result = Upon death, you instead get a new attempt.
Attempted change (Change all death prev references to 37b2d8cb291b9b0f) //In hindsight, that was somewhat stupid. I should be changing nodes with next. So let's do that // Though a node with a death node as its prev node seems really confusing (Unless paradise exists (or hell))
Attempted change: Same as above, but next references. (This led to the game stalling)
So it seems like I can't simply "jump" to a specific node by just changing the node link as I theorized. This is more annoying than I thought. Or perhaps I need to jump to the correct node?
Let's try jumping to a DlgNodeStart: 0x6AAA is a DlgNodeStart (Id = 0x597e6e09efab41a5). Setting all next death references to that address. Result = The game stalled for the first death in the scene, but upon the second death things just reset.
There is a potentially identifiable node around 0x6a80, contains a script for a checkpoint
At around 0x15b0 there is a checkpoint for "cs_approachPrescott" (ID = 0xa50b93addf206925).

Current understanding: Child is start of a chain of nodes. The first node (after child) will have its prev point to the chain head ID and the chain head link will point to the first node.
addr = 0x1910, 0x1820 <- Remember to check this later.





*/

struct DependencyLoader
{
    uint8_t hasData;
    uint64_t typeSymbol;
    void *symbolTypeData;
};

struct DlgDownstreamVisibilityConditions
{
    uint32_t flags;
    int32_t maxNumNodeEvals; // Normally -999
};

struct DlgVisibilityConditions
{
    uint8_t diesOff;
    uint32_t flags;
    uint32_t downStreamConditionsBlock; // Always equal to 0x0C
    struct DlgDownstreamVisibilityConditions downStreamConditions;
    uint32_t scriptBlock;
    struct String scriptVisCond; // lua code
    struct Rule rule;            // if flags & 1
};

struct DlgVisibilityConditionsOwner
{
    uint32_t conditionsBlock;
    struct DlgVisibilityConditions conditions;
};

struct DlgObjectID // 0x8 (8) bytes
{
    uint64_t symbol;
};

struct DlgObjectIDOwner // 0x0C (12) bytes
{
    uint32_t block; // Always equal to 0x0C
    struct DlgObjectID id;
};

struct DlgObjectProps
{
    uint32_t flags; // Enum (DlgObjectProps::PropsTypeT) = eUserProps = 0x1, eProductionProps = 0x2, eToolProps = 0x4
};

struct DlgObjectPropsOwner
{
    uint32_t block; // Always equal to 0x08 (Nope that is wrong)
    struct DlgObjectProps objectProps;
};

struct DlgNodeLink // 0x14 (20) bytes // Block = 0x18
{
    uint32_t idBlock;
    struct DlgObjectIDOwner id;
    uint32_t requiredChainContextType; // eCCUnspecified = 0x1, eCCAction = 0x2, eCCData = 0x3
};

struct DlgChainHead // 0x28 (40) bytes // Block = 0x2C
{
    uint32_t idBlock; // Always equal to 0x10
    struct DlgObjectIDOwner id;
    uint32_t linkBlock; // Always equal to 0x18
    struct DlgNodeLink link;
};

struct DlgChild
{
    uint32_t chainHeadBlock; // Always equal tp 0x2C
    struct DlgChainHead chainHead;
    uint64_t nameSymbol;
    uint32_t visCondOwnerBlock;
    struct DlgVisibilityConditionsOwner visCondOwner;
    uint32_t objectPropsOwnerBlock; // Always equal to 0x0C
    struct DlgObjectPropsOwner objectPropsOwner;
    uint32_t nodeLinkBlock; // Always equal to 0x18
    struct DlgNodeLink parent;
};

struct DlgChildSymbol
{
    uint64_t childSymbol;
    uint32_t childBlock;
    struct DlgChild child;
};

struct DlgChildSet
{
    uint32_t childCount;
    struct DlgChildSymbol *children;
};

struct DlgFolderChild // 0x7bdcf5ceaf476dbb
{
    struct DlgChild child;
};

struct DlgCondition
{
    struct DlgObjectIDOwner objectIDOwner;
};

struct DlgConditionInput // 0x8e47b65968d5f15f
{
    struct DlgCondition condition;
};

struct DlgConditionSet
{
    struct SymbolDlgConditionPair
    {
        uint64_t conditionTypeSymbol;
        struct DlgCondition condition; // Type is conditionTypeSymbol
    };
    uint32_t conditionsBlock;
    uint32_t conditionCount;
    struct SymbolDlgConditionPair *conditions;
};

struct DlgConditionalCase
{
    struct DlgChild child;
};

struct DlgChoice // 0x98d5fd53a80e7c13
{
    struct DlgChild child;
    struct DlgConditionSet conditionSet;
};

struct DlgConditionRule // 0xfd444bb730c2d4f6
{
};

struct DlgNode
{
    uint32_t idOwnerBlock; // 0x10
    struct DlgObjectIDOwner idOwner;
    uint32_t visCondOwnerBlock;
    struct DlgVisibilityConditionsOwner visCondOwner;
    uint32_t propOwnerBlock; // 0x0C
    struct DlgObjectPropsOwner propOwner;
    uint32_t prevBlock; // 0x18
    struct DlgNodeLink prev;
    uint32_t nextBlock; // 0x18
    struct DlgNodeLink next;
    uint64_t nameSymbol;
    uint32_t flags;
    uint32_t chainContextTypeID;
};

struct DlgNodeConditional
{
    struct DlgChildSetConditionalCase
    {
        struct DlgChildSet childSet;
    };
    struct DlgNode node;
    uint32_t casesBlock;
    struct DlgChildSetConditionalCase cases;
};

struct DlgChildSetChoicesChildPost // 0x171bdf4066e69130
{
    uint32_t childSetBlock;
    struct DlgChildSet childSet;
};

struct DlgChildSetChoicesChildPre // 0xe985d67eccd73fee
{
    uint32_t childSetBlock;
    struct DlgChildSet childSet;
};

struct DlgChildSetChoice
{
    uint32_t childSetBlock;
    struct DlgChildSet childSet;
};

struct DlgNodeChoices // 0x9134de0b6f8d9913
{
    struct DlgNode node;
    uint32_t choiceBlock;
    struct DlgChildSetChoice choice;
    uint32_t preChoiceBlock;
    struct DlgChildSetChoicesChildPre preChoice;
    uint32_t postChoiceBlock;
    struct DlgChildSetChoicesChildPost postChoice;
};

struct DlgNodeJump // 0x987d701bcf0be72e
{
    struct DlgNode node;
    uint32_t jumpToLinkBlock;
    struct DlgNodeLink jumpToLink;
    uint64_t jumpToNameSymbol;
    uint32_t jumpTargetClass;     // Enum: eToNae = 0x1, eToParent = 0x2, eToNodeAfterParentWaitNode = 0x3 }
    uint32_t jumpBehaviour;       // Enum: eJumpAndExecute = 0x1, eJumpExecuteAndReturn = 0x2, eReturn = 0x3 }
    uint32_t visibilityBehaviour; // Enum: eIgnoreVisibility = 0x1, eObeyVisibility = 0x2 }
    uint32_t choiceTransparency;
    uint32_t jumpToDlgHandleBlock;
    uint64_t jumpToDlgHandle;
};

struct DlgNodeIdle // 0x8d8b0107aea7a818
{
    struct DlgNode node;
    uint32_t choreHandleBlock;
    uint64_t choreHandle;
    uint32_t overrideOption;      // eUseDefaults = 1, eOverride = 2
    uint32_t overrideOptionStyle; // eUseDefaults = 1, eOverride = 2
    float transitionTimeOverride;
    uint32_t transitionStyleOverride;
    uint32_t idleSlot;
};

struct DlgNodeCriteria // 0x3b3c5055276b4321 // Unfinished
{
    uint32_t testType;          // required = 1, forbidden = 2
    uint32_t flagThreshold;     // Enum: render = 0, capture = 1, all = 2, dataFlow_enum_count = 3
    uint32_t criteriaThreshold; // Enum: render = 0, capture = 1, all = 2, dataFlow_enum_count = 3
    uint32_t defaultResult;     // Enum: defaultToPass = 0x1, defaultToNotPass = 0x2, defaultToNotPassUnlessTransparent = 0x3
    uint32_t classFlags;
    void *classIDs;
};

struct DlgNodeExchange // 0x979ec6c0f454d985
{
    float priority; // 9900
    uint32_t choreHandleBlock;
    uint64_t choreHandle;
    struct DlgNode node;
};

struct DlgNodeSequence // 0x59f8b5e15f177d70 // Unfinished
{
    struct DlgNode node;
    uint32_t elementsBlock;
    struct DlgChildSet elements;
    uint32_t playbackMode; // sequential = 1, shuffle = 2
    uint32_t lifetimeMode; // looping = 1, singleSequence = 2, singleSequenceRepeatFinal = 3
    uint32_t elementUseCriteriaBlock;
    struct DlgNodeCriteria elementUseCriteria;
};

struct DlgNodeMarker // 0x62398665690a223b
{
    struct DlgNode node;
};

struct DlgNodeWait
{
};

struct SymbolBoolPair
{
    uint64_t symbol;
    uint8_t negate;
};

struct SymbolIntPair
{
    uint64_t symbol;
    uint32_t num;
};

struct LogicItem
{
    struct PropertySet prop;
    uint32_t nameBlock;
    struct String name;
    uint32_t keyNegateListBlock;
    uint32_t keyNegateCount;
    struct SymbolBoolPair *keyNegateList;
    uint32_t keyComparisonListBlock;
    uint32_t keyComparisonCount;
    struct SymbolIntPair *keyComparisonList;
    uint32_t keyActionListBlock;
    uint32_t keyActionCount;
    struct SymbolIntPair *keyActionList;
    uint32_t referenceKeyListBlock;
    uint32_t referenceKeyCount;
    struct String *referenceKeyList;
};

struct StringLogicItemPair
{
    struct String string;
    struct LogicItem item;
};

struct LogicGroup
{
    uint32_t operator;
    uint32_t itemsBlock;
    uint32_t itemsCount;
    struct StringLogicItemPair *items;
    uint32_t logicGroupBlock;
    uint32_t logicGroupCount;
    struct LogicGroup *logicGroups;
    uint32_t groupOperator;
    uint32_t type;
    uint32_t nameBlock;
    struct String name;
};
// 001be489
// addr = 0x665fd9
// 824462
struct Rule
{
    uint32_t nameBlock;
    struct String name;
    uint32_t runtimePropNameBlock;
    struct String runtimePropName;
    uint32_t flags;
    uint32_t conditionsBlock;
    struct LogicGroup conditions;
    uint32_t actionsBlock;
    struct LogicGroup actions;
    uint32_t elseBlock;
    struct LogicGroup nullptr; // You seriously called a variable "else". I will one up you telltale and call my variable "nullptr" and that is valid code since I am using a superior language. For the record the variable is called "mElse"
    uint32_t agentCategoryBlock;
    struct String agentCategory;
};

struct DlgNodeLogic // 0xcead330a880c7ed9
{
    struct DlgNode node;
    uint32_t ruleBlock;
    struct Rule rule;
};

struct DlgNodeStart // 0x2a8b64c603022291
{
    struct DlgNode node;
    uint32_t prodReportPropBlock;
    struct PropertySet prodReportProp;
};

struct DlgNodeNotes // 0xd80d7ae46db975c4
{
    struct DlgNode node;
    uint32_t noteTextBlock;
    struct String noteText;
};

struct DlgNodeExit
{
};

struct DlgNodeScript // 0xb6dc854d60a36ce0 (big endian)
{
    struct DlgNode node;
    uint32_t scriptBlock;
    struct String luaScript;
    uint8_t blocking;
    uint8_t executeOnInstanceRetire;
};

struct DlgNodeStart // 0x2a8b64c603022291
{
    struct DlgNode node;
    uint32_t propBlock;
    struct PropertySet prodReportProp;
};

struct DlgNodeSymbol
{
    uint64_t typeSymbol;
    uint32_t nodeBlock;
    struct DlgNode node;
};

struct DlgFolder
{
    uint32_t IDblock;
    struct DlgObjectIDOwner IDowner;
    uint32_t objectPropsBlock;
    struct DlgObjectPropsOwner propsOwner;
    uint32_t childSetBlock;
    struct DlgChildSet childSet;
    uint64_t nameSymbol;
    uint32_t prodReportPropBlock;
    struct PropertySet prodReportProp;
};

struct Dlg
{
    uint32_t idOwnerBlock;
    struct DlgObjectIDOwner idOwner;
    uint32_t stringBlock;
    struct String name;
    uint32_t version;
    uint32_t idBlock;
    struct DlgObjectID id;
    uint32_t langBlock;
    struct LanguageDB db;
    uint32_t projectID;
    uint64_t resourceLocationID;
    uint32_t chronolgy;
    uint32_t flags;
    uint32_t dependencyBlock;
    struct DependencyLoader dependencyLoader;
    uint32_t propBlock;
    struct PropertySet prodReportProps;
    uint32_t jiraBlock; // Always equal to 4. Turn on the emergency alarms if it is not.
    uint8_t hasToolOnlyData;
    uint32_t folderCount;
    struct DlgFolder *folders;
    uint32_t nodeCount;
    struct DlgNodeSymbol *nodes;
};

size_t blockRead(FILE *stream, uint8_t *buffer)
{
    size_t bytesRead = 0;
    bytesRead += fread(buffer, sizeof(uint32_t), 1, stream);
    buffer += bytesRead;
    bytesRead += fread(buffer, (*(uint32_t *)(buffer - sizeof(uint32_t)) - 4), 1, stream);
    return bytesRead;
}

void dlgRead(FILE *stream, uint8_t *headerBuffer, struct TreeNode *nodes)
{
    headerBuffer += blockRead(stream, headerBuffer); // idOwner
    headerBuffer += blockRead(stream, headerBuffer); // string
    headerBuffer += fread(headerBuffer, sizeof(uint32_t), 1, stream);
    headerBuffer += blockRead(stream, headerBuffer); // objectID
    headerBuffer += blockRead(stream, headerBuffer); // landb
    headerBuffer += fread(headerBuffer, 20, 1, stream);
    headerBuffer += blockRead(stream, headerBuffer); // dependency
    headerBuffer += blockRead(stream, headerBuffer); // prop
    headerBuffer += blockRead(stream, headerBuffer); // Jira
    headerBuffer += fread(headerBuffer, sizeof(uint8_t), 1, stream);
    uint32_t folderCount = (*(uint32_t *)(headerBuffer - sizeof(uint32_t)));

    struct TreeNode *nodeTree;

    for (uint32_t i = 0; i < folderCount; ++i)
    {
        headerBuffer += blockRead(stream, headerBuffer);
        headerBuffer += blockRead(stream, headerBuffer);
    }

    fseek(stream, 0x1543, SEEK_SET);
    uint32_t nodeCount;
    fread(&nodeCount, sizeof(nodeCount), 1, stream);
    for (uint32_t i = 0; i < nodeCount; ++i)
    {
    }
}

void readChain(struct LinkedListNode *node, FILE *stream)
{
    uint64_t symbol;
    uint32_t size;
    fread(&symbol, sizeof(symbol), 1, stream);
    fread(&size, sizeof(size), 1, stream);
    enum Type type = searchDatabase("./protonDatabase.db", symbol);
}