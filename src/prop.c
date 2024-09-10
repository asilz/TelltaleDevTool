#include <stdlib.h>
#include <meta.h>
#include <intrinsic.h>
#include <prop.h>
#include <container.h>
#include <stream.h>
#include <types.h>

enum PropertyFlags
{
    eSceneProperties = 0x1,
    eChoreAgentProperties = 0x2,
    eChoreResourceProperties = 0x4,
    ePlaybackControllerProperties = 0x8,
    eRuntimeProperties = 0x10,
    eRuleProperty = 0x20,
    eActorAgentProperties = 0x40,
    eAgentProperties = 0x80,
    eScriptChangedAtRuntime = 0x100,
    eExcludeFromSaveGames = 0x200,
    eHasEditorProps = 0x400,
    eInDelayPostLoadQueue = 0x1000,
    eInDelayPostLoadQueueLocked = 0x2000,
    eInDelayPostLoadQueueDeleted = 0x4000,
    eReadOnly = 0x8000,
    ePropertyInterface = 0x10000,
    ePropertyCallbacksOnly = 0x20000,
    eCreateKeysInAlt = 0x40000,
    eTransientProperties = 0x80000,
    eDontValidate = 0x100000,
    eReferencedByLuaFunc = 0x200000,
    eLockedInCache = 0x400000,
    eEmbedded = 0x800000,
    eSynchronousCallbacks = 0x1000000,
    eOwnsHandleToSelf = 0x2000000,
    eMaterialProperties = 0x4000000,
    eMaterialRuntimeProperties = 0x8000000
};

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

int TypeGroupRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->child = calloc(1, sizeof(struct TreeNode));
    struct TreeNode *currentNode = node->child;
    currentNode->parent = node;
    currentNode->description = getMetaClassDescriptionByIndex(Symbol);
    currentNode->description->read(stream, currentNode, flags);

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;

    currentNode->child = calloc(1, sizeof(struct TreeNode));
    currentNode->child->parent = currentNode;
    currentNode = currentNode->child;
    currentNode->description = getMetaClassDescriptionByIndex(int_type);
    currentNode->description->read(stream, currentNode, flags);
    uint32_t valueCount = *(uint32_t *)currentNode->staticBuffer * 2;

    const struct MetaClassDescription *metaClassDescription = getMetaClassDescriptionBySymbol(*(uint64_t *)(node->child->staticBuffer));

    for (uint32_t i = 0; i < valueCount; ++i)
    {
        currentNode->sibling = calloc(1, sizeof(struct TreeNode));
        currentNode->sibling->parent = currentNode->parent;
        currentNode = currentNode->sibling;
        if (i % 2)
        {
            currentNode->description = metaClassDescription;
            metaClassDescription->read(stream, currentNode, flags);
        }
        else
        {
            currentNode->description = getMetaClassDescriptionByIndex(Symbol);
            currentNode->description->read(stream, currentNode, flags);
        }
    }

    return 0;
}

static int PropCoreRead(FILE *stream, struct TreeNode *node, uint32_t flags) // Don't know what to name it
{
    node->child = calloc(1, sizeof(struct TreeNode));
    struct TreeNode *currentNode = node->child;
    currentNode->parent = node;
    currentNode->description = getMetaClassDescriptionByIndex(DCArray_Handle_PropertySet__);
    currentNode->description->read(stream, currentNode, flags);

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;
    genericArrayRead(stream, currentNode, flags, getMetaClassDescriptionByIndex(Map_SymbolPropertySetless_Symbol__)); // Array of typeGroup

    if (*(uint32_t *)(currentNode->parent->parent->child->sibling->staticBuffer) & eHasEditorProps)
    {
        currentNode->sibling = calloc(1, sizeof(struct TreeNode));
        currentNode->sibling->parent = currentNode->parent;
        currentNode = currentNode->sibling;
        currentNode->description = getMetaClassDescriptionByIndex(PropertySet);
        currentNode->description->read(stream, currentNode, flags);
    }

    return 0;
}

int PropRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->child = calloc(1, sizeof(struct TreeNode));
    struct TreeNode *currentNode = node->child;
    currentNode->parent = node;
    currentNode->description = getMetaClassDescriptionByIndex(int_type);
    currentNode->description->read(stream, currentNode, flags);

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;
    currentNode->description = getMetaClassDescriptionByIndex(Flags);
    currentNode->description->read(stream, currentNode, flags);

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;
    currentNode->isBlocked = 1;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    // // TODO: Set metaClassDescription
    PropCoreRead(stream, currentNode, flags);

    return 0;
}

int DCArray_Handle_PropertySet__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(Handle_PropertySet_));
}

int Map_StringPropertySetless_String__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericMapRead(stream, node, flags, getMetaClassDescriptionByIndex(String), getMetaClassDescriptionByIndex(PropertySet));
}
