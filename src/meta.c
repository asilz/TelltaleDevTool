#include <inttypes.h>
#include <stdio.h>
#include "meta.h"
#include <stdlib.h>

void readMetaStreamHeader(FILE *file, struct MetaStreamHeader *header)
{
    fread(&(header->version), sizeof(header->version), 1, file);
    fread(&(header->defaultSize), sizeof(header->defaultSize), 1, file);
    fread(&(header->debugSize), sizeof(header->debugSize), 1, file);
    fread(&(header->asyncSize), sizeof(header->asyncSize), 1, file);
    fread(&(header->numVersion), sizeof(header->numVersion), 1, file);

    header->typeSymbolCrc = malloc(sizeof(uint64_t) * header->numVersion);
    header->versionCrc = malloc(sizeof(uint32_t) * header->numVersion);

    fread(header->typeSymbolCrc, sizeof(uint64_t) * header->numVersion, 1, file);
    fread(header->versionCrc, sizeof(uint32_t) * header->numVersion, 1, file);
}