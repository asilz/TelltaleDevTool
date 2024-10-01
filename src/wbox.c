#include <tree.h>
#include <stream.h>
#include <container.h>
#include <stdlib.h>
#include <types.h>
#include <meta.h>

static int WalkBoxes__EdgeRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    (void)flags;
    node->dataSize = 5 * sizeof(int32_t) + sizeof(float);
    node->dynamicBuffer = malloc(node->dataSize);
    fread(node->dynamicBuffer, node->dataSize, 1, stream); // TODO: Maybe change to children

    return 0;
}

static int SArray_int3_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    (void)flags;
    node->dataSize = 3 * sizeof(int32_t);
    node->dynamicBuffer = malloc(node->dataSize);
    fread(node->dynamicBuffer, node->dataSize, 1, stream);

    return 0;
}

static int SArray_int4_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    (void)flags;
    node->dataSize = 4 * sizeof(int32_t);
    node->dynamicBuffer = malloc(node->dataSize);
    fread(node->dynamicBuffer, node->dataSize, 1, stream);

    return 0;
}

static int SArray_float3_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    (void)flags;
    node->dataSize = 3 * sizeof(float);
    node->dynamicBuffer = malloc(node->dataSize);
    fread(node->dynamicBuffer, node->dataSize, 1, stream);

    return 0;
}

static int SArray_WalkBoxes__Edge3_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->child = calloc(1, sizeof(struct TreeNode));
    node->child->parent = node;
    struct TreeNode *currentNode = node->child;
    WalkBoxes__EdgeRead(stream, currentNode, flags);

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;
    WalkBoxes__EdgeRead(stream, currentNode, flags);

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;
    WalkBoxes__EdgeRead(stream, currentNode, flags);

    return 0;
}

static int WalkBoxes__TriRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->child = calloc(1, sizeof(struct TreeNode));
    struct TreeNode *currentNode = node->child;
    currentNode->parent = node;
    currentNode->memberName = "mFootstepMaterial";
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    currentNode->isBlocked = 1;
    currentNode->description = getMetaClassDescriptionByIndex(long_type); // SoundFootsteps::EnumMaterial
    currentNode->description->read(stream, currentNode, flags);

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;
    currentNode->memberName = "mFlags";
    currentNode->description = getMetaClassDescriptionByIndex(Flags);
    currentNode->description->read(stream, currentNode, flags);

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;
    currentNode->memberName = "mNormal";
    currentNode->description = getMetaClassDescriptionByIndex(long_type);
    currentNode->description->read(stream, currentNode, flags);

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;
    currentNode->memberName = "mQuadBuddy";
    currentNode->description = getMetaClassDescriptionByIndex(long_type);
    currentNode->description->read(stream, currentNode, flags);

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;
    currentNode->memberName = "mMaxRadius";
    currentNode->description = getMetaClassDescriptionByIndex(long_type);
    currentNode->description->read(stream, currentNode, flags);

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    currentNode->isBlocked = 1;
    currentNode->memberName = "mVerts";
    SArray_int3_Read(stream, currentNode, flags);

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    currentNode->isBlocked = 1;
    currentNode->memberName = "mEdgeInfo";
    SArray_WalkBoxes__Edge3_Read(stream, currentNode, flags);

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    currentNode->isBlocked = 1;
    currentNode->memberName = "mVertOffsets";
    SArray_int3_Read(stream, currentNode, flags);

    currentNode->sibling = calloc(1, sizeof(struct TreeNode));
    currentNode->sibling->parent = currentNode->parent;
    currentNode = currentNode->sibling;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    currentNode->isBlocked = 1;
    currentNode->memberName = "mVertScales";
    SArray_float3_Read(stream, currentNode, flags);

    return 0;
}

static int WalkBoxes__VertRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 0, .memberName = "mFlags", .metaClassDescriptionIndex = Flags},
        {.isBlocked = 0, .memberName = "mPos", .metaClassDescriptionIndex = Vector3},
    };
    return genericRead(stream, node, flags, 2, descriptions);
}

static int WalkBoxes__QuadRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->child = calloc(1, sizeof(struct TreeNode));
    struct TreeNode *currentNode = node->child;
    currentNode->parent = node;
    currentNode->memberName = "mVerts";
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    currentNode->isBlocked = 1;
    SArray_int4_Read(stream, currentNode, flags);

    return 0;
}

int DCArray_WalkBoxes__Tri_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
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
        WalkBoxes__TriRead(stream, currentNode, flags);
    }
    return 0;
}

int DCArray_WalkBoxes__Vert_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
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
        WalkBoxes__VertRead(stream, currentNode, flags);
    }
    return 0;
}

int DCArray_WalkBoxes__Quad_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
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
        WalkBoxes__QuadRead(stream, currentNode, flags);
    }
    return 0;
}

int DCArray_Vector3_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(Vector3));
}

int WalkBoxesRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "mName", .metaClassDescriptionIndex = String},
        {.isBlocked = 1, .memberName = "mTris", .metaClassDescriptionIndex = DCArray_WalkBoxes__Tri_},
        {.isBlocked = 1, .memberName = "mVerts", .metaClassDescriptionIndex = DCArray_WalkBoxes__Vert_},
        {.isBlocked = 1, .memberName = "mNormals", .metaClassDescriptionIndex = DCArray_Vector3_},
        {.isBlocked = 1, .memberName = "mQuads", .metaClassDescriptionIndex = DCArray_WalkBoxes__Quad_},
    };
    return genericRead(stream, node, flags, 5, descriptions);
}
