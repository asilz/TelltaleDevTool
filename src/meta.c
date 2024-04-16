#include <inttypes.h>
#include <stdio.h>
#include "meta.h"
#include <stdlib.h>
#include "ttarch.h"

void readMetaStreamHeader(FILE *stream, struct MetaStreamHeader *header)
{
    fread(&(header->version), sizeof(header->version), 1, stream);
    fread(&(header->defaultSize), sizeof(header->defaultSize), 1, stream);
    fread(&(header->debugSize), sizeof(header->debugSize), 1, stream);
    fread(&(header->asyncSize), sizeof(header->asyncSize), 1, stream);
    fread(&(header->numVersion), sizeof(header->numVersion), 1, stream);

    header->crc = malloc(sizeof(header->crc) * header->numVersion);

    for (uint32_t i = 0; i < header->numVersion; ++i)
    {
        fread((uint8_t *)(header->crc + i), sizeof(header->crc->typeSymbolCrc) + sizeof(header->crc->versionCrc), 1, stream);
    }
}

void readMetaStream(FILE *stream, struct MetaStreamHeader *header)
{
    readMetaStreamHeader(stream, header);

    if ((int32_t)header->defaultSize < 0)
    {
        uint32_t defaultStart = ftell(stream);
        streamDecrypt(&stream);
        header->defaultSize = ftell(stream) - defaultStart;
    }
    else
    {
        fseek(stream, header->defaultSize, SEEK_CUR);
    }

    if ((int32_t)header->debugSize < 0)
    {
        uint32_t debugStart = ftell(stream);
        streamDecrypt(&stream);
        header->debugSize = ftell(stream) - debugStart;
    }
    else
    {
        fseek(stream, header->debugSize, SEEK_CUR);
    }

    if ((int32_t)header->asyncSize < 0)
    {
        uint32_t asyncStart = ftell(stream);
        streamDecrypt(&stream);
        header->asyncSize = ftell(stream) - asyncStart;
    }
    else
    {
        fseek(stream, header->asyncSize, SEEK_CUR);
    }
}
