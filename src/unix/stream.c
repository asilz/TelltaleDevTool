#include <stdio.h>
#include <inttypes.h>

FILE *cfopen(const char *filename, const char *mode)
{
    return fopen(filename, mode);
}

int cfseek(FILE *stream, int64_t offset, int whence)
{
    return fseek(stream, offset, whence);
}

int64_t cftell(FILE *stream)
{
    return ftell(stream);
}