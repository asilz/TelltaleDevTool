#include <tree.h>
#include <ttstring.h>
#include <intrinsic.h>
#include <stream.h>
#include <stdlib.h>
#include <container.h>
#include <types.h>
#include <meta.h>

int LanguageResLocalRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 3;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
        node->children[i]->isBlocked = 1;
    }

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->description = getMetaClassDescriptionByIndex(String);
    StringRead(stream, node->children[0], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[1]->description = getMetaClassDescriptionByIndex(String);
    StringRead(stream, node->children[1], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[2]->description = getMetaClassDescriptionByIndex(LocalizeInfo);
    intrinsic4Read(stream, node->children[2], flags);

    return 0;
}

int LanguageResRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 10;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    node->children[0]->description = getMetaClassDescriptionByIndex(Symbol);
    intrinsic8Read(stream, node->children[0], flags);

    node->children[1]->description = getMetaClassDescriptionByIndex(unsignedlong);
    intrinsic4Read(stream, node->children[1], flags);

    node->children[2]->description = getMetaClassDescriptionByIndex(unsignedlong);
    intrinsic4Read(stream, node->children[2], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    node->children[3]->isBlocked = 1;
    node->children[3]->description = getMetaClassDescriptionByIndex(Handle_Animation_);
    intrinsic8Read(stream, node->children[3], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    node->children[4]->isBlocked = 1;
    node->children[4]->description = getMetaClassDescriptionByIndex(Handle_SoundData_);
    intrinsic8Read(stream, node->children[4], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    node->children[5]->isBlocked = 1;
    node->children[5]->description = getMetaClassDescriptionByIndex(DCArray_LanguageResLocal_);
    genericArrayRead(stream, node->children[5], flags, getMetaClassDescriptionByIndex(LanguageResLocal));

    node->children[6]->description = getMetaClassDescriptionByIndex(float_type);
    intrinsic4Read(stream, node->children[6], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    node->children[7]->isBlocked = 1;
    node->children[7]->description = getMetaClassDescriptionByIndex(LanguageResLocal);
    LanguageResLocalRead(stream, node->children[7], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    node->children[8]->isBlocked = 1;
    node->children[8]->description = getMetaClassDescriptionByIndex(RecordingUtils__EnumRecordingStatus);
    intrinsic4Read(stream, node->children[8], flags);

    node->children[9]->description = getMetaClassDescriptionByIndex(Flags);
    intrinsic4Read(stream, node->children[9], flags);

    return 0;
}

int LocalizationRegistryRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 3;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    node->children[0]->isBlocked = 1;
    node->children[0]->description = getMetaClassDescriptionByIndex(Map_Symbolintless_Symbol__);
    genericMapRead(stream, node->children[0], flags, getMetaClassDescriptionByIndex(Symbol), getMetaClassDescriptionByIndex(int_type));

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    node->children[1]->isBlocked = 1;
    node->children[1]->description = getMetaClassDescriptionByIndex(Map_intSymbolless_int__);
    genericMapRead(stream, node->children[1], flags, getMetaClassDescriptionByIndex(int_type), getMetaClassDescriptionByIndex(Symbol));

    node->children[2]->description = getMetaClassDescriptionByIndex(ToolProps);
    BoolRead(stream, node->children[2], flags);

    return 0;
}

int ProjectDatabaseIDPairRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 2;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    node->children[0]->description = getMetaClassDescriptionByIndex(unsignedlong);
    intrinsic4Read(stream, node->children[0], flags);

    node->children[1]->description = getMetaClassDescriptionByIndex(long_type);
    intrinsic4Read(stream, node->children[1], flags);

    return 0;
}

static int LangAnmWavTripletRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 3;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
        node->children[i]->description = getMetaClassDescriptionByIndex(String);
        StringRead(stream, node->children[i], flags);
    }

    return 0;
}

int LanguageDBRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 8;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    node->children[0]->isBlocked = 1;
    node->children[0]->description = getMetaClassDescriptionByIndex(UID__Owner);
    intrinsic4Read(stream, node->children[0], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    node->children[1]->isBlocked = 1;
    node->children[1]->description = getMetaClassDescriptionByIndex(UID__Generator);
    intrinsic4Read(stream, node->children[1], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    node->children[2]->isBlocked = 1;
    node->children[2]->description = getMetaClassDescriptionByIndex(Map_unsignedintLanguageResless_unsignedint__);
    genericMapRead(stream, node->children[2], flags, getMetaClassDescriptionByIndex(int_type), getMetaClassDescriptionByIndex(LanguageRes));

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    node->children[3]->isBlocked = 1;
    node->children[3]->description = getMetaClassDescriptionByIndex(LocalizationRegistry);
    LocalizationRegistryRead(stream, node->children[3], flags);

    node->children[4]->description = getMetaClassDescriptionByIndex(Flags);
    intrinsic4Read(stream, node->children[4], flags);

    node->children[5]->description = getMetaClassDescriptionByIndex(unsignedlong);
    intrinsic4Read(stream, node->children[5], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    node->children[6]->isBlocked = 1;
    node->children[6]->description = getMetaClassDescriptionByIndex(DCArray_ProjectDatabaseIDPair_);
    genericArrayRead(stream, node->children[6], flags, getMetaClassDescriptionByIndex(ProjectDatabaseIDPair));

    // node->children[7]->typeSymbol = 0; // TODO: Set description
    node->children[7]->childCount = *(uint32_t *)(node->children[2]->children[0]->data.staticBuffer);
    node->children[7]->children = malloc(node->children[7]->childCount * sizeof(struct TreeNode *));
    for (uint16_t i = 0; i < node->children[7]->childCount; ++i)
    {
        node->children[7]->children[i] = calloc(1, sizeof(struct TreeNode)); // TODO: Set description
        node->children[7]->children[i]->parent = node->children[7];
        LangAnmWavTripletRead(stream, node->children[7]->children[i], flags);
    }

    return 0;
}