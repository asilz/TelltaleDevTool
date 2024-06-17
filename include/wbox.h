#include <stdio.h>
#include <inttypes.h>

int DCArray_WalkBoxes__Tri_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int DCArray_WalkBoxes__Vert_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int DCArray_WalkBoxes__Quad_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int DCArray_Vector3_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int WalkBoxesRead(FILE *stream, struct TreeNode *node, uint32_t flags);