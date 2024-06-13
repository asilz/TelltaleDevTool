#ifndef TREE_H
#define TREE_H
#include <inttypes.h>
#include <stdio.h>

struct TreeNode;

typedef int (*SerializeFunction)(FILE *stream, struct TreeNode *node, uint32_t flags);
typedef int (*RenderFunction)(struct TreeNode *node, uint32_t flags);

struct MetaClassDescription
{
    const uint64_t crc;
    const char *name;
    SerializeFunction read;
    const RenderFunction render;
};

struct TreeNode
{
    union Data
    {
        uint8_t *dynamicBuffer;
        uint8_t staticBuffer[sizeof(uint8_t *)];
    } data;
    const struct MetaClassDescription *description;
    struct TreeNode *parent;
    struct TreeNode **children;
    uint32_t dataSize;
    uint16_t childCount; // Reason I placed members like so is to avoid padding. Padding is the bane of my existence.
    uint8_t isBlocked;
    uint8_t serializeType;
};

void treeFree(struct TreeNode *root);
uint32_t writeTree(FILE *stream, struct TreeNode *root);
void treePushBack(struct TreeNode *tree, struct TreeNode *child);
struct TreeNode *copyTree(struct TreeNode *tree);
void treeErase(struct TreeNode *tree, uint16_t childIndex);

#endif