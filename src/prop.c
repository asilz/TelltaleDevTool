#include <inttypes.h>
#include <stdio.h>
#include <crc64.h>
#include <types.h>
#include <string.h>
#include <stdlib.h>
#include <meta.h>

struct Symbol
{
    uint64_t crc64;
};

struct KeyInfo
{
    struct Flags flags;
    struct Symbol crcName;
};

struct PropertySet
{
    uint32_t version;
    uint32_t flags;
    uint32_t size; // Int at BlockStart which is the total size of the values all serialized between BlockStart and BlockEnd in bytes (includes the 4 byte block length in the size!).
    uint32_t parentCount;
    uint64_t *parentSymbols;
    uint32_t numTypes;
    struct TypeGroup *groups; // Number of groups is equal to numTypes
};

struct NameTypePair
{
    uint64_t nameSymbol;
    void *buffer;
};

struct TypeGroup
{
    uint64_t typeSymbol;
    uint32_t numValues;
    struct NameTypePair *typeStruct // The number of structs is equal to numValues. The type of these structs can be determined from the typeSymbol
};

int readProp(FILE *stream, void **property, uint32_t flags)
{
    *property = malloc(sizeof(struct PropertySet));
    struct PropertySet *prop = *property;
    printf("readProp\n");
    fread(&(prop->version), sizeof(prop->version), 1, stream);
    fread(&(prop->flags), sizeof(prop->flags), 1, stream);
    fread(&(prop->size), sizeof(prop->size), 1, stream);
    fread(&(prop->parentCount), sizeof(prop->parentCount), 1, stream);

    prop->parentSymbols = malloc(prop->parentCount * sizeof(uint64_t));
    fread(prop->parentSymbols, prop->parentCount * sizeof(uint64_t), 1, stream);

    fread(&(prop->numTypes), sizeof(prop->numTypes), 1, stream);

    prop->groups = malloc(prop->numTypes * sizeof(struct TypeGroup));

    for (uint32_t i = 0; i < prop->numTypes; ++i)
    {
        fread(&(prop->groups[i].typeSymbol), sizeof(prop->groups[i].typeSymbol), 1, stream);
        fread(&(prop->groups[i].numValues), sizeof(prop->groups[i].numValues), 1, stream);

        prop->groups[i].typeStruct = malloc(prop->groups[i].numValues * sizeof(struct NameTypePair));

        for (uint32_t j = 0; j < prop->groups[i].numValues; ++j)
        {
            fread(&prop->groups[i].typeStruct[j].nameSymbol, sizeof(prop->groups[i].typeStruct[j].nameSymbol), 1, stream);
            readMetaClass(stream, &(prop->groups[i].typeStruct[j].buffer), prop->groups[i].typeSymbol, flags);
        }
    }
    return 0;
}

int freeProp(struct PropertySet *prop)
{
    for (uint32_t i = 0; i < prop->numTypes; ++i)
    {
        for (uint32_t j = 0; j < prop->groups[i].numValues; ++j)
        {
        }
        free(prop->groups[i].typeStruct)
    }
    free(prop->groups);
    free(prop->parentSymbols);
    return 0;
}
