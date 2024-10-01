#ifndef TREE_H
#define TREE_H
#include <inttypes.h>
#include <stdio.h>

struct TreeNode;

typedef int (*SerializeFunction)(FILE *stream, struct TreeNode *node, uint32_t flags);
typedef int (*RenderFunction)(struct TreeNode *node, uint32_t flags);

struct MetaClassDescription
{
    uint64_t crc;
    const char *name;
    const SerializeFunction read;
    const RenderFunction render;
};

struct MetaMemberDescription
{
    const char *memberName;
    uint16_t metaClassDescriptionIndex;
    uint8_t isBlocked;
};

/** @struct TreeNode
 * @brief This is an n-ary tree. All telltale data types are serialized into this data structure. Each child represents a member of the data type usually. Which data type is serialized into the node is determined by the description. If the data type is intrinsic, then staticBuffer or dynamicBuffer is used to store the values
 */
struct TreeNode
{
    union
    {
        uint8_t *dynamicBuffer;
        uint8_t staticBuffer[sizeof(uint8_t *)];
        struct TreeNode *child;
    };
    const char *memberName;
    const struct MetaClassDescription *description;
    struct TreeNode *parent;
    struct TreeNode *sibling;

    uint32_t dataSize;
    uint8_t isBlocked;
    uint8_t serializeType;
    // TODO: Spend last 2 bytes
};

void treeFree(struct TreeNode *root);
uint32_t writeTree(FILE *stream, const struct TreeNode *root);

#endif