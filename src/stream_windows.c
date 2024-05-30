#include <stdio.h>
#include <inttypes.h>

FILE *cfopen(const char *filename, const char *mode)
{
    FILE *stream;
    if (fopen_s(&stream, filename, mode) == 0 && stream != NULL)
    {
        return stream;
    }
    return NULL;
}

int cfseek(FILE *stream, int64_t offset, int whence)
{
    return _fseeki64(stream, offset, whence);
}

int64_t cftell(FILE *stream)
{
    return _ftelli64(stream);
}