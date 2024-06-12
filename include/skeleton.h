#include <stdio.h>
#include <inttypes.h>
#include <tree.h>

int TransformRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int TRange_float_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int SArray_TRange_float_3_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int BoneContraintsRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int SkeletonEntryRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int SkeletonRead(FILE *stream, struct TreeNode *node, uint32_t flags);