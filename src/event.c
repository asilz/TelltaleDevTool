#include <meta.h>
#include <types.h>
#include <stdlib.h>
#include <stream.h>
#include <container.h>

static int EventStorage__PageEntryRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "mhPage", .metaClassDescriptionIndex = Handle_EventStorage_},
        {.isBlocked = 0, .memberName = "mMaxEventID", .metaClassDescriptionIndex = unsignedint},
    };
    return genericRead(stream, node, flags, 2, descriptions);
}

int DCArray_EventStorage__PageEntry_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->child = malloc(sizeof(struct TreeNode));
    node->child->description = getMetaClassDescriptionByIndex(int_type);
    node->child->description->read(stream, node->child, flags);
    node->child->parent = node;
    node->child->serializeType = 0;
    node->child->memberName = "entryCount";
    node->child->isBlocked = 0;
    node->child->sibling = NULL;

    struct TreeNode *currentNode = node->child;

    for (uint32_t i = 0; i < *(uint32_t *)(node->child->staticBuffer); ++i)
    {
        currentNode->sibling = calloc(1, sizeof(struct TreeNode));
        currentNode = currentNode->sibling;
        currentNode->parent = node;
        EventStorage__PageEntryRead(stream, currentNode, flags);
    }
    return 0;
}

int EventStorageRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 0, .memberName = "mVersion", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 0, .memberName = "mSessionID", .metaClassDescriptionIndex = unsigned__int64},
        {.isBlocked = 1, .memberName = "mPages", .metaClassDescriptionIndex = DCArray_EventStorage__PageEntry_},
        {.isBlocked = 1, .memberName = "mName", .metaClassDescriptionIndex = String},
        {.isBlocked = 0, .memberName = "mLastEventID", .metaClassDescriptionIndex = unsignedlong},
        {.isBlocked = 0, .memberName = "mEventStoragePageSize", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 0, .memberName = "serializeEventStoragePage", .metaClassDescriptionIndex = bool_type},
    };
    genericRead(stream, node, flags, 7, descriptions);

    struct TreeNode *currentNode = node->child->sibling->sibling->sibling->sibling->sibling->sibling;

    if (*((uint8_t *)(currentNode->staticBuffer)))
    {
        currentNode->sibling = calloc(1, sizeof(struct TreeNode));
        currentNode->sibling->parent = currentNode->parent;
        currentNode = currentNode->sibling;
        currentNode->memberName = "eventStoragePage";
        currentNode->description = getMetaClassDescriptionByIndex(EventStoragePage);
        currentNode->description->read(stream, currentNode, flags);
    }

    return 0;
};

int EventStoragePageRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 0, .memberName = "mVersion", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 0, .memberName = "mSessionID", .metaClassDescriptionIndex = unsigned__int64},
        {.isBlocked = 1, .memberName = "mFlushedNameOnDisk", .metaClassDescriptionIndex = String},
    };
    genericRead(stream, node, flags, 3, descriptions);

    node->child->sibling->sibling->sibling = calloc(1, sizeof(struct TreeNode));
    struct TreeNode *currentNode = node->child->sibling->sibling->sibling;
    currentNode->parent = node;
    currentNode->memberName = "eventLoggerEvents";
    genericArrayRead(stream, currentNode, flags, getMetaClassDescriptionByIndex(EventLoggerEvent));

    return 0;
}

static int EventLoggerCoreRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->child = calloc(1, sizeof(struct TreeNode));
    struct TreeNode *currentNode = node->child;
    currentNode->parent = node;
    currentNode->memberName = "typeHeaderCount";
    currentNode->description = getMetaClassDescriptionByIndex(long_type);
    currentNode->description->read(stream, currentNode, flags);

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;
    currentNode->memberName = "zero";
    currentNode->description = getMetaClassDescriptionByIndex(long_type);
    currentNode->description->read(stream, currentNode, flags);

    struct TreeNode *typeHeader = currentNode;

    for (uint32_t i = 0; i < *(uint32_t *)(node->child->staticBuffer); ++i)
    {
        currentNode->sibling = calloc(1, sizeof(struct TreeNode));
        currentNode = currentNode->sibling;
        currentNode->parent = node;

        currentNode->child = calloc(1, sizeof(struct TreeNode));
        struct TreeNode *typeHeaderChild = currentNode->child;
        typeHeaderChild->parent = currentNode;
        typeHeaderChild->memberName = "mType";
        typeHeaderChild->description = getMetaClassDescriptionByIndex(Symbol);
        typeHeaderChild->description->read(stream, typeHeaderChild, flags);

        typeHeaderChild->sibling = calloc(1, sizeof(struct TreeNode));
        typeHeaderChild->sibling->parent = typeHeaderChild->parent;
        typeHeaderChild = typeHeaderChild->sibling;
        typeHeaderChild->memberName = "eventDataCount";
        typeHeaderChild->description = getMetaClassDescriptionByIndex(long_type);
        typeHeaderChild->description->read(stream, typeHeaderChild, flags);

        uint32_t eventDataCount = *(uint32_t *)(typeHeaderChild->staticBuffer);
        while (eventDataCount--)
        {
            typeHeaderChild->sibling = calloc(1, sizeof(struct TreeNode));
            typeHeaderChild->sibling->parent = typeHeaderChild->parent;
            typeHeaderChild = typeHeaderChild->sibling;
            typeHeaderChild->memberName = "dataType";
            typeHeaderChild->description = getMetaClassDescriptionByIndex(unsignedchar);
            typeHeaderChild->description->read(stream, typeHeaderChild, flags);
        }
    }
    for (uint32_t i = 0; i < *(uint32_t *)(node->child->staticBuffer); ++i)
    {
        typeHeader = typeHeader->sibling;
        struct TreeNode *eventData = typeHeader->child->sibling->sibling;
        while (eventData != NULL)
        {
            if (*((uint8_t *)(eventData->staticBuffer)) == 0)
            {
                currentNode->sibling = calloc(1, sizeof(struct TreeNode));
                currentNode = currentNode->sibling;
                currentNode->parent = node;
                currentNode->memberName = "symbol";
                currentNode->description = getMetaClassDescriptionByIndex(Symbol);
                currentNode->description->read(stream, currentNode, flags);
            }
            else if (*((uint8_t *)(eventData->staticBuffer)) == 1)
            {
                currentNode->sibling = calloc(1, sizeof(struct TreeNode));
                currentNode = currentNode->sibling;
                currentNode->parent = node;
                currentNode->description = getMetaClassDescriptionByIndex(int64);
                currentNode->description->read(stream, currentNode, flags);
            }
            else if (*((uint8_t *)(eventData->staticBuffer)) == 2)
            {
                currentNode->sibling = calloc(1, sizeof(struct TreeNode));
                currentNode = currentNode->sibling;
                currentNode->parent = node;
                currentNode->description = getMetaClassDescriptionByIndex(double_type);
                currentNode->description->read(stream, currentNode, flags);
            }

            currentNode->sibling = calloc(1, sizeof(struct TreeNode));
            currentNode = currentNode->sibling;
            currentNode->parent = node;
            currentNode->memberName = "severity";
            currentNode->description = getMetaClassDescriptionByIndex(char_type);
            currentNode->description->read(stream, currentNode, flags);

            eventData = eventData->sibling;
        }
    }
    return 0;
}

int EventLoggerEventRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->child = calloc(1, sizeof(struct TreeNode));
    struct TreeNode *currentNode = node->child;
    currentNode->parent = node;
    currentNode->memberName = "mEventID";
    currentNode->description = getMetaClassDescriptionByIndex(unsignedlong);
    currentNode->description->read(stream, currentNode, flags);

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;
    currentNode->memberName = "mMaxSeverity";
    currentNode->description = getMetaClassDescriptionByIndex(long_type);
    currentNode->description->read(stream, currentNode, flags);

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;
    currentNode->isBlocked = 1;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    currentNode->memberName = "core";
    EventLoggerCoreRead(stream, currentNode, flags);

    return 0;
}
