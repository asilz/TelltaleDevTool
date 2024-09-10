#include <tree.h>
#include <stream.h>
#include <container.h>
#include <stdlib.h>
#include <types.h>
#include <meta.h>
#include <assert.h>

int bankwavemapRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "mWaveMap", .metaClassDescriptionIndex = Map_SymbolSoundBankWaveMapEntryless_Symbol__},
    };
    return genericRead(stream, node, flags, 1, descriptions);
}

static int SoundBankWaveMapEntryRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 0, .memberName = "fLengthSeconds", .metaClassDescriptionIndex = float_type},
        {.isBlocked = 1, .memberName = "strFileName", .metaClassDescriptionIndex = String},
    };
    return genericRead(stream, node, flags, 1, descriptions);
}

int Map_SymbolSoundBankWaveMapEntryless_Symbol__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->child = malloc(sizeof(struct TreeNode));
    node->child->description = getMetaClassDescriptionByIndex(int_type);
    node->child->description->read(stream, node->child, flags);
    node->child->parent = node;
    node->child->serializeType = 0;
    node->child->memberName = "pairCount";
    node->child->isBlocked = 0;
    node->child->sibling = NULL;

    struct TreeNode *currentNode = node->child;

    for (uint32_t i = 0; i < *(uint32_t *)(node->child->staticBuffer) * 2; ++i)
    {
        currentNode->sibling = calloc(1, sizeof(struct TreeNode));
        currentNode = currentNode->sibling;
        currentNode->parent = node;
        if (i % 2) // if odd
        {
            SoundBankWaveMapEntryRead(stream, currentNode, flags);
        }
        else
        {
            currentNode->description = getMetaClassDescriptionByIndex(Symbol);
            currentNode->description->read(stream, currentNode, flags);
        }
    }
    return 0;
}