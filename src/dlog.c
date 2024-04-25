#include <inttypes.h>
#include "string.h"
#include "landb.h"
#include "types.h"
#include "tree.h"

/*
env_airportExterior.dlog
0x2D8 is end of Metastream
0x359 is end of landb (B1) !
0x38A is end of prop (04) !
0x3EF is end of first child chain head (00) !
0x448 is start of second child (0x2C) !
0x6D5 is end of first childset (Symbol) !
0x1093 is end of second folder
0x11E0 is end of third folder
0x1543 is end of folders (7E) !


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
    uint32_t block; // Always equal to 0x08
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

struct DlgFolderChild // 0x7bdcf5ceaf476dbb
{
    struct DlgChild child;
};

struct DlgCondition
{
};

struct DlgConditionSet // 0x98d5fd53a80e7c13
{
    uint32_t conditionCount;
};

struct DlgChoice // 0xe985d67eccd73fee
{
    struct DlgChild child;
    struct DlgConditionSet conditionSet;
};

struct DlgConditionRule // 0xfd444bb730c2d4f6
{
};

struct DlgChoicesChildPre // 0x171bdf4066e69130
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

struct DlgNodeIdle // 0x987d701bcf0be72e
{
};

struct DlgNodeCriteria // 0x7432d6a8b467e336
{
};

struct DlgNodeExchange // 0x5e5c7c866aa411f5
{
};

struct DlgNodeSequence // 0x59f8b5e15f177d70
{
};

struct DlgNodeWait // 0x62398665690a223b
{
};

struct DlgNodeLogic // 0xcead330a880c7ed9
{
};

struct DlgNodeStart // 0x2a8b64c603022291
{
};

struct DlgNodeNotes // 0xd80d7ae46db975c4
{
};

struct DlgNodeExit // 0x8d8b0107aea7a818
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
    headerBuffer += fread(headerBuffer, 4, 1, stream);
    uint32_t folderCount = (*(uint32_t *)(headerBuffer - sizeof(uint32_t)));

    struct TreeNode *trees;

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
