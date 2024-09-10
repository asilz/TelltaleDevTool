#include <stdio.h>
#include <inttypes.h>
#include <tree.h>

int TransformRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int SArray_TRange_float_3_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int BoneContraintsRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int SkeletonEntryRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int SkeletonRead(FILE *stream, struct TreeNode *node, uint32_t flags);

int DCArray_Skeleton__Entry_Read(FILE *stream, struct TreeNode *node, uint32_t flags);