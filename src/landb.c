#include <tree.h>
#include <ttstring.h>
#include <intrinsic.h>
#include <stream.h>
#include <stdlib.h>
#include <container.h>

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
    node->children[0]->typeSymbol = 0xcd9c6e605f5af4b4; // crc64 of "String"
    StringRead(stream, node->children[0], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[1]->typeSymbol = 0xcd9c6e605f5af4b4; // crc64 of "String"
    StringRead(stream, node->children[1], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[2]->typeSymbol = 0xfce72eb881aeafb7; // crc64 of "LocalizeInfo"
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

    node->children[0]->typeSymbol = 0x4f023463d89fb0; // crc64 of "Symbol"
    intrinsic8Read(stream, node->children[0], flags);

    node->children[1]->typeSymbol = 0xfc96dfdede31fb4a; // crc64 of "unsignedlong"
    intrinsic4Read(stream, node->children[1], flags);

    node->children[2]->typeSymbol = 0xfc96dfdede31fb4a; // crc64 of "unsignedlong"
    intrinsic4Read(stream, node->children[2], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    node->children[3]->isBlocked = 1;
    node->children[3]->typeSymbol = 0x3057c880e751d8c5; // crc64 of "Handle<Animation>"
    intrinsic8Read(stream, node->children[3], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    node->children[4]->isBlocked = 1;
    node->children[4]->typeSymbol = 0xdf2c74713a04dda7; // crc64 of "Handle<SoundData>"
    intrinsic8Read(stream, node->children[4], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    node->children[5]->isBlocked = 1;
    node->children[5]->typeSymbol = 0xa87b9875c485db; // crc64 of "DCArray<LanguageResLocal>"
    genericArrayRead(stream, node->children[5], flags, LanguageResLocalRead, 0x5f637a07408dbc60);

    node->children[6]->typeSymbol = 0xbae4cbd77f139a91; // crc64 of "float"
    intrinsic4Read(stream, node->children[6], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    node->children[7]->isBlocked = 1;
    node->children[7]->typeSymbol = 0x5f637a07408dbc60; // crc64 of "LanguageResLocal"
    LanguageResLocalRead(stream, node->children[7], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    node->children[8]->isBlocked = 1;
    node->children[8]->typeSymbol = 0x7edcd6db33a5dc53; // crc64 of "RecordingUtils::EnumRecordingStatus"
    intrinsic4Read(stream, node->children[8], flags);

    node->children[9]->typeSymbol = 0x84283cb979d71641; // crc64 of "Flags"
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
    node->children[0]->typeSymbol = 0x33c56d9e4438fcd9; // crc64 of "Map<Symbol,int,less<Symbol>>"
    genericMapRead(stream, node->children[0], flags, intrinsic8Read, intrinsic4Read, 0x4f023463d89fb0, 0x99d7c52ea7f0f97d);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    node->children[1]->isBlocked = 1;
    node->children[1]->typeSymbol = 0x4cc03bef63487679; // crc64 of "Map<int,Symbol,less<int>>"
    genericMapRead(stream, node->children[1], flags, intrinsic4Read, intrinsic8Read, 0x99d7c52ea7f0f97d, 0x4f023463d89fb0);

    node->children[2]->typeSymbol = 0x9004c5587575d6c0; // crc64 of "bool"
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

    node->children[0]->typeSymbol = 0xfc96dfdede31fb4a; // crc64 of "unsignedlong"
    intrinsic4Read(stream, node->children[0], flags);

    node->children[1]->typeSymbol = 0x101e4bf52a9999ac; // crc64 of "long"
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
        node->children[i]->typeSymbol = 0xcd9c6e605f5af4b4; // crc64 of "String"
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
    node->children[0]->typeSymbol = 0x48588668f74fed9f; // crc64 of "UID::Owner"
    intrinsic4Read(stream, node->children[0], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    node->children[1]->isBlocked = 1;
    node->children[1]->typeSymbol = 0x4bd04e27e6716ea0; // crc64 of "UID::Generator"
    intrinsic4Read(stream, node->children[1], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    node->children[2]->isBlocked = 1;
    node->children[2]->typeSymbol = 0xbfd0071071629e03; // crc64 of "Map<unsignedint,LanguageRes,less<unsignedint>>"
    genericMapRead(stream, node->children[2], flags, intrinsic4Read, LanguageResRead, 0xcb029403b12d08d6 /*crc64 of "unsignedint"*/, 0x3be17704e64cebda /*crc64 of "LanguageRes"*/);
    printf("map size = %d\n", *(uint32_t *)(node->children[2]->children[0]->data.staticBuffer));

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    node->children[3]->isBlocked = 1;
    node->children[3]->typeSymbol = 0xaf292b29059f879b; // crc64 of "LocalizationRegistry"
    LocalizationRegistryRead(stream, node->children[3], flags);

    node->children[4]->typeSymbol = 0x84283cb979d71641; // crc64 of "Flags"
    intrinsic4Read(stream, node->children[4], flags);

    node->children[5]->typeSymbol = 0xfc96dfdede31fb4a; // crc64 of "unsignedlong"
    intrinsic4Read(stream, node->children[5], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    node->children[6]->isBlocked = 1;
    node->children[6]->typeSymbol = 0x30694adb68e78e3d;                               // crc64 of "DCArray<ProjectDatabaseIDPair>"
    genericArrayRead(stream, node->children[6], flags, ProjectDatabaseIDPairRead, 0); // TODO: Set symbol?

    node->children[7]->typeSymbol = 0; // TODO: Set symbol
    node->children[7]->childCount = *(uint32_t *)(node->children[2]->children[0]->data.staticBuffer);
    node->children[7]->children = malloc(node->children[7]->childCount * sizeof(struct TreeNode *));
    for (uint16_t i = 0; i < node->children[7]->childCount; ++i)
    {
        node->children[7]->children[i] = calloc(1, sizeof(struct TreeNode)); // TODO: Set symbol
        node->children[7]->children[i]->parent = node->children[7];
        LangAnmWavTripletRead(stream, node->children[7]->children[i], flags);
    }

    return 0;
}