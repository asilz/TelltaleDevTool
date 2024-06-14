#ifndef STRING_H
#define STRING_H
#include <inttypes.h>
#include <stdio.h>
#include <tree.h>

int StringRead(FILE *stream, struct TreeNode *string, uint32_t flags);
int DCArray_String_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int Map_StringStringless_String__Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int Map_intStringless_int__Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int Map_Stringintless_String__Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int List_String_Read(FILE *stream, struct TreeNode *node, uint32_t flags);

#endif // STRING_H