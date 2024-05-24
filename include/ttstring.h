#ifndef STRING_H
#define STRING_H
#include <inttypes.h>
#include <stdio.h>
#include <tree.h>

int StringRead(FILE *stream, struct TreeNode *string, uint32_t flags);

#endif // STRING_H