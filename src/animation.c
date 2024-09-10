#include <tree.h>
#include <stream.h>
#include <container.h>
#include <stdlib.h>
#include <types.h>
#include <meta.h>
#include <assert.h>

enum AnimationValueInterfaceBaseFlags
{
    eDisabled = 0x1,
    eTimeBehavior = 0x2,
    eWeightBehavior = 0x4,
    eMoverAnim = 0x10,
    ePropertyAnimation = 0x20,
    eTextureMatrixAnimation = 0x40,
    eAudioDataAnimation = 0x80,
    eDontOptimize = 0x100,
    eHomogeneous = 0x200,
    eMixerScaled = 0x400,
    eMixerHomogeneous = 0x800,
    eStyleAnimation = 0x1000,
    ePropForceUpdate = 0x2000,
    eMixerOwned = 0x4000,
    eMixerDirty = 0x8000,
    eAdditive = 0x10000,
    eExternallyOwned = 0x20000,
    eDontMixPausedControllers = 0x40000,
    eRuntimeAnimation = 0x80000,
    eTransientAnimation = 0x100000,
    eToolOnly = 0x200000,
    eKeyedAttachmentAnimation = 0x400000,
    eMixerWeightedBlend = 0x800000,
    eValueKind = 0xff000000
};

static int AnimationValueRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    (void)flags;
    node->dataSize = sizeof(uint64_t) + 2 * sizeof(uint16_t);
    node->dynamicBuffer = malloc(node->dataSize);
    fread(node->dynamicBuffer, node->dataSize, 1, stream);

    return 0;
}

static int AnimationValueArrayRead(FILE *stream, struct TreeNode *node, uint32_t flags)
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
        AnimationValueRead(stream, currentNode, flags);
    }
    return 0;
}

static int AnimationCoreRead(FILE *stream, struct TreeNode *node, uint32_t flags) // Don't know what to name this
{
    node->child = calloc(1, sizeof(struct TreeNode));
    struct TreeNode *currentNode = node->child;
    currentNode->parent = node;
    currentNode->memberName = "interfaceCount";
    currentNode->description = getMetaClassDescriptionByIndex(long_type);
    currentNode->description->read(stream, currentNode, flags);
    uint32_t interfaceCount = *(uint32_t *)(currentNode->staticBuffer);

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;
    currentNode->memberName = "dataBufferSize";
    currentNode->description = getMetaClassDescriptionByIndex(long_type);
    currentNode->description->read(stream, currentNode, flags);

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;
    AnimationValueArrayRead(stream, currentNode, flags);

    struct TreeNode *animationValuesHeadersArray = currentNode->child->sibling;

    while (animationValuesHeadersArray != NULL)
    {
        for (uint32_t j = 0; j < ((uint16_t *)animationValuesHeadersArray->dynamicBuffer)[4]; ++j)
        {
            currentNode->sibling = calloc(1, sizeof(struct TreeNode));
            currentNode->sibling->parent = currentNode->parent;
            currentNode = currentNode->sibling;
            currentNode->description = getMetaClassDescriptionBySymbol(*(uint64_t *)animationValuesHeadersArray->dynamicBuffer);
            currentNode->description->read(stream, currentNode, flags);
        }
        animationValuesHeadersArray = animationValuesHeadersArray->sibling;
    }

    for (uint32_t i = 0; i < interfaceCount; ++i)
    {
        currentNode->sibling = calloc(1, sizeof(struct TreeNode));
        currentNode->sibling->parent = currentNode->parent;
        currentNode = currentNode->sibling;
        currentNode->description = getMetaClassDescriptionByIndex(Flags);
        currentNode->description->read(stream, currentNode, flags);
    }

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;
    currentNode->memberName = "symbolsNotSerialized";
    currentNode->description = getMetaClassDescriptionByIndex(unsignedshort);
    currentNode->description->read(stream, currentNode, flags);

    uint16_t symbolsNotSerialized = *(uint16_t *)(currentNode->staticBuffer);
    if (symbolsNotSerialized == 0)
    {
        for (uint32_t i = 0; i < interfaceCount; ++i)
        {
            currentNode->sibling = calloc(1, sizeof(struct TreeNode));
            currentNode->sibling->parent = currentNode->parent;
            currentNode = currentNode->sibling;
            currentNode->description = getMetaClassDescriptionByIndex(Symbol);
            currentNode->description->read(stream, currentNode, flags);
        }
    }

    return 0;
}

int AnimationRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 0, .memberName = "mVersion", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 0, .memberName = "mFlags", .metaClassDescriptionIndex = Flags},
        {.isBlocked = 0, .memberName = "mName", .metaClassDescriptionIndex = Symbol},
        {.isBlocked = 0, .memberName = "mLength", .metaClassDescriptionIndex = float_type},
        {.isBlocked = 0, .memberName = "mAdditiveMask", .metaClassDescriptionIndex = float_type},
        {.isBlocked = 0, .memberName = "mToolProps", .metaClassDescriptionIndex = ToolProps},
    };
    genericRead(stream, node, flags, 6, descriptions);

    struct TreeNode *currentNode = node->child->sibling->sibling->sibling->sibling->sibling;
    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;

    currentNode->memberName = "animationCore";
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    currentNode->isBlocked = 1;
    //  TODO: Set description
    AnimationCoreRead(stream, currentNode, flags);

    return 0;
}