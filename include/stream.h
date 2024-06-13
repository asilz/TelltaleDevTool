#include <stdio.h>
#include <inttypes.h>

FILE *cfopen(const char *filename, const char *mode);
int cfseek(FILE *stream, int64_t offset, int whence);
int64_t cftell(FILE *stream);