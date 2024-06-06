#include <stdlib.h>
#include <meta.h>
#include <intrinsic.h>
#include <prop.h>
#include <container.h>
#include <stream.h>

#define EMBEDDED_PROP_FLAG 0x0400

struct TypeGroup
{
    uint64_t typeSymbol;
    uint32_t numValues;
    struct NameTypePair
    {
        uint64_t nameSymbol;
        void *buffer;
    } *typeStruct; // The number of structs is equal to numValues. The type of these structs can be determined from the typeSymbol
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

int TypeGroupRead(FILE *stream, struct TreeNode *group, uint32_t flags)
{
    group->childCount = 2;
    group->children = malloc(group->childCount * sizeof(struct TreeNode *));

    group->children[0] = calloc(1, sizeof(struct TreeNode));
    group->children[0]->parent = group;
    group->children[0]->typeSymbol = 0x4f023463d89fb0; // crc64 of "Symbol"
    intrinsic8Read(stream, group->children[0], flags);

    group->children[1] = calloc(1, sizeof(struct TreeNode));
    group->children[1]->parent = group;
    group->children[1]->typeSymbol = 0xc810fa560d267d67; // TODO: Set symbol

    group->children[1]->childCount = 1;
    group->children[1]->children = malloc(group->children[1]->childCount * sizeof(struct TreeNode *));

    group->children[1]->children[0] = calloc(1, sizeof(struct TreeNode));
    group->children[1]->children[0]->typeSymbol = 0x99d7c52ea7f0f97d; // crc64 of "int"
    intrinsic4Read(stream, group->children[1]->children[0], flags);
    group->children[1]->children[0]->parent = group->children[1];

    group->children[1]->childCount += *(uint32_t *)(group->children[1]->children[0]->data.staticBuffer) * 2;
    group->children[1]->children = realloc(group->children[1]->children, group->children[1]->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 1; i < group->children[1]->childCount; ++i)
    {
        group->children[1]->children[i] = calloc(1, sizeof(struct TreeNode));
        group->children[1]->children[i]->parent = group->children[1];
        if (i % 2)
        {
            group->children[1]->children[i]->typeSymbol = 0x4f023463d89fb0; // crc64 of "Symbol"
            intrinsic8Read(stream, group->children[1]->children[i], flags);
        }
        else
        {
            group->children[1]->children[i]->typeSymbol = *(uint64_t *)(group->children[0]->data.staticBuffer);
            readMetaClass(stream, group->children[1]->children[i], flags);
        }
    }

    return 0;
}

static int PropCoreRead(FILE *stream, struct TreeNode *prop, uint32_t flags) // Don't know what to name it
{
    prop->childCount = 2;
    prop->children = malloc(prop->childCount * sizeof(struct TreeNode *));

    prop->children[0] = calloc(1, sizeof(struct TreeNode));
    prop->children[0]->parent = prop;
    prop->children[0]->typeSymbol = 0x1d0bcff71e6bc2e1;                                     // crc64 of "DCArray<Handle<PropertySet>>"
    genericArrayRead(stream, prop->children[0], flags, intrinsic8Read, 0x387e6b9ca558ac8d); // List of Handle<PropertySet>

    prop->children[1] = calloc(1, sizeof(struct TreeNode));
    prop->children[1]->parent = prop;
    prop->children[1]->typeSymbol = 0xd48d0c3b810c1975; // TODO: Set symbol
    genericArrayRead(stream, prop->children[1], flags, TypeGroupRead, prop->children[1]->typeSymbol);

    if ((*(uint32_t *)prop->parent->children[1]->data.staticBuffer & EMBEDDED_PROP_FLAG) != 0)
    {
        prop->children = realloc(prop->children, ++prop->childCount * sizeof(struct TreeNode *));
        prop->children[2] = calloc(1, sizeof(struct TreeNode));
        prop->children[2]->parent = prop;
        prop->children[2]->typeSymbol = 0xcd75dc4f6b9f15d2; // crc64 of "PropertySet"
        PropRead(stream, prop->children[2], flags);
    }

    return 0;
}

int PropRead(FILE *stream, struct TreeNode *prop, uint32_t flags)
{
    prop->childCount = 3;
    prop->children = malloc(prop->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < prop->childCount; ++i)
    {
        prop->children[i] = calloc(1, sizeof(struct TreeNode));
        prop->children[i]->parent = prop;
    }

    prop->children[0]->typeSymbol = 0x99d7c52ea7f0f97d; // crc64 of "int"
    intrinsic4Read(stream, prop->children[0], flags);

    prop->children[1]->typeSymbol = 0x84283cb979d71641; // crc64 of "Flags"
    intrinsic4Read(stream, prop->children[1], flags);

    prop->children[2]->isBlocked = 1;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    prop->children[2]->typeSymbol = 0xe908072c98443ada; // TODO: Set value
    PropCoreRead(stream, prop->children[2], flags);

    return 0;
}
