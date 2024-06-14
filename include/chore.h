#include <stdio.h>
#include <inttypes.h>

int WalkPathRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int HandleBaseRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DCArray_Handle_Chore__Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int ChoreAgentRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int ChoreResourceBlockRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int ChoreResourceRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int ChoreRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int ChoreAgentAttachmentRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int AutoActStatusRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int KeyframedValue_unsigned__int64_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int DCArray_KeyframedValue_unsigned__int64___Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int AnimatedValueInterface_unsigned__int64_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int AnimationValueInterfaceBaseRead(FILE *stream, struct TreeNode *node, uint32_t flags);

int PerAgentClipResourceFilterRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int Set_StringStringCompareCaseInsensitive_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int Map_StringClipResourceFilterStringCompareCaseInsensitive_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int DCArray_int_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int DCArray_ChoreResource__Block_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int Map_Symbolfloatless_Symbol__Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int Map_SymbolWalkPathless_Symbol__Read(FILE *stream, struct TreeNode *node, uint32_t flags);