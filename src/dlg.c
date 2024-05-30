#include <stdlib.h>
#include <meta.h>
#include <assert.h>
#include <dlg.h>
#include <prop.h>
#include <intrinsic.h>
#include <stddef.h>
#include <container.h>
#include <ttstring.h>
#include <stream.h>

int AnimOrChoreRead(FILE *stream, struct TreeNode *node, uint32_t flags) // TODO: Move this function to a different file
{
    node->childCount = 2;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->parent = node;
    node->children[0]->isBlocked = 1;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->typeSymbol = 0x3057c880e751d8c5; // crc64 of "Handle<Animation>"
    intrinsic8Read(stream, node->children[0], flags);

    node->children[1] = calloc(1, sizeof(struct TreeNode));
    node->children[1]->parent = node;
    node->children[1]->isBlocked = 1;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[1]->typeSymbol = 0x6643af6dcccc81a9; // crc64 of "Handle<Chore>"
    intrinsic8Read(stream, node->children[1], flags);

    return 0;
}

int ScriptEnumRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 1;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->parent = node;
    node->children[0]->isBlocked = 1;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->typeSymbol = 0xcd9c6e605f5af4b4; // crc64 of "String"
    StringRead(stream, node->children[0], flags);

    return 0;
}

int DependencyLoaderRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 1;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->parent = node;
    node->children[0]->typeSymbol = 0x9004c5587575d6c0; // crc64 of "bool"
    BoolRead(stream, node->children[0], flags);

    if (node->children[0]->data.staticBuffer[0] == '1')
    {
        node->childCount = 2;
        node->children = realloc(node->children, node->childCount * sizeof(struct TreeNode *));
        node->children[1] = calloc(1, sizeof(struct TreeNode));
        node->children[1]->serializeType = 1;
        fread(&(node->children[1]->typeSymbol), sizeof(node->children[1]->typeSymbol), 1, stream);
        node->children[1]->parent = node;
        readMetaClass(stream, node->children[1], flags);
    }
    return 0;
}

uint64_t DlgGetNextID(struct TreeNode *node)
{
    if (node->childCount == 0 || node->children == NULL || node->children[0] == NULL)
    {
        printf("Error: Node has no children");
        return 0;
    }
    if (node->children[0]->typeSymbol == 0x8940087148bf4c61) // DlgNode
    {
        return *(uint64_t *)(node->children[0]->children[4]->children[0]->children[0]->data.staticBuffer);
    }
    if (node->children[0]->typeSymbol == 0x915709e72a07b3ed) // DlgChild
    {
        return *(uint64_t *)(node->children[0]->children[0]->children[1]->children[0]->children[0]->data.staticBuffer);
    }
    printf("Error: TreeNode is not a DlgNode/DlgChild");
    return 0;
}

uint64_t DlgGetPrevID(struct TreeNode *node)
{
    if (node->childCount == 0 || node->children == NULL || node->children[0] == NULL)
    {
        printf("Error: Node has no children");
        return 0;
    }
    if (node->children[0]->typeSymbol == 0x8940087148bf4c61) // DlgNode
    {
        return *(uint64_t *)(node->children[0]->children[3]->children[0]->children[0]->data.staticBuffer);
    }
    if (node->children[0]->typeSymbol == 0x915709e72a07b3ed) // DlgChild
    {
        return *(uint64_t *)(node->children[0]->children[3]->children[0]->children[0]->data.staticBuffer);
    }
    printf("Error: TreeNode is not a DlgNode/DlgChild");
    return 0;
}

uint64_t DlgGetID(struct TreeNode *node)
{
    if (node->childCount == 0 || node->children == NULL || node->children[0] == NULL)
    {
        printf("Error: Node has no children");
        return 0;
    }
    if (node->children[0]->typeSymbol == 0x8940087148bf4c61) // DlgNode
    {
        return *(uint64_t *)(node->children[0]->children[0]->children[0]->data.staticBuffer);
    }
    if (node->children[0]->typeSymbol == 0x915709e72a07b3ed) // DlgChild
    {
        return *(uint64_t *)(node->children[0]->children[0]->children[0]->children[0]->data.staticBuffer);
    }
    printf("Error: TreeNode is not a DlgNode/DlgChild");
    return 0;
}

static size_t blockRead(FILE *stream, uint8_t *buffer)
{
    size_t bytesRead = 0;
    bytesRead += fread(buffer, sizeof(uint32_t), 1, stream);
    buffer += bytesRead;
    bytesRead += fread(buffer, (*(uint32_t *)(buffer - sizeof(uint32_t)) - 4), 1, stream);
    return bytesRead;
}

static int DlgFolderRead(FILE *stream, struct TreeNode *folder, uint32_t flags)
{
    folder->childCount = 5;
    folder->children = malloc(folder->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < folder->childCount; ++i)
    {
        folder->children[i] = calloc(1, sizeof(struct TreeNode));
        folder->children[i]->parent = folder;
    }

    folder->children[0]->isBlocked = 1;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    folder->children[0]->typeSymbol = 0x8d53d2745d07232b; // crc64 of "DlgObjIDOwner"
    DlgObjectIDOwnerRead(stream, folder->children[0], flags);

    folder->children[1]->isBlocked = 1;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    folder->children[1]->typeSymbol = 0x96f88556b81a6af; // crc64 of "DlgObjectPropsOwner"
    DlgObjectPropsOwnerRead(stream, folder->children[1], flags);

    folder->children[2]->isBlocked = 1;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    folder->children[2]->typeSymbol = 0x915709e72a07b3ed; // crc64 of "DlgChildSet"
    DlgChildSetRead(stream, folder->children[2], flags);

    folder->children[3]->typeSymbol = 0x4f023463d89fb0; // crc64 of "Symbol"
    intrinsic8Read(stream, folder->children[3], flags);

    folder->children[4]->isBlocked = 1;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    folder->children[4]->typeSymbol = 0xcd75dc4f6b9f15d2; // crc64 of "PropertySet"
    PropRead(stream, folder->children[4], flags);

    return 0;
}

int DlgRead(FILE *stream, struct TreeNode *dlg, uint32_t flags)
{
    dlg->childCount = 16;
    dlg->children = malloc(dlg->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < dlg->childCount; ++i)
    {
        dlg->children[i] = calloc(1, sizeof(struct TreeNode));
        dlg->children[i]->parent = dlg;
    }

    dlg->children[0]->isBlocked = 1;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    dlg->children[0]->typeSymbol = 0x8d53d2745d07232b; // crc64 of "DlgObjIDOwner"
    DlgObjectIDOwnerRead(stream, dlg->children[0], flags);

    dlg->children[1]->isBlocked = 1;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    dlg->children[1]->typeSymbol = 0xcd9c6e605f5af4b4; // crc64 of "String"
    StringRead(stream, dlg->children[1], flags);

    dlg->children[2]->typeSymbol = 0x99d7c52ea7f0f97d; // crc64 of "int"
    intrinsic4Read(stream, dlg->children[2], flags);

    dlg->children[3]->isBlocked = 1;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    dlg->children[3]->typeSymbol = 0x87e0e880d7d2dfc1; // crc64 of "DlgObjID"
    DlgObjectIDRead(stream, dlg->children[3], flags);

    // TODO: Implement landb reading
    uint32_t block;
    dlg->children[4]->isBlocked = 1;
    fread(&block, 1, sizeof(uint32_t), stream);
    dlg->children[4]->typeSymbol = 0x87e0e880d7d2dfc1; // crc64 of "DlgObjID"
    dlg->children[4]->dataSize = block - sizeof(uint32_t);
    dlg->children[4]->data.dynamicBuffer = malloc(dlg->children[4]->dataSize);
    fread(dlg->children[4]->data.dynamicBuffer, dlg->children[4]->dataSize, 1, stream);

    dlg->children[5]->typeSymbol = 0x99d7c52ea7f0f97d; // crc64 of "int"
    intrinsic4Read(stream, dlg->children[5], flags);

    dlg->children[6]->typeSymbol = 0x4f023463d89fb0; // crc64 of "Symbol"
    intrinsic8Read(stream, dlg->children[6], flags);

    dlg->children[7]->typeSymbol = 0x99d7c52ea7f0f97d; // crc64 of "int"
    intrinsic4Read(stream, dlg->children[7], flags);

    dlg->children[8]->typeSymbol = 0x84283cb979d71641; // crc64 of "Flags"
    intrinsic4Read(stream, dlg->children[8], flags);

    dlg->children[9]->isBlocked = 1;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    dlg->children[9]->typeSymbol = 0xefc64d422df9376f; // crc64 of "DependencyLoader<1>"
    DependencyLoaderRead(stream, dlg->children[9], flags);

    dlg->children[10]->isBlocked = 1;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    dlg->children[10]->typeSymbol = 0xcd75dc4f6b9f15d2; // crc64 of "PropertySet"
    PropRead(stream, dlg->children[10], flags);

    dlg->children[11]->isBlocked = 1;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    dlg->children[11]->typeSymbol = 0x9d2d91124150cffb; // crc64 of "JiraRecordManager"

    dlg->children[12]->typeSymbol = 0x9004c5587575d6c0; // crc64 of "bool"
    BoolRead(stream, dlg->children[12], flags);

    dlg->children[13]->typeSymbol = 0; // TODO: Set symbol
    genericArrayRead(stream, dlg->children[13], flags, DlgFolderRead, 0xa7152ced90755212);

    dlg->children[14]->typeSymbol = 0; // TODO: Set Symbol
    dlg->children[14]->childCount = 1;
    dlg->children[14]->children = malloc(dlg->children[14]->childCount * sizeof(struct TreeNode *));
    dlg->children[14]->children[0] = calloc(1, sizeof(struct TreeNode));
    dlg->children[14]->children[0]->typeSymbol = 0x99d7c52ea7f0f97d; // crc64 of "int"
    intrinsic4Read(stream, dlg->children[14]->children[0], flags);
    dlg->children[14]->children[0]->parent = dlg->children[14];

    dlg->children[14]->childCount += *(uint32_t *)(dlg->children[14]->children[0]->data.staticBuffer);
    dlg->children[14]->children = realloc(dlg->children[14]->children, dlg->children[14]->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 1; i < dlg->children[14]->childCount; ++i)
    {
        dlg->children[14]->children[i] = calloc(1, sizeof(struct TreeNode));
        dlg->children[14]->children[i]->serializeType = 1;
        fread(&(dlg->children[14]->children[i]->typeSymbol), sizeof(dlg->children[14]->children[i]->typeSymbol), 1, stream);
        dlg->children[14]->children[i]->parent = dlg->children[14];
        readMetaClass(stream, dlg->children[14]->children[i], flags);
    }

    dlg->children[15]->typeSymbol = 0x9004c5587575d6c0; // crc64 of "bool"
    BoolRead(stream, dlg->children[15], flags);

    printf("Dlg Read\n");
    return 0;
}

int LogicItemRead(FILE *stream, struct TreeNode *logicItem, uint32_t flags)
{
    logicItem->childCount = 6;
    logicItem->children = malloc(logicItem->childCount * sizeof(struct TreeNode *));

    for (uint32_t i = 0; i < logicItem->childCount; ++i)
    {
        logicItem->children[i] = calloc(1, sizeof(struct TreeNode));
        logicItem->children[i]->isBlocked = 1;
        logicItem->children[i]->parent = logicItem;
    }

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);              // Skip block
    logicItem->children[0]->typeSymbol = 0xcd75dc4f6b9f15d2; // crc64 of "PropertySet"
    PropRead(stream, logicItem->children[0], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);              // Skip block
    logicItem->children[1]->typeSymbol = 0xcd9c6e605f5af4b4; // crc64 of "String"
    StringRead(stream, logicItem->children[1], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);             // Skip block
    logicItem->children[2]->typeSymbol = 0x42f3cd4fe3d6d0d; // crc64 of "Map<Symbol,bool,less<Symbol>>"
    genericMapRead(stream, logicItem->children[2], flags, intrinsic8Read, BoolRead, 0x4f023463d89fb0, 0x9004c5587575d6c0);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);              // Skip block
    logicItem->children[3]->typeSymbol = 0x33c56d9e4438fcd9; // crc64 of "Map<Symbol,int,less<Symbol>>"
    genericMapRead(stream, logicItem->children[3], flags, intrinsic8Read, intrinsic4Read, 0x4f023463d89fb0, 0x99d7c52ea7f0f97d);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);              // Skip block
    logicItem->children[4]->typeSymbol = 0x33c56d9e4438fcd9; // crc64 of "Map<Symbol,int,less<Symbol>>"
    genericMapRead(stream, logicItem->children[4], flags, intrinsic8Read, intrinsic4Read, 0x4f023463d89fb0, 0x99d7c52ea7f0f97d);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);              // Skip block
    logicItem->children[5]->typeSymbol = 0x24642a39023f0f34; // crc64 of "DCArray<String>"
    genericArrayRead(stream, logicItem->children[5], flags, StringRead, 0xcd9c6e605f5af4b4);

    return 0;
}

int LogicGroupRead(FILE *stream, struct TreeNode *logicGroup, uint32_t flags) // TODO: Fix to allow general write function
{
    logicGroup->childCount = 6;
    logicGroup->children = malloc(logicGroup->childCount * sizeof(struct TreeNode *));

    for (uint32_t i = 0; i < logicGroup->childCount; ++i)
    {
        logicGroup->children[i] = calloc(1, sizeof(struct TreeNode));
        logicGroup->children[i]->parent = logicGroup;
    }

    logicGroup->children[0]->typeSymbol = 0x99d7c52ea7f0f97d; // crc64 of "int"
    intrinsic4Read(stream, logicGroup->children[0], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    logicGroup->children[1]->isBlocked = 1;
    logicGroup->children[1]->typeSymbol = 0xa98eae8f6cff1125; // crc64 of "Map<String,LogicGroup::LogicItem,less<String>>"
    genericMapRead(stream, logicGroup->children[1], flags, StringRead, LogicItemRead, 0xcd9c6e605f5af4b4, 0xb705c1a4e971420e);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    logicGroup->children[2]->isBlocked = 1;
    logicGroup->children[2]->typeSymbol = 0x36d2e9352e9eed3e; // crc64 of "DCArray<LogicGroup>"
    genericArrayRead(stream, logicGroup->children[2], flags, LogicGroupRead, 0x114636e822613a22);

    logicGroup->children[3]->typeSymbol = 0x99d7c52ea7f0f97d; // crc64 of "int"
    intrinsic4Read(stream, logicGroup->children[3], flags);

    logicGroup->children[4]->typeSymbol = 0x99d7c52ea7f0f97d; // crc64 of "int"
    intrinsic4Read(stream, logicGroup->children[4], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    logicGroup->children[5]->isBlocked = 1;
    logicGroup->children[5]->typeSymbol = 0xcd9c6e605f5af4b4; // crc64 of "string"
    StringRead(stream, logicGroup->children[5], flags);

    return 0;
}

int RuleRead(FILE *stream, struct TreeNode *rule, uint32_t flags)
{
    rule->childCount = 7;
    rule->children = malloc(rule->childCount * sizeof(struct TreeNode *));

    for (uint32_t i = 0; i < rule->childCount; ++i)
    {
        rule->children[i] = calloc(1, sizeof(struct TreeNode));
        rule->children[i]->parent = rule;
    }

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip name block
    rule->children[0]->isBlocked = 1;
    rule->children[0]->typeSymbol = 0xcd9c6e605f5af4b4; // crc64 of "string"
    StringRead(stream, rule->children[0], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip runtimePropName block
    rule->children[1]->isBlocked = 1;
    rule->children[1]->typeSymbol = 0xcd9c6e605f5af4b4; // crc64 of "string"
    StringRead(stream, rule->children[1], flags);

    rule->children[2]->typeSymbol = 0x84283cb979d71641; // crc64 of "Flags"
    intrinsic4Read(stream, rule->children[2], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip conditions block
    rule->children[3]->isBlocked = 1;
    rule->children[3]->typeSymbol = 0x114636e822613a22; // crc64 of "LogicGroup"
    LogicGroupRead(stream, rule->children[3], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip actions block
    rule->children[4]->isBlocked = 1;
    rule->children[4]->typeSymbol = 0x114636e822613a22; // crc64 of "LogicGroup"
    LogicGroupRead(stream, rule->children[4], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip else (nullptr block (heh)) block
    rule->children[5]->isBlocked = 1;
    rule->children[5]->typeSymbol = 0x114636e822613a22; // crc64 of "LogicGroup"
    LogicGroupRead(stream, rule->children[5], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip agentCategoryBlock block
    rule->children[6]->isBlocked = 1;
    rule->children[6]->typeSymbol = 0xcd9c6e605f5af4b4; // crc64 of "string"
    StringRead(stream, rule->children[6], flags);

    return 0;
}

int DlgDownStreamVisibilityConditionsRead(FILE *stream, struct TreeNode *visCond, uint32_t flags)
{
    visCond->dataSize = 2 * sizeof(uint32_t);
    fread(visCond->data.staticBuffer, visCond->dataSize, 1, stream);

    return 0;
}

int DlgVisibilityConditionsRead(FILE *stream, struct TreeNode *visCond, uint32_t flags)
{

    visCond->childCount = 4;
    uint32_t visCondFlags;
    cfseek(stream, sizeof(uint8_t), SEEK_CUR);
    fread(&visCondFlags, sizeof(visCondFlags), 1, stream);
    cfseek(stream, -(long)(sizeof(visCondFlags) + sizeof(uint8_t)), SEEK_CUR);
    if (visCondFlags & 1)
    {
        ++visCond->childCount;
    }
    visCond->children = malloc(visCond->childCount * sizeof(struct TreeNode *));
    for (uint32_t i = 0; i < visCond->childCount; ++i)
    {
        visCond->children[i] = calloc(1, sizeof(struct TreeNode));
        visCond->children[i]->parent = visCond;
    }
    visCond->children[0]->typeSymbol = 0x9004c5587575d6c0; // crc64 of "bool"
    BoolRead(stream, visCond->children[0], flags);

    visCond->children[1]->typeSymbol = 0x84283cb979d71641; // crc64 of "Flags"
    intrinsic4Read(stream, visCond->children[1], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);            // Skip block
    visCond->children[2]->typeSymbol = 0xad550479d1324785; // crc64 of "DlgDownStreamVisibilityConditions"
    DlgDownStreamVisibilityConditionsRead(stream, visCond->children[2], flags);
    visCond->children[2]->isBlocked = 1;

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);            // Skip string block
    visCond->children[3]->typeSymbol = 0xcd9c6e605f5af4b4; // crc64 of "string"
    StringRead(stream, visCond->children[3], flags);
    visCond->children[3]->isBlocked = 1;
    if (visCondFlags & 1)
    {
        visCond->children[4]->typeSymbol = 0x5549d9efe67060e1; // crc64 of "rule"
        RuleRead(stream, visCond->children[4], flags);
    }
    return 0;
}

int DlgVisibilityConditionsOwnerRead(FILE *stream, struct TreeNode *visCond, uint32_t flags)
{
    visCond->childCount = 1;
    visCond->children = malloc(visCond->childCount * sizeof(struct TreeNode *));

    visCond->children[0] = calloc(1, sizeof(struct TreeNode));
    visCond->children[0]->typeSymbol = 0xb54642189b1237ca; // crc64 of "DlgVisibilityConditions";
    visCond->children[0]->parent = visCond;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    visCond->children[0]->isBlocked = 1;
    DlgVisibilityConditionsRead(stream, visCond->children[0], flags);
    return 0;
}

int DlgObjectPropsRead(FILE *stream, struct TreeNode *objectProps, uint32_t flags)
{
    objectProps->childCount = 1;
    objectProps->children = malloc(objectProps->childCount * sizeof(struct TreeNode *));

    objectProps->children[0] = calloc(1, sizeof(struct TreeNode));
    objectProps->children[0]->typeSymbol = 0x84283cb979d71641; // crc64 of "Flags"
    intrinsic4Read(stream, objectProps->children[0], flags);
    objectProps->children[0]->parent = objectProps;

    uint32_t propFlags = *(uint32_t *)(objectProps->children[0]->data.staticBuffer);
    objectProps->childCount += ((propFlags & 0x1) > 0) + ((propFlags & 0x2) > 0) + ((propFlags & 0x4) > 0);
    objectProps->children = realloc(objectProps->children, objectProps->childCount * sizeof(struct TreeNode *));

    for (uint32_t i = 1; i < objectProps->childCount; ++i)
    {
        objectProps->children[i] = calloc(1, sizeof(struct TreeNode));
        objectProps->children[i]->typeSymbol = 0xcd75dc4f6b9f15d2; // crc64 of "PropertySet"
        PropRead(stream, objectProps->children[i], flags);
        objectProps->children[i]->parent = objectProps;
    }

    return 0;
}
int DlgObjectPropsOwnerRead(FILE *stream, struct TreeNode *objectProps, uint32_t flags)
{
    objectProps->childCount = 1;
    objectProps->children = malloc(objectProps->childCount * sizeof(struct TreeNode *));
    objectProps->children[0] = calloc(1, sizeof(struct TreeNode));
    objectProps->children[0]->typeSymbol = 0xce1b42c4736deb3a; // crc64 of "DlgObjectProps"
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);                // skip block
    objectProps->children[0]->isBlocked = 1;
    DlgObjectPropsRead(stream, objectProps->children[0], flags);
    objectProps->children[0]->parent = objectProps;
    return 0;
}

int DlgObjectIDRead(FILE *stream, struct TreeNode *id, uint32_t flags)
{
    id->dataSize = sizeof(uint64_t);
    fread(id->data.staticBuffer, id->dataSize, 1, stream);
    return 0;
}

int DlgObjectIDOwnerRead(FILE *stream, struct TreeNode *IDOwner, uint32_t flags)
{
    IDOwner->childCount = 1;
    IDOwner->children = malloc(IDOwner->childCount * sizeof(struct TreeNode *));
    IDOwner->children[0] = calloc(1, sizeof(struct TreeNode));
    IDOwner->children[0]->typeSymbol = 0x87e0e880d7d2dfc1; // crc64 of "DlgObjID"
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);            // skip block
    IDOwner->children[0]->isBlocked = 1;
    DlgObjectIDRead(stream, IDOwner->children[0], flags);
    IDOwner->children[0]->parent = IDOwner;
    return 0;
}

int DlgNodeLinkRead(FILE *stream, struct TreeNode *link, uint32_t flags)
{
    link->childCount = 1;
    link->children = malloc(link->childCount * sizeof(struct TreeNode *));
    link->children[0] = calloc(1, sizeof(struct TreeNode));
    link->children[0]->typeSymbol = 0x8d53d2745d07232b; // crc64 of "DlgObjIDOwner"
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // skip block
    link->children[0]->isBlocked = 1;
    DlgObjectIDOwnerRead(stream, link->children[0], flags);
    link->children[0]->parent = link;

    link->dataSize = sizeof(uint32_t); // requiredChainContextType
    fread(link->data.staticBuffer, link->dataSize, 1, stream);
    return 0;
}

int DlgNodeRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    // printf("DlgNodeRead\n");

    node->childCount = 5;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->typeSymbol = 0x8d53d2745d07232b; // crc64 of "DlgObjIDOwner"
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // Skip Block
    node->children[0]->isBlocked = 1;
    DlgObjectIDOwnerRead(stream, node->children[0], flags);
    node->children[0]->parent = node;

    node->children[1] = calloc(1, sizeof(struct TreeNode));
    node->children[1]->typeSymbol = 0xd7ff03fa339f3f62; // crc64 of "DlgVisibilityConditionsOwner"
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // Skip Block
    node->children[1]->isBlocked = 1;
    DlgVisibilityConditionsOwnerRead(stream, node->children[1], flags);
    node->children[1]->parent = node;

    node->children[2] = calloc(1, sizeof(struct TreeNode));
    node->children[2]->typeSymbol = 0x96f88556b81a6af; // crc64 of "DlgObjectPropsOwner"
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);        // skip block
    node->children[2]->isBlocked = 1;
    DlgObjectPropsOwnerRead(stream, node->children[2], flags);
    node->children[2]->parent = node;

    node->children[3] = calloc(1, sizeof(struct TreeNode));
    node->children[3]->typeSymbol = 0x1fc8525b23714149; // crc64 of "DlgNodeLink"
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // skip block
    node->children[3]->isBlocked = 1;
    DlgNodeLinkRead(stream, node->children[3], flags);
    node->children[3]->parent = node;

    node->children[4] = calloc(1, sizeof(struct TreeNode));
    node->children[4]->typeSymbol = 0x1fc8525b23714149; // crc64 of "DlgNodeLink"
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // skip block
    node->children[4]->isBlocked = 1;
    DlgNodeLinkRead(stream, node->children[4], flags);
    node->children[4]->parent = node;

    node->dataSize = sizeof(uint64_t) + 2 * sizeof(uint32_t); // nameSymbol, flags and chainContextTypeID
    node->data.dynamicBuffer = malloc(node->dataSize);
    fread(node->data.dynamicBuffer, node->dataSize, 1, stream);
    return 0;
}

int DlgNodeTextRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    // printf("DlgNodeTextRead\n");

    node->childCount = 2;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->typeSymbol = 0x8940087148bf4c61; // crc64 of "DlgNode"
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // skip block
    node->children[0]->isBlocked = 1;
    DlgNodeRead(stream, node->children[0], flags);
    node->children[0]->parent = node;

    node->children[1] = calloc(1, sizeof(struct TreeNode));
    node->children[1]->typeSymbol = 0xd5c86012bf6b91ca; // crc64 of "LanguageResProxy"
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // skip block
    node->children[1]->isBlocked = 1;
    intrinsic4Read(stream, node->children[1], flags);
    node->children[1]->parent = node;

    return 0;
}

int DlgNodeLogicRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    // printf("DlgNodeLogic\n");

    node->childCount = 2;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->typeSymbol = 0x8940087148bf4c61; // crc64 of "DlgNode"
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // skip block
    node->children[0]->isBlocked = 1;
    DlgNodeRead(stream, node->children[0], flags);
    node->children[0]->parent = node;

    node->children[1] = calloc(1, sizeof(struct TreeNode));
    node->children[1]->typeSymbol = 0x5549d9efe67060e1; // crc64 of "Rule"
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // skip block
    node->children[1]->isBlocked = 1;
    RuleRead(stream, node->children[1], flags);
    node->children[1]->parent = node;

    return 0;
}

int DlgNodeScriptRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    // printf("DlgNodeScriptRead\n");

    node->childCount = 2;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->typeSymbol = 0x8940087148bf4c61; // crc64 of "DlgNode"
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // skip block
    node->children[0]->isBlocked = 1;
    DlgNodeRead(stream, node->children[0], flags);
    node->children[0]->parent = node;

    node->children[1] = calloc(1, sizeof(struct TreeNode));
    node->children[1]->typeSymbol = 0xcd9c6e605f5af4b4; // crc64 of "String"
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // skip block
    node->children[1]->isBlocked = 1;
    StringRead(stream, node->children[1], flags);
    node->children[1]->parent = node;

    node->dataSize = 2 * sizeof(uint8_t);
    fread(node->data.staticBuffer, node->dataSize, 1, stream);

    return 0;
}

int DlgNodeNotesRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    // printf("DlgNodeNotesRead\n");

    node->childCount = 2;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->typeSymbol = 0x8940087148bf4c61; // crc64 of "DlgNode"
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // skip block
    node->children[0]->isBlocked = 1;
    DlgNodeRead(stream, node->children[0], flags);
    node->children[0]->parent = node;

    node->children[1] = calloc(1, sizeof(struct TreeNode));
    node->children[1]->typeSymbol = 0xcd9c6e605f5af4b4; // crc64 of "String"
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // skip block
    node->children[1]->isBlocked = 1;
    StringRead(stream, node->children[1], flags);
    node->children[1]->parent = node;

    return 0;
}

int DlgNodeStartRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    // printf("DlgNodeStartRead\n");

    node->childCount = 2;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->typeSymbol = 0x8940087148bf4c61; // crc64 of "DlgNode"
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // skip block
    node->children[0]->isBlocked = 1;
    DlgNodeRead(stream, node->children[0], flags);
    node->children[0]->parent = node;

    node->children[1] = calloc(1, sizeof(struct TreeNode));
    node->children[1]->typeSymbol = 0xcd75dc4f6b9f15d2; // crc64 of "PropertySet"
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // skip block
    node->children[1]->isBlocked = 1;
    PropRead(stream, node->children[1], flags);
    node->children[1]->parent = node;

    return 0;
}

int DlgNodeJumpRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    // printf("DlgNodeJumpRead\n");

    node->childCount = 2;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->typeSymbol = 0x8940087148bf4c61; // crc64 of "DlgNode"
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // skip block
    node->children[0]->isBlocked = 1;
    DlgNodeRead(stream, node->children[0], flags);
    node->children[0]->parent = node;

    node->children[1] = calloc(1, sizeof(struct TreeNode));
    node->children[1]->typeSymbol = 0x1fc8525b23714149; // crc64 of "DlgNodeLink"
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // skip block
    node->children[1]->isBlocked = 1;
    DlgNodeLinkRead(stream, node->children[1], flags);
    node->children[1]->parent = node;

    node->dataSize = 36;
    node->data.dynamicBuffer = malloc(node->dataSize);
    fread(node->data.dynamicBuffer, node->dataSize, 1, stream);

    return 0;
}

int DlgChainHeadRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{

    node->childCount = 2;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->typeSymbol = 0x8d53d2745d07232b; // crc64 of "DlgObjIDOwner"
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // skip block
    node->children[0]->isBlocked = 1;
    DlgObjectIDOwnerRead(stream, node->children[0], flags);
    node->children[0]->parent = node;

    node->children[1] = calloc(1, sizeof(struct TreeNode));
    node->children[1]->typeSymbol = 0x1fc8525b23714149; // crc64 of "DlgNodeLink"
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // skip block
    node->children[1]->isBlocked = 1;
    DlgNodeLinkRead(stream, node->children[1], flags);
    node->children[1]->parent = node;

    return 0;
}

int DlgChildRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    // The amount of overhead is getting insane.
    node->childCount = 5;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint32_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    node->children[0]->typeSymbol = 0xe26bbba29eba3e2a; // crc64 of "DlgChainHead"
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // skip block
    node->children[0]->isBlocked = 1;
    DlgChainHeadRead(stream, node->children[0], flags);

    node->children[1]->typeSymbol = 0x4f023463d89fb0; // crc64 of "Symbol"
    intrinsic8Read(stream, node->children[1], flags);

    node->children[2]->typeSymbol = 0xd7ff03fa339f3f62; // crc64 of "DlgVisibilityConditionsOwner"
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // skip block
    node->children[2]->isBlocked = 1;
    DlgVisibilityConditionsOwnerRead(stream, node->children[2], flags);

    node->children[3]->typeSymbol = 0x96f88556b81a6af; // crc64 of "DlgObjectPropsOwner"
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);        // skip block
    node->children[3]->isBlocked = 1;
    DlgObjectPropsOwnerRead(stream, node->children[3], flags);

    node->children[4]->typeSymbol = 0x1fc8525b23714149; // crc64 of "DlgNodeLink"
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // skip block
    node->children[4]->isBlocked = 1;
    DlgNodeLinkRead(stream, node->children[4], flags);

    // printf("child = %d\n", node->childCount);

    return 0;
}

int DlgNodeIdleRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    printf("DlgNodeIdleRead\n");

    node->childCount = 1;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->typeSymbol = 0x8940087148bf4c61; // crc64 of "DlgNode"
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // skip block
    node->children[0]->isBlocked = 1;
    DlgNodeRead(stream, node->children[0], flags);
    node->children[0]->parent = node;

    node->dataSize = sizeof(uint32_t) * 5 + sizeof(float) + sizeof(uint64_t);
    node->data.dynamicBuffer = malloc(node->dataSize);
    fread(node->data.dynamicBuffer, node->dataSize, 1, stream);

    return 0;
}

int DlgNodeMarkerRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    printf("DlgNodeMarkerRead\n");

    node->childCount = 1;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->typeSymbol = 0x8940087148bf4c61; // crc64 of "DlgNode"
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // skip block
    node->children[0]->isBlocked = 1;
    DlgNodeRead(stream, node->children[0], flags);
    node->children[0]->parent = node;

    return 0;
}

int DlgNodeExitRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    DlgNodeMarkerRead(stream, node, flags);
}

int DlgLineRead(FILE *stream, struct TreeNode *line, uint32_t flags)
{
    line->childCount = 3;
    line->children = malloc(line->childCount * sizeof(struct TreeNode *));

    for (uint32_t i = 0; i < line->childCount; ++i)
    {
        line->children[i] = calloc(1, sizeof(struct TreeNode));
        line->children[i]->parent = line;
        line->children[i]->isBlocked = 1;
    }

    line->children[0]->typeSymbol = 0x4bd04e27e6716ea0; // crc64 of "UID::Generator"
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // skip block
    intrinsic4Read(stream, line->children[0], flags);

    line->children[1]->typeSymbol = 0x8d53d2745d07232b; // crc64 of "DlgObjIDOwner"
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // skip block
    DlgObjectIDOwnerRead(stream, line->children[1], flags);

    line->children[2]->typeSymbol = 0xd5c86012bf6b91ca; // crc64 of "LanguageResProxy"
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // skip block
    intrinsic4Read(stream, line->children[2], flags);

    return 0;
}

int DlgLineCollectionRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    printf("DlgLineCollectionRead\n");

    node->childCount = 2;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint32_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
        node->children[i]->isBlocked = 1;
    }

    node->children[0]->typeSymbol = 0x4bd04e27e6716ea0; // crc64 of "UID::Generator"
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // skip block
    intrinsic4Read(stream, node->children[0], flags);

    node->children[1]->typeSymbol = 0xaba164d5aab73489; // crc64 of "Map<int,DlgLine,less<int>>"
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // skip block
    genericMapRead(stream, node->children[1], flags, intrinsic4Read, DlgLineRead, 0x99d7c52ea7f0f97d, 0xa7fe20e35345ac33);

    return 0;
}

int DlgNodeExchangeRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    printf("DlgNodeExchange\n");

    node->childCount = 4;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->parent = node;
    node->children[0]->typeSymbol = 0xbae4cbd77f139a91; // crc64 of "float"
    intrinsic4Read(stream, node->children[0], flags);

    node->children[1] = calloc(1, sizeof(struct TreeNode));
    node->children[1]->parent = node;
    node->children[1]->typeSymbol = 0x6643af6dcccc81a9; // crc64 of "Handle<Chore>"
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // Skip block
    node->children[1]->isBlocked = 1;
    intrinsic8Read(stream, node->children[1], flags);

    node->children[2] = calloc(1, sizeof(struct TreeNode));
    node->children[2]->parent = node;
    node->children[2]->typeSymbol = 0x8940087148bf4c61; // crc64 of "DlgNode"
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // Skip block
    node->children[2]->isBlocked = 1;
    DlgNodeRead(stream, node->children[2], flags);

    uint32_t nodeFlags = *((uint32_t *)(node->children[2]->data.dynamicBuffer) + 2);
    node->childCount += ((nodeFlags & 1) > 0) + ((nodeFlags & 2) > 0);
    node->children = realloc(node->children, node->childCount * sizeof(struct TreeNode *));

    node->children[3] = calloc(1, sizeof(struct TreeNode));
    node->children[3]->parent = node;
    node->children[3]->typeSymbol = 0x11f7e31fb297d7d; // crc64 of "DCArray<DlgNodeExchange::Entry>"
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);        // Skip block
    node->children[3]->isBlocked = 1;

    node->children[3]->childCount = 1;
    node->children[3]->children = malloc(node->children[3]->childCount * sizeof(struct TreeNode *));
    node->children[3]->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[3]->children[0]->typeSymbol = 0x99d7c52ea7f0f97d; // crc64 of "int"
    intrinsic4Read(stream, node->children[3]->children[0], flags);
    node->children[3]->children[0]->parent = node->children[3];

    node->children[3]->childCount += *(uint32_t *)(node->children[3]->children[0]->data.staticBuffer);
    node->children[3]->children = realloc(node->children[3]->children, node->children[3]->childCount * sizeof(struct TreeNode *));

    for (uint32_t i = 1; i < node->children[3]->childCount; ++i)
    {
        node->children[3]->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[3]->children[i]->typeSymbol = 0xe008b7efba7579d4; // crc64 of "DlgNodeExchange::Entry"
        node->children[3]->children[i]->parent = node->children[3];
        intrinsic8Read(stream, node->children[3]->children[i], flags);
    }

    uint32_t index = 4;

    if (nodeFlags & 0x1)
    {
        printf("Error: NoteCollection Read not implemented");
        ++index;
        return -1;
    }

    if (nodeFlags & 0x2)
    {
        node->children[index] = calloc(1, sizeof(struct TreeNode));
        node->children[index]->parent = node;
        node->children[index]->typeSymbol = 0x1c9a459992d55f3; // crc64 of "DlgLineCollection"
        DlgLineCollectionRead(stream, node->children[index], flags);
    }

    return 0;
}

int DlgConditionRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 1;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->isBlocked = 1;
    node->children[0]->typeSymbol = 0x8d53d2745d07232b; // crc64 of "DlgObjIDOwner"
    node->children[0]->parent = node;
    DlgObjectIDOwnerRead(stream, node->children[0], flags);

    return 0;
}

int DlgConditionRuleRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 2;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->isBlocked = 1;
    node->children[0]->typeSymbol = 0x26591af0c98ad8f6; // crc64 of "DlgCondition"
    node->children[0]->parent = node;
    DlgConditionRead(stream, node->children[0], flags);

    node->children[1] = calloc(1, sizeof(struct TreeNode));
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[1]->isBlocked = 1;
    node->children[1]->typeSymbol = 0x5549d9efe67060e1; // crc64 of "rule"
    node->children[1]->parent = node;
    RuleRead(stream, node->children[1], flags);

    return 0;
}

int DlgConditionTimeRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 1;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->isBlocked = 1;
    node->children[0]->typeSymbol = 0x26591af0c98ad8f6; // crc64 of "DlgCondition"
    node->children[0]->parent = node;
    DlgConditionRead(stream, node->children[0], flags);

    node->dataSize = 2 * sizeof(uint32_t);
    fread(node->data.staticBuffer, node->dataSize, 1, stream); // TODO: Maybe change these to children

    return 0;
}

int DlgConditionInputRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 1;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->isBlocked = 1;
    node->children[0]->typeSymbol = 0x26591af0c98ad8f6; // crc64 of "DlgCondition"
    node->children[0]->parent = node;
    DlgConditionRead(stream, node->children[0], flags);

    return 0;
}

int DlgConditionSetRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 1;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->typeSymbol = 0xe6130df4edcec9c8; // TODO: Set appropriate type
    node->children[0]->parent = node;

    node->children[0]->childCount = 1;
    node->children[0]->children = malloc(node->children[0]->childCount * sizeof(struct TreeNode *));
    node->children[0]->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->children[0]->typeSymbol = 0x99d7c52ea7f0f97d; // crc64 of "int"
    intrinsic4Read(stream, node->children[0]->children[0], flags);
    node->children[0]->children[0]->parent = node->children[0];

    node->children[0]->childCount += *(uint32_t *)(node->children[0]->children[0]->data.staticBuffer);
    node->children[0]->children = realloc(node->children[0]->children, node->children[0]->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 1; i < node->children[0]->childCount; ++i)
    {
        printf("ftell = %lx\n", cftell(stream));
        node->children[0]->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[0]->children[i]->serializeType = 1;
        fread(&(node->children[0]->children[i]->typeSymbol), sizeof(node->children[0]->children[i]->typeSymbol), 1, stream);
        node->children[0]->children[i]->parent = node->children[0];
        readMetaClass(stream, node->children[0]->children[i], flags);
    }

    return 0;
}

int DlgNodeWaitRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    printf("DlgNodeWaitRead\n");

    node->childCount = 2;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    node->children[0]->isBlocked = 1;
    node->children[0]->parent = node;
    node->children[0]->typeSymbol = 0x8940087148bf4c61; // crc64 of "DlgNode"
    DlgNodeRead(stream, node->children[0], flags);

    node->children[1] = calloc(1, sizeof(struct TreeNode));
    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    node->children[1]->isBlocked = 1;
    node->children[1]->parent = node;
    node->children[1]->typeSymbol = 0xe6130df4edcec9c8; // crc64 of "DlgConditionSet"
    DlgConditionSetRead(stream, node->children[1], flags);

    return 0;
}

int DlgChildSetRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 1;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->typeSymbol = 0x99d7c52ea7f0f97d; // crc64 of "int"
    intrinsic4Read(stream, node->children[0], flags);
    node->children[0]->parent = node;

    node->childCount += *(uint32_t *)(node->children[0]->data.staticBuffer);
    // printf("ftell = %lx\n", cftell(stream));
    node->children = realloc(node->children, node->childCount * sizeof(struct TreeNode *));
    // printf("childCount = %d\n", node->childCount);

    for (uint16_t i = 1; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->serializeType = 1;
        fread(&(node->children[i]->typeSymbol), sizeof(node->children[i]->typeSymbol), 1, stream);
        node->children[i]->parent = node;
        readMetaClass(stream, node->children[i], flags);
    }

    return 0;
}

int DlgChildSetChoiceRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 1;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->parent = node;
    node->children[0]->isBlocked = 1;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // Skip block
    node->children[0]->typeSymbol = 0x915709e72a07b3ed; // crc64 of "DlgChildSet"
    DlgChildSetRead(stream, node->children[0], flags);

    return 0;
}

int DlgChildSetChoicesChildPre(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DlgChildSetChoiceRead(stream, node, flags);
}

int DlgChildSetChoicesChildPost(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DlgChildSetChoiceRead(stream, node, flags);
}

int DlgNodeChoicesRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    printf("DlgNodeChoice\n");

    node->childCount = 4;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
        node->children[i]->isBlocked = 1;
    }

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // Skip block
    node->children[0]->typeSymbol = 0x8940087148bf4c61; // crc64 of "DlgNode"
    DlgNodeRead(stream, node->children[0], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);        // Skip block
    node->children[1]->typeSymbol = 0xd4bf6e9059b7abd; // crc64 of "DlgChildSetChoice"
    DlgChildSetChoiceRead(stream, node->children[1], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // Skip block
    node->children[2]->typeSymbol = 0x30732a2c440a6c8a; // crc64 of "DlgChildSetChoicesChildPre"
    DlgChildSetChoicesChildPre(stream, node->children[2], flags);

    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // Skip block
    node->children[3]->typeSymbol = 0x681af06eda9af05d; // crc64 of "DlgChildSetChoicesChildPost"
    DlgChildSetChoicesChildPost(stream, node->children[3], flags);

    return 0;
}

int DlgFolderChildRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 1;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->parent = node;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->isBlocked = 1;
    node->children[0]->typeSymbol = 0xbfb0ce2bd1f38792; // crc64 of "DlgChild"
    DlgChildRead(stream, node->children[0], flags);

    return 0;
}

int DlgChoicesChildPreRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DlgFolderChildRead(stream, node, flags);
}

int DlgChoicesChildPostRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DlgFolderChildRead(stream, node, flags);
}

int DlgChoiceRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    printf("DlgChoiceRead\n");

    node->childCount = 2;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->parent = node;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[0]->isBlocked = 1;
    node->children[0]->typeSymbol = 0xbfb0ce2bd1f38792; // crc64 of "DlgChild"
    DlgChildRead(stream, node->children[0], flags);

    node->children[1] = calloc(1, sizeof(struct TreeNode));
    node->children[1]->parent = node;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);
    node->children[1]->isBlocked = 1;
    node->children[1]->typeSymbol = 0xe6130df4edcec9c8; // crc64 of "DlgConditionSet"
    DlgConditionSetRead(stream, node->children[1], flags);

    return 0;
}

int DlgConditionalCaseRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DlgFolderChildRead(stream, node, flags);
}

int DlgNodeCriteriaRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 6;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    node->children[0]->isBlocked = 1;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // Skip block
    node->children[0]->typeSymbol = 0x365952b07c0ce60d; // crc64 of "DlgNodeCriteria::EnumTestT"
    intrinsic4Read(stream, node->children[0], flags);

    node->children[1]->isBlocked = 1;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // Skip block
    node->children[1]->typeSymbol = 0xd513aeaf2180fc7e; // crc64 of "DlgNodeCriteria::EnumThresholdT"
    intrinsic4Read(stream, node->children[1], flags);

    node->children[2]->isBlocked = 1;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // Skip block
    node->children[2]->typeSymbol = 0xd513aeaf2180fc7e; // crc64 of "DlgNodeCriteria::EnumThresholdT"
    intrinsic4Read(stream, node->children[2], flags);

    node->children[3]->isBlocked = 1;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // Skip block
    node->children[3]->typeSymbol = 0xb32b46b47690925e; // crc64 of "DlgNodeCriteria::EnumDefaultResultT"
    intrinsic4Read(stream, node->children[3], flags);

    node->children[4]->typeSymbol = 0x84283cb979d71641; // crc64 of "Flags"
    intrinsic4Read(stream, node->children[4], flags);

    node->children[5]->isBlocked = 1;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // Skip block
    node->children[5]->typeSymbol = 0x84283cb979d71641; // TODO: Replace with Set
    intrinsic4Read(stream, node->children[5], flags);

    return 0;
}

int DlgChildSetElementRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 1;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->isBlocked = 1;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // Skip block
    node->children[0]->typeSymbol = 0x915709e72a07b3ed; // crc64 of "DlgChildSet"
    DlgChildSetRead(stream, node->children[0], flags);
    node->children[0]->parent = node;

    return 0;
}

int DlgNodeSequenceElementRead(FILE *stream, struct TreeNode *node, uint32_t flags)
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
    node->children[0]->typeSymbol = 0xbfb0ce2bd1f38792; // crc64 of "DlgChild"
    DlgChildRead(stream, node->children[0], flags);

    node->children[1]->typeSymbol = 0x101e4bf52a9999ac; // crc64 of "long"
    intrinsic4Read(stream, node->children[1], flags);

    node->children[2]->typeSymbol = 0x101e4bf52a9999ac; // crc64 of "long"
    intrinsic4Read(stream, node->children[2], flags);

    return 0;
}

int DlgNodeSequenceRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    printf("DlgNodeSequenceRead\n");

    node->childCount = 5;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    for (uint16_t i = 0; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
    }

    node->children[0]->isBlocked = 1;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // Skip block
    node->children[0]->typeSymbol = 0x8940087148bf4c61; // crc64 of "DlgNode"
    DlgNodeRead(stream, node->children[0], flags);

    node->children[1]->isBlocked = 1;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // Skip block
    node->children[1]->typeSymbol = 0x9fab8543532be4a7; // crc64 of "DlgNodeSequence::DlgChildSetElement"
    DlgChildSetElementRead(stream, node->children[1], flags);

    node->children[2]->typeSymbol = 0x101e4bf52a9999ac; // crc64 of "long"
    intrinsic4Read(stream, node->children[2], flags);

    node->children[3]->typeSymbol = 0x101e4bf52a9999ac; // crc64 of "long"
    intrinsic4Read(stream, node->children[3], flags);

    node->children[4]->isBlocked = 1;
    cfseek(stream, sizeof(uint32_t), SEEK_CUR);         // Skip block
    node->children[4]->typeSymbol = 0x21436b2755503c3b; // crc64 of "DlgNodeCriteria"
    DlgNodeCriteriaRead(stream, node->children[4], flags);

    return 0;
}

int DlgChildSetConditionalCaseRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return DlgChildSetElementRead(stream, node, flags);
}

int DlgNodeConditionalRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->childCount = 2;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));

    node->children[0] = calloc(1, sizeof(struct TreeNode));
    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    node->children[0]->isBlocked = 1;
    node->children[0]->parent = node;
    node->children[0]->typeSymbol = 0x8940087148bf4c61; // crc64 of "DlgNode"
    DlgNodeRead(stream, node->children[0], flags);

    node->children[1] = calloc(1, sizeof(struct TreeNode));
    cfseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    node->children[1]->isBlocked = 1;
    node->children[1]->parent = node;
    node->children[1]->typeSymbol = 0x87b6b95067db12f9; // crc64 of "DlgChildSetConditionalCase"
    DlgChildSetConditionalCaseRead(stream, node->children[1], flags);

    return 0;
}
