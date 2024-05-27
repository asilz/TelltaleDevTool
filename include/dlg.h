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
    struct TreeNode **childSet;
    uint64_t nameSymbol;
    uint32_t prodReportPropBlock;
    void *prodReportProp;
};

struct Dlg
{
    uint32_t idOwnerBlock;
    struct DlgObjectIDOwner idOwner;
    uint32_t stringBlock;
    struct TreeNode *name;
    uint32_t version;
    uint32_t idBlock;
    struct DlgObjectID id;
    uint32_t langBlock;
    void *db;
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
    struct TreeNode *folders;
    uint32_t nodeCount;
    struct TreeNode **nodes;
};

// void DlgWrite(FILE *stream, struct Dlg *dlg, uint32_t flags);
int DlgRead(FILE *stream, struct TreeNode *dlg, uint32_t flags);
int DlgNodeLogicRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgNodeTextRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgNodeExchangeRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgNodeChoicesRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgChoiceRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgNodeWaitRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgNodeMarkerRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgNodeIdleRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgConditionalCaseRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgNodeScriptRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgNodeSequenceRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgNodeSequenceElementRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgNodeJumpRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgNodeStartRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgNodeNotesRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgNodeExitRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgNodeConditionalRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgFolderChildRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgChoicesChildPreRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgChoicesChildPostRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgConditionInputRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgConditionTimeRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgConditionRuleRead(FILE *stream, struct TreeNode *node, uint32_t flags);

int DependencyLoaderRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int LogicItemRead(FILE *stream, struct TreeNode *logicItem, uint32_t flags);
int LogicGroupRead(FILE *stream, struct TreeNode *logicGroup, uint32_t flags);
int RuleRead(FILE *stream, struct TreeNode *rule, uint32_t flags);
int DlgDownStreamVisibilityConditionsRead(FILE *stream, struct TreeNode *visCond, uint32_t flags);
int DlgVisibilityConditionsRead(FILE *stream, struct TreeNode *visCond, uint32_t flags);
int DlgVisibilityConditionsOwnerRead(FILE *stream, struct TreeNode *visCond, uint32_t flags);
int DlgObjectPropsRead(FILE *stream, struct TreeNode *objectProps, uint32_t flags);
int DlgObjectPropsOwnerRead(FILE *stream, struct TreeNode *objectProps, uint32_t flags);
int DlgObjectIDRead(FILE *stream, struct TreeNode *id, uint32_t flags);
int DlgObjectIDOwnerRead(FILE *stream, struct TreeNode *IDOwner, uint32_t flags);
int DlgNodeLinkRead(FILE *stream, struct TreeNode *link, uint32_t flags);
int DlgNodeRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgChildSetRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int LogicItemRead(FILE *stream, struct TreeNode *logicItem, uint32_t flags);
int ScriptEnumRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int AnimOrChoreRead(FILE *stream, struct TreeNode *node, uint32_t flags);
#endif