#include <stdlib.h>
#include <tree.h>
#include <ttstring.h>
#include <intrinsic.h>
#include <prop.h>
#include <dlg.h>
#include <types.h>
#include <meta.h>
#include <container.h>
#include <stream.h>
#include <string.h>

int Map_StringClipResourceFilterStringCompareCaseInsensitive_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericMapRead(stream, node, flags, getMetaClassDescriptionByIndex(String), getMetaClassDescriptionByIndex(ClipResourceFilter));
}

int Set_StringStringCompareCaseInsensitive_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(String));
}

int DCArray_Handle_Chore__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(Handle_Chore_));
}

int Map_SymbolWalkPathless_Symbol__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericMapRead(stream, node, flags, getMetaClassDescriptionByIndex(Symbol), getMetaClassDescriptionByIndex(WalkPath));
}

int PerAgentClipResourceFilterRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 3;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->isBlocked = 1;
    node->children[0]->description = getMetaClassDescriptionByIndex(Map_StringClipResourceFilterStringCompareCaseInsensitive_);
    node->children[0]->description->read(stream, node->children[0], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[1]->isBlocked = 1;
    node->children[1]->description = getMetaClassDescriptionByIndex(Set_StringStringCompareCaseInsensitive_);
    node->children[1]->description->read(stream, node->children[1], flags);

    node->children[2]->description = getMetaClassDescriptionByIndex(bool_type);
    node->children[2]->description->read(stream, node->children[2], flags);

    return 0;
}

int WalkPathRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 2;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->parent = node;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->isBlocked = 1;
    node->children[0]->description = getMetaClassDescriptionByIndex(String);
    node->children[0]->description->read(stream, node->children[0], flags);

    node->children[1] = calloc(1, sizeof(struct TreeNode));
    node->children[1]->parent = node;

    node->children[1]->childCount = 1;
    node->children[1]->children = malloc(node->children[1]->childCount * sizeof(struct TreeNode *));

    node->children[1]->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[1]->children[0]->parent = node;
    node->children[1]->children[0]->description = getMetaClassDescriptionByIndex(int_type);
    node->children[1]->children[0]->description->read(stream, node->children[1]->children[0], flags);

    node->children[1]->childCount += *(uint32_t *)node->children[1]->children[0]->data.staticBuffer;
    node->children[1]->children = realloc(node->children[1]->children, node->children[1]->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 1; i < node->children[1]->childCount; ++i)
    {
        node->children[1]->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[1]->children[i]->parent = node->children[1];

        uint64_t typeSymbol;
        fread(&typeSymbol, sizeof(typeSymbol), 1, stream);
        node->children[1]->children[i]->description = getMetaClassDescriptionBySymbol(typeSymbol);
        node->children[1]->children[i]->serializeType = 1;
        node->children[1]->children[i]->description->read(stream, node->children[1]->children[i], flags);
    }

    return 0;
}

int AutoActStatusRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 1;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->parent = node;
    node->children[0]->description = getMetaClassDescriptionByIndex(int_type);
    node->children[0]->description->read(stream, node->children[0], flags);

    return 0;
}

int HandleBaseRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->dataSize = sizeof(uint64_t);
    fread(node->data.staticBuffer, sizeof(uint64_t), 1, stream);

    if (*(uint64_t *)node->data.staticBuffer != 0)
    {
        node->dataSize += sizeof(uint64_t);
        uint8_t *data = malloc(node->dataSize);
        if (data == NULL)
        {
            exit(-1);
        }
        memcpy(data, node->data.staticBuffer, sizeof(node->data.staticBuffer));
        fread(data + 8, sizeof(uint64_t), 1, stream);
        node->data.dynamicBuffer = data;
    }

    return 0;
}

int ChoreAgentAttachmentRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 7;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    node->children[0]->description = getMetaClassDescriptionByIndex(bool_type);
    node->children[0]->description->read(stream, node->children[0], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[1]->isBlocked = 1;
    node->children[1]->description = getMetaClassDescriptionByIndex(String);
    node->children[1]->description->read(stream, node->children[1], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[2]->isBlocked = 1;
    node->children[2]->description = getMetaClassDescriptionByIndex(String);
    node->children[2]->description->read(stream, node->children[2], flags);

    node->children[3]->description = getMetaClassDescriptionByIndex(Vector3);
    node->children[3]->description->read(stream, node->children[3], flags);

    node->children[4]->description = getMetaClassDescriptionByIndex(Quaternion);
    node->children[4]->description->read(stream, node->children[4], flags);

    node->children[5]->description = getMetaClassDescriptionByIndex(bool_type);
    node->children[5]->description->read(stream, node->children[5], flags);

    node->children[6]->description = getMetaClassDescriptionByIndex(bool_type);
    node->children[6]->description->read(stream, node->children[6], flags);

    return 0;
}

int DCArray_int_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(int_type));
}

int ChoreAgentRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 6;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->isBlocked = 1;
    node->children[0]->description = getMetaClassDescriptionByIndex(String);
    node->children[0]->description->read(stream, node->children[0], flags);

    node->children[1]->description = getMetaClassDescriptionByIndex(Flags);
    node->children[1]->description->read(stream, node->children[1], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[2]->isBlocked = 1;
    node->children[2]->description = getMetaClassDescriptionByIndex(DCArray_int_);
    node->children[2]->description->read(stream, node->children[2], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[3]->isBlocked = 1;
    node->children[3]->description = getMetaClassDescriptionByIndex(ChoreAgent__Attachment);
    node->children[3]->description->read(stream, node->children[3], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[4]->isBlocked = 1;
    node->children[4]->description = getMetaClassDescriptionByIndex(ActorAgentBinding);
    node->children[4]->description->read(stream, node->children[4], flags);

    fseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[5]->isBlocked = 1;
    node->children[5]->description = getMetaClassDescriptionByIndex(Rule);
    node->children[5]->description->read(stream, node->children[5], flags);

    return 0;
}

int ChoreResourceBlockRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->dataSize = sizeof(float) * 3 + sizeof(uint8_t);
    node->data.dynamicBuffer = malloc(node->dataSize);
    fread(node->data.dynamicBuffer, node->dataSize, 1, stream);

    return 0;
}

int DCArray_ChoreResource__Block_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(ChoreResource__Block));
}

int Map_Symbolfloatless_Symbol__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericMapRead(stream, node, flags, getMetaClassDescriptionByIndex(Symbol), getMetaClassDescriptionByIndex(float_type));
}

int ChoreResourceRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 20;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    node->children[0]->description = getMetaClassDescriptionByIndex(int_type);
    node->children[0]->description->read(stream, node->children[0], flags);

    node->children[1]->description = getMetaClassDescriptionByIndex(Symbol);
    node->children[1]->description->read(stream, node->children[1], flags);

    node->children[2]->description = getMetaClassDescriptionByIndex(float_type);
    node->children[2]->description->read(stream, node->children[2], flags);

    node->children[3]->description = getMetaClassDescriptionByIndex(int_type);
    node->children[3]->description->read(stream, node->children[3], flags);

    node->children[4]->description = getMetaClassDescriptionByIndex(Flags);
    node->children[4]->description->read(stream, node->children[4], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[5]->isBlocked = 1;
    node->children[5]->description = getMetaClassDescriptionByIndex(String);
    node->children[5]->description->read(stream, node->children[5], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[6]->isBlocked = 1;
    node->children[6]->description = getMetaClassDescriptionByIndex(HandleBase);
    node->children[6]->description->read(stream, node->children[6], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[7]->isBlocked = 1;
    node->children[7]->description = getMetaClassDescriptionByIndex(Animation);
    node->children[7]->description->read(stream, node->children[7], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[8]->isBlocked = 1;
    node->children[8]->description = getMetaClassDescriptionByIndex(DCArray_ChoreResource__Block_);
    node->children[8]->description->read(stream, node->children[8], flags);

    node->children[9]->description = getMetaClassDescriptionByIndex(bool_type);
    node->children[9]->description->read(stream, node->children[9], flags);

    node->children[10]->description = getMetaClassDescriptionByIndex(bool_type);
    node->children[10]->description->read(stream, node->children[10], flags);

    node->children[11]->description = getMetaClassDescriptionByIndex(bool_type);
    node->children[11]->description->read(stream, node->children[11], flags);

    node->children[12]->description = getMetaClassDescriptionByIndex(bool_type);
    node->children[12]->description->read(stream, node->children[12], flags);

    node->children[13]->description = getMetaClassDescriptionByIndex(bool_type);
    node->children[13]->description->read(stream, node->children[13], flags);

    node->children[14]->description = getMetaClassDescriptionByIndex(bool_type);
    node->children[14]->description->read(stream, node->children[14], flags);

    node->children[15]->description = getMetaClassDescriptionByIndex(bool_type);
    node->children[15]->description->read(stream, node->children[15], flags);

    node->children[16]->description = getMetaClassDescriptionByIndex(bool_type);
    node->children[16]->description->read(stream, node->children[16], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[17]->isBlocked = 1;
    node->children[17]->description = getMetaClassDescriptionByIndex(PropertySet);
    node->children[17]->description->read(stream, node->children[17], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[18]->isBlocked = 1;
    node->children[18]->description = getMetaClassDescriptionByIndex(Map_Symbolfloatless_Symbol__);
    node->children[18]->description->read(stream, node->children[18], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[19]->isBlocked = 1;
    node->children[19]->description = getMetaClassDescriptionByIndex(AutoActStatus);
    node->children[19]->description->read(stream, node->children[19], flags);

    if (*(node->children[10]->data.staticBuffer) == '1')
    {
        node->childCount += 3;
        node->children = realloc(node->children, node->childCount * sizeof(struct TreeNode *));

        for (uint16_t i = 20; i < node->childCount; ++i)
        {
            node->children[i] = calloc(1, sizeof(struct TreeNode));
            node->children[i]->parent = node;
        }

        node->children[20]->description = getMetaClassDescriptionByIndex(Symbol);
        node->children[20]->description->read(stream, node->children[20], flags);

        node->children[21]->description = getMetaClassDescriptionByIndex(Symbol);
        node->children[21]->description->read(stream, node->children[21], flags);

        node->children[22]->description = getMetaClassDescriptionBySymbol(*(uint64_t *)node->children[21]->data.staticBuffer);
        node->children[22]->description->read(stream, node->children[22], flags);
    }

    return 0;
}

int ChoreRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 5;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // skip block
    node->children[0]->isBlocked = 1;
    node->children[0]->description = getMetaClassDescriptionByIndex(String);
    StringRead(stream, node->children[0], flags);

    node->children[1]->description = getMetaClassDescriptionByIndex(Flags);
    intrinsic4Read(stream, node->children[1], flags);

    node->children[2]->description = getMetaClassDescriptionByIndex(float_type);
    intrinsic4Read(stream, node->children[2], flags);

    node->children[3]->description = getMetaClassDescriptionByIndex(long_type);
    intrinsic4Read(stream, node->children[3], flags);

    node->children[4]->description = getMetaClassDescriptionByIndex(long_type);
    intrinsic4Read(stream, node->children[4], flags);

    node->childCount = 12 + *(uint32_t *)(node->children[3]->data.staticBuffer) + *(uint32_t *)(node->children[4]->data.staticBuffer);
    node->children = realloc(node->children, node->childCount * sizeof(struct TreeNode *)); // Allocating space for agents and resources

    for (uint16_t i = 5; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // skip block
    node->children[5]->isBlocked = 1;
    node->children[5]->description = getMetaClassDescriptionByIndex(PropertySet);
    PropRead(stream, node->children[5], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // skip block
    node->children[6]->isBlocked = 1;
    node->children[6]->description = getMetaClassDescriptionByIndex(String);
    StringRead(stream, node->children[6], flags);

    node->children[7]->description = getMetaClassDescriptionByIndex(long_type);
    intrinsic4Read(stream, node->children[7], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // skip block
    node->children[8]->isBlocked = 1;
    node->children[8]->description = getMetaClassDescriptionByIndex(LocalizeInfo);
    intrinsic4Read(stream, node->children[8], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // skip block
    node->children[9]->isBlocked = 1;
    node->children[9]->description = getMetaClassDescriptionByIndex(DependencyLoader_1_);
    DependencyLoaderRead(stream, node->children[9], flags);

    node->children[10]->description = getMetaClassDescriptionByIndex(ToolProps);
    intrinsic1Read(stream, node->children[10], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // skip block
    node->children[11]->isBlocked = 1;
    node->children[11]->description = getMetaClassDescriptionByIndex(Map_SymbolWalkPathless_Symbol__);
    node->children[11]->description->read(stream, node->children[11], flags);

    uint16_t index = 12;
    for (; index < 12 + *(uint32_t *)(node->children[3]->data.staticBuffer); ++index)
    {
        node->children[index]->description = getMetaClassDescriptionByIndex(ChoreResource);
        node->children[index]->description->read(stream, node->children[index], flags);
    }

    for (; index < node->childCount; ++index)
    {
        node->children[index]->description = getMetaClassDescriptionByIndex(ChoreAgent);
        node->children[index]->description->read(stream, node->children[index], flags);
    }

    return 0;
}
