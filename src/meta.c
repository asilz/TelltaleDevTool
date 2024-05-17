#include <inttypes.h>
#include <stdio.h>
#include <meta.h>
#include <stdlib.h>
#include <ttarch.h>
#include <crc64.h>
#include <types.h>
#include <string.h>
#include <ttbool.h>
#include <dlg.h>

#define META_CLASS_DESCRIPTIONS_COUNT 977

typedef int (*serializeFunction)(FILE *stream, void **object, uint32_t flags);

struct MetaClassDescription
{
    uint8_t *name;

    serializeFunction read;
    serializeFunction write;
};

static struct MetaClassDescription metaClassDescriptions[META_CLASS_DESCRIPTIONS_COUNT] = {0};

void readMetaStreamHeader(FILE *stream, struct MetaStreamHeader *header)
{
    fread(&(header->version), sizeof(header->version), 1, stream);
    fread(&(header->defaultSize), sizeof(header->defaultSize), 1, stream);
    fread(&(header->debugSize), sizeof(header->debugSize), 1, stream);
    fread(&(header->asyncSize), sizeof(header->asyncSize), 1, stream);
    fread(&(header->numVersion), sizeof(header->numVersion), 1, stream);
    header->crc = malloc((sizeof(struct MetaStreamHeader)) * header->numVersion);

    for (uint32_t i = 0; i < header->numVersion; ++i)
    {
        fread(header->crc + i, sizeof(header->crc->typeSymbolCrc) + sizeof(header->crc->versionCrc), 1, stream);
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

    fseek(stream, (-((int64_t)header->defaultSize + (int64_t)header->debugSize + (int64_t)header->asyncSize)), SEEK_CUR); // Seeks back to the end of header
}

int readMetaClass(FILE *stream, void **object, uint64_t typeSymbol, uint32_t flags)
{
    enum Type type = searchDatabase("protonDatabase.db", typeSymbol);
    serializeFunction readFunction = metaClassDescriptions[type].read;
    if (readFunction == NULL)
    {
        printf("Error: Read function not implemented for %s\n", metaClassDescriptions[type].name);
        return -1;
    }
    return metaClassDescriptions[type].read(stream, object, flags);
}

int writeMetaClass(FILE *stream, void **object, uint64_t typeSymbol, uint32_t flags)
{
    enum Type type = searchDatabase("protonDatabase.db", typeSymbol);
    serializeFunction writeFunction = metaClassDescriptions[type].write;
    if (writeFunction == NULL)
    {
        printf("Error: Write function not implemented for %s\n", metaClassDescriptions[type].name);
        return -1;
    }
    return metaClassDescriptions[type].write(stream, object, flags);
}

char *getMetaClassName(uint64_t typeSymbol)
{
    return metaClassDescriptions[searchDatabase("protonDatabase.db", typeSymbol)].name;
}

int readLanguageResProxy(FILE *stream, void **languageResProxy, uint32_t flags)
{
    *languageResProxy = malloc(sizeof(uint32_t));
    fread(*languageResProxy, sizeof(uint32_t), 1, stream);
}

int initializeMetaClassDescriptions()
{
    FILE *stream = fopen("./typeNames2.txt", "rb");
    for (uint16_t i = 1; i < META_CLASS_DESCRIPTIONS_COUNT; ++i)
    {
        uint8_t buffer[256];
        size_t nameLength;
        for (int j = 0; j < 256; ++j)
        {
            uint8_t byte;
            if (fread(&byte, 1, 1, stream) == 0)
            {
                break;
            }
            if (byte == '\n')
            {
                nameLength = j + 1;
                buffer[j] = '\0';
                break;
            }
            buffer[j] = byte;
        }
        metaClassDescriptions[i].name = malloc(nameLength);
        memcpy(metaClassDescriptions[i].name, buffer, nameLength);
    }
    fclose(stream);

    metaClassDescriptions[bool_type].read = readBool;
    metaClassDescriptions[LanguageResProxy].read = readLanguageResProxy;

    metaClassDescriptions[DlgNodeLogic].read = DlgNodeLogicRead;
    metaClassDescriptions[DlgNodeExchange].read = DlgNodeExchangeRead;
    metaClassDescriptions[DlgNodeChoices].read = DlgNodeChoicesRead;
    metaClassDescriptions[DlgChoice].read = DlgChoiceRead;
    metaClassDescriptions[DlgNodeWait].read = DlgNodeWaitRead;
    metaClassDescriptions[DlgNodeMarker].read = DlgNodeMarkerRead;
    metaClassDescriptions[DlgNodeIdle].read = DlgNodeIdleRead;
    metaClassDescriptions[DlgConditionalCase].read = DlgConditionalCaseRead;
    metaClassDescriptions[DlgNodeScript].read = DlgNodeScriptRead;
    metaClassDescriptions[DlgNodeSequence].read = DlgNodeSequenceRead;
    metaClassDescriptions[DlgNodeJump].read = DlgNodeJumpRead;
    metaClassDescriptions[DlgNodeStart].read = DlgNodeStartRead;
    metaClassDescriptions[DlgNodeNotes].read = DlgNodeNotesRead;
    metaClassDescriptions[DlgNodeExit].read = DlgNodeExitRead;
    metaClassDescriptions[DlgNodeConditional].read = DlgNodeConditionalRead;
    metaClassDescriptions[DlgFolderChild].read = DlgFolderChildRead;
    metaClassDescriptions[DlgChoicesChildPre].read = DlgChoicesChildPreRead;
    metaClassDescriptions[DlgChoicesChildPost].read = DlgChoicesChildPostRead;

    printf("init metaClassDescriptions\n");
    return 0;
}
