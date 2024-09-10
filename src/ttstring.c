#include <ttstring.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <container.h>
#include <meta.h>
#include <types.h>

struct String
{
    uint32_t length;
    uint8_t *data;
};

int StringRead(FILE *stream, struct TreeNode *string, uint32_t flags)
{
    (void)flags;
    uint32_t stringSize;
    fread(&stringSize, 1, sizeof(stringSize), stream);
    string->dataSize = stringSize + sizeof(stringSize);
    if (string->dataSize > sizeof(string->staticBuffer))
    {
        string->dynamicBuffer = malloc(string->dataSize);
        memcpy(string->dynamicBuffer, &stringSize, sizeof(stringSize));
        fread(string->dynamicBuffer + sizeof(stringSize), stringSize, 1, stream);
    }
    else
    {
        // I think this could be done better, but not completely sure how
        memcpy(string->staticBuffer, &stringSize, sizeof(stringSize));
        fread(string->staticBuffer + sizeof(stringSize), stringSize, 1, stream);
    }
    return 0;
}

int DCArray_String_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(String));
}

int Map_StringStringless_String__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericMapRead(stream, node, flags, getMetaClassDescriptionByIndex(String), getMetaClassDescriptionByIndex(String));
}

int Map_intStringless_int__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericMapRead(stream, node, flags, getMetaClassDescriptionByIndex(int_type), getMetaClassDescriptionByIndex(String));
}

int Map_Stringintless_String__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericMapRead(stream, node, flags, getMetaClassDescriptionByIndex(String), getMetaClassDescriptionByIndex(int_type));
}

int List_String_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(String));
}
