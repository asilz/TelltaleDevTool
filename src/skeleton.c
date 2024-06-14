#include <inttypes.h>
#include <tree.h>
#include <meta.h>
#include <types.h>
#include <stream.h>
#include <container.h>
#include <stdlib.h>

int TransformRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 2;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->description = getMetaClassDescriptionByIndex(Quaternion);
    node->children[0]->parent = node;
    node->children[0]->description->read(stream, node->children[0], flags);

    node->children[1] = calloc(1, sizeof(struct TreeNode));
    node->children[1]->description = getMetaClassDescriptionByIndex(Vector3);
    node->children[1]->parent = node;
    node->children[1]->description->read(stream, node->children[1], flags);

    return 0;
}

int TRange_float_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->dataSize = 2 * sizeof(float);
    fread(node->data.staticBuffer, node->dataSize, 1, stream);

    return 0;
}

int SArray_TRange_float_3_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 3;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
        node->children[i]->description = getMetaClassDescriptionByIndex(TRange_float_);
        node->children[i]->description->read(stream, node->children[i], flags);
    }
    return 0;
}

int BoneContraintsRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 3;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->description = getMetaClassDescriptionByIndex(long_type);
    node->children[0]->parent = node;
    node->children[0]->description->read(stream, node->children[0], flags);

    node->children[1] = calloc(1, sizeof(struct TreeNode));
    node->children[1]->description = getMetaClassDescriptionByIndex(Vector3);
    node->children[1]->parent = node;
    node->children[1]->description->read(stream, node->children[1], flags);

    node->children[2] = calloc(1, sizeof(struct TreeNode));
    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    node->children[2]->isBlocked = 1;
    node->children[2]->description = getMetaClassDescriptionByIndex(SArray_TRange_float_3_);
    node->children[2]->parent = node;
    node->children[2]->description->read(stream, node->children[2], flags);

    return 0;
}

int SkeletonEntryRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 14;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    node->children[0]->description = getMetaClassDescriptionByIndex(Symbol);
    node->children[0]->description->read(stream, node->children[0], flags);

    node->children[1]->description = getMetaClassDescriptionByIndex(Symbol);
    node->children[1]->description->read(stream, node->children[1], flags);

    node->children[2]->description = getMetaClassDescriptionByIndex(long_type);
    node->children[2]->description->read(stream, node->children[2], flags);

    node->children[3]->description = getMetaClassDescriptionByIndex(Symbol);
    node->children[3]->description->read(stream, node->children[3], flags);

    node->children[4]->description = getMetaClassDescriptionByIndex(long_type);
    node->children[4]->description->read(stream, node->children[4], flags);

    node->children[5]->description = getMetaClassDescriptionByIndex(Vector3);
    node->children[5]->description->read(stream, node->children[5], flags);

    node->children[6]->description = getMetaClassDescriptionByIndex(Quaternion);
    node->children[6]->description->read(stream, node->children[6], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[7]->isBlocked = 1;
    node->children[7]->description = getMetaClassDescriptionByIndex(Transform);
    node->children[7]->description->read(stream, node->children[7], flags);

    node->children[8]->description = getMetaClassDescriptionByIndex(Vector3);
    node->children[8]->description->read(stream, node->children[8], flags);

    node->children[9]->description = getMetaClassDescriptionByIndex(Vector3);
    node->children[9]->description->read(stream, node->children[9], flags);

    node->children[10]->description = getMetaClassDescriptionByIndex(Vector3);
    node->children[10]->description->read(stream, node->children[10], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[11]->isBlocked = 1;
    node->children[11]->description = getMetaClassDescriptionByIndex(Map_Symbolfloatless_Symbol__);
    node->children[11]->description->read(stream, node->children[11], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[12]->isBlocked = 1;
    node->children[12]->description = getMetaClassDescriptionByIndex(BoneContraints);
    node->children[12]->description->read(stream, node->children[12], flags);

    node->children[13]->description = getMetaClassDescriptionByIndex(Flags);
    node->children[13]->description->read(stream, node->children[13], flags);

    return 0;
}

int DCArray_Skeleton__Entry_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(Skeleton__Entry));
}

int SkeletonRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 1;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->parent = node;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->isBlocked = 1;
    node->children[0]->description = getMetaClassDescriptionByIndex(DCArray_Skeleton__Entry_);
    node->children[0]->description->read(stream, node->children[0], flags);

    return 0;
}