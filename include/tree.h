#ifndef TREE_H
#define TREE_H
#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>

struct TreeNode;

typedef int (*SerializeFunction)(FILE *stream, struct TreeNode *node, uint32_t flags);
typedef int (*RenderFunction)(struct TreeNode *node, uint32_t flags);

struct MetaClassDescription
{
    const uint64_t crc;
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

#define TREE_NODE_ADD_CHILD(tree_, metaClassDescriptionIndex_, name_, isBlocked_) TREE_NODE_ADD_CHILD_##isBlocked_##(tree_, metaClassDescriptionIndex_, name_)

#define TREE_NODE_ADD_CHILD_0(tree_, metaClassDescriptionIndex_, name_)                  \
    do                                                                                   \
    {                                                                                    \
        tree_->sibling = calloc(1, sizeof(struct TreeNode));                             \
        tree_->sibling->parent = tree_->parent;                                          \
        tree_ = tree_->sibling;                                                          \
        tree_->memberName = name_;                                                       \
        tree_->description = getMetaClassDescriptionByIndex(metaClassDescriptionIndex_); \
        tree_->description->read(stream, tree_, flags);                                  \
    } while (0)

#define TREE_NODE_ADD_CHILD_1(tree_, metaClassDescriptionIndex_, name_)                  \
    do                                                                                   \
    {                                                                                    \
        tree_->sibling = calloc(1, sizeof(struct TreeNode));                             \
        tree_->sibling->parent = tree_->parent;                                          \
        tree_ = tree_->sibling;                                                          \
        tree_->isBlocked = 1;                                                            \
        cfseek(stream, sizeof(uint32_t), SEEK_CUR);                                      \
        tree_->memberName = name_;                                                       \
        tree_->description = getMetaClassDescriptionByIndex(metaClassDescriptionIndex_); \
        tree_->description->read(stream, tree_, flags);                                  \
    } while (0)

/** @struct TreeNode
 * @brief All telltale data types are serialized into this data structure. Each child represents a member of the data type usually. Which data type is serialized into the node is determined by the description. If the data type is intrinsic, then data is used to store the values
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
};

void treeFree(struct TreeNode *restrict root);
uint32_t writeTree(FILE *stream, const struct TreeNode *restrict root);
void treePushBack(struct TreeNode *restrict tree, struct TreeNode *restrict child);
struct TreeNode *copyTree(struct TreeNode *restrict tree);
void treeErase(struct TreeNode *tree, uint16_t childIndex);

#endif