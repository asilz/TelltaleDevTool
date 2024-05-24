#include <ttstring.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct String
{
    uint32_t length;
    uint8_t *data;
};

int StringRead(FILE *stream, struct TreeNode *string, uint32_t flags)
{

    uint32_t stringSize;
    fread(&stringSize, 1, sizeof(stringSize), stream);
    string->dataSize = stringSize + sizeof(stringSize);
    if (string->dataSize > sizeof(string->data))
    {
        string->data.dynamicBuffer = malloc(string->dataSize);
        memcpy(string->data.dynamicBuffer, &stringSize, sizeof(stringSize));
        fread(string->data.dynamicBuffer + sizeof(stringSize), stringSize, 1, stream);
    }
    else
    {
        // I think this could be done better, but not completely sure how
        memcpy(string->data.staticBuffer, &stringSize, sizeof(stringSize));
        fread(string->data.staticBuffer + sizeof(stringSize), stringSize, 1, stream);
    }
    return 0;
}
