#include <stdio.h>
#include <inttypes.h>

int EventStorageRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int EventStoragePageRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int EventLoggerEventRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int DCArray_EventStorage__PageEntry_Read(FILE *stream, struct TreeNode *node, uint32_t flags);