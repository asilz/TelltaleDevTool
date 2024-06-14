#include <stdio.h>
#include <tree.h>

int PropRead(FILE *stream, struct TreeNode *prop, uint32_t flags);
int TypeGroupRead(FILE *stream, struct TreeNode *group, uint32_t flags);

int DCArray_Handle_PropertySet__Read(FILE *stream, struct TreeNode *prop, uint32_t flags);