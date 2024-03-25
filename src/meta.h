#include <inttypes.h>
#include <stdio.h>

struct MetaStreamHeader // Based on Lucas Saragosa's Telltale inspector
{
    uint32_t version; // For example MSV6
    uint32_t defaultSize;
    uint32_t debugSize;
    uint32_t asyncSize;
    uint32_t numVersion; // A number between 0 and 0x3E8. It defines the length of the two next variables
    uint64_t *typeSymbolCrc;
    uint32_t *versionCrc;
};

void readMetaStreamHeader(FILE *file, struct MetaStreamHeader *header);