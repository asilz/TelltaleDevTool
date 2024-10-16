#include <inttypes.h>

#ifndef DLG_H
#define DLG_H

int DlgRead(FILE *stream, struct TreeNode *dlg, uint32_t flags);
int DlgFolderRead(FILE *stream, struct TreeNode *folder, uint32_t flags);
int DlgNodeLogicRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgNodeTextRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgNodeExchangeRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgNodeChoicesRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgChoiceRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgNodeChoreRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgNodeWaitRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgNodeCancelChoicesRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgNodeMarkerRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgNodeIdleRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgConditionalCaseRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgNodeScriptRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgNodeSequenceRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgNodeParallelRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgNodeParallelPElementRead(FILE *stream, struct TreeNode *node, uint32_t flags);
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
int DlgLineRead(FILE *stream, struct TreeNode *line, uint32_t flags);
int DlgChildRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgChildSetRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgChildSetChoiceRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgConditionSetRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgChainHeadRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgChildSetConditionalCaseRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgConditionRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgChildSetElementRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgNodeCriteriaRead(FILE *stream, struct TreeNode *node, uint32_t flags);

int DependencyLoaderRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int LogicItemRead(FILE *stream, struct TreeNode *logicItem, uint32_t flags);
int LogicGroupRead(FILE *stream, struct TreeNode *logicGroup, uint32_t flags);
int RuleRead(FILE *stream, struct TreeNode *rule, uint32_t flags);
int DlgDownstreamVisibilityConditionsRead(FILE *stream, struct TreeNode *visCond, uint32_t flags);
int DlgLineCollectionRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgVisibilityConditionsRead(FILE *stream, struct TreeNode *visCond, uint32_t flags);
int DlgVisibilityConditionsOwnerRead(FILE *stream, struct TreeNode *visCond, uint32_t flags);
int DlgObjectPropsRead(FILE *stream, struct TreeNode *objectProps, uint32_t flags);
int DlgObjectPropsOwnerRead(FILE *stream, struct TreeNode *objectProps, uint32_t flags);
int DlgObjIDOwnerRead(FILE *stream, struct TreeNode *IDOwner, uint32_t flags);
int DlgNodeLinkRead(FILE *stream, struct TreeNode *link, uint32_t flags);
int DlgNodeRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DlgChildSetRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int LogicItemRead(FILE *stream, struct TreeNode *logicItem, uint32_t flags);
int ScriptEnumRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int AnimOrChoreRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int Vector3Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int Vector4Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int Set_Colorless_Color__Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int Map_Symbolintless_Symbol__Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int Map_Symbolboolless_Symbol__Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int Map_StringLogicGroup__LogicItemless_String__Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int DCArray_LogicGroup_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int DCArray_DlgNodeExchange__Entry_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int DCArray_Ptr_DlgChild__Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int Map_intDlgLineless_int__Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int JiraRecordManagerRead(FILE *stream, struct TreeNode *node, uint32_t flags);

#endif