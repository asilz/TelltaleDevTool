#include <inttypes.h>
#include <ttstring.h>

#ifndef DLG_H
#define DLG_H

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

struct DlgFolder
{
    uint32_t IDblock;
    struct DlgObjectIDOwner IDowner;
    uint32_t objectPropsBlock;
    struct DlgObjectPropsOwner propsOwner;
    uint32_t childSetBlock;
    uint32_t childCount;
    struct DlgTreeNode **childSet;
    uint64_t nameSymbol;
    uint32_t prodReportPropBlock;
    void *prodReportProp;
};

struct Dlg
{
    uint32_t idOwnerBlock;
    struct DlgObjectIDOwner idOwner;
    uint32_t stringBlock;
    void *name;
    uint32_t version;
    uint32_t idBlock;
    struct DlgObjectID id;
    uint32_t langBlock;
    struct LanguageDB *db;
    uint32_t projectID;
    uint64_t resourceLocationID;
    uint32_t chronolgy;
    uint32_t flags;
    uint32_t dependencyBlock;
    struct DependencyLoader *dependencyLoader;
    uint32_t propBlock;
    void *prodReportProps;
    uint32_t jiraBlock; // Always equal to 4. Turn on the emergency alarms if it is not.
    uint8_t hasToolOnlyData;
    uint32_t folderCount;
    struct DlgFolder *folders;
    uint32_t nodeCount;
    struct DlgTreeNode **nodes;
};

void DlgRead(FILE *stream, struct Dlg *dlg, uint32_t flags);
int DlgNodeLogicRead(FILE *stream, void **treeNode, uint32_t flags);
int DlgNodeExchangeRead(FILE *stream, void **treeNode, uint32_t flags);
int DlgNodeChoicesRead(FILE *stream, void **treeNode, uint32_t flags);
int DlgChoiceRead(FILE *stream, void **treeNode, uint32_t flags);
int DlgNodeWaitRead(FILE *stream, void **treeNode, uint32_t flags);
int DlgNodeMarkerRead(FILE *stream, void **treeNode, uint32_t flags);
int DlgNodeIdleRead(FILE *stream, void **treeNode, uint32_t flags);
int DlgConditionalCaseRead(FILE *stream, void **treeNode, uint32_t flags);
int DlgNodeScriptRead(FILE *stream, void **treeNode, uint32_t flags);
int DlgNodeSequenceRead(FILE *stream, void **treeNode, uint32_t flags);
int DlgNodeJumpRead(FILE *stream, void **treeNode, uint32_t flags);
int DlgNodeStartRead(FILE *stream, void **treeNode, uint32_t flags);
int DlgNodeNotesRead(FILE *stream, void **treeNode, uint32_t flags);
int DlgNodeExitRead(FILE *stream, void **treeNode, uint32_t flags);
int DlgNodeConditionalRead(FILE *stream, void **treeNode, uint32_t flags);
int DlgFolderChildRead(FILE *stream, void **treeNode, uint32_t flags);
int DlgChoicesChildPreRead(FILE *stream, void **treeNode, uint32_t flags);
int DlgChoicesChildPostRead(FILE *stream, void **treeNode, uint32_t flags);

#endif