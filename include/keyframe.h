#include <stdio.h>
#include <inttypes.h>

int KeyframedValue_Transform_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int DCArray_KeyframedValue_Transform___Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int KeyframedValue_float_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int DCArray_KeyframedValue_float___Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int KeyframedValue_unsigned__int64_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int DCArray_KeyframedValue_unsigned__int64___Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int KeyframedValue_AnimOrChore_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int DCArray_KeyframedValue_AnimOrChore___Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int KeyframedValue_bool_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int DCArray_KeyframedValue_bool___Sample_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int AnimationValueInterfaceBaseRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int AnimatedValueInterfaceGenericRead(FILE *stream, struct TreeNode *node, uint32_t flags);