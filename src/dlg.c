#include <types.h>
#include <tree.h>
#include <stdlib.h>
#include <meta.h>
#include <assert.h>
#include <dlg.h>
#include <prop.h>
#include <ttbool.h>

struct DependencyLoader
{
    uint8_t hasData;
    uint64_t typeSymbol;
    void *symbolTypeData;
};

struct DlgDownstreamVisibilityConditions
{
    uint32_t flags;
    int32_t maxNumNodeEvals; // Normally -999
};

struct DlgVisibilityConditions
{
    uint8_t diesOff;
    uint32_t flags;
    uint32_t downStreamConditionsBlock; // Always equal to 0x0C
    struct DlgDownstreamVisibilityConditions downStreamConditions;
    uint32_t scriptBlock;
    struct String scriptVisCond; // lua code
    void *rule;                  // if flags & 1
};

struct DlgVisibilityConditionsOwner
{
    uint32_t conditionsBlock;
    struct DlgVisibilityConditions conditions;
};

static size_t blockRead(FILE *stream, uint8_t *buffer)
{
    size_t bytesRead = 0;
    bytesRead += fread(buffer, sizeof(uint32_t), 1, stream);
    buffer += bytesRead;
    bytesRead += fread(buffer, (*(uint32_t *)(buffer - sizeof(uint32_t)) - 4), 1, stream);
    return bytesRead;
}

static int DlgFolderRead(FILE *stream, struct DlgFolder *folder, uint32_t flags)
{
    printf("FolderRead\n");
    fread(&folder->IDblock, sizeof(folder->IDblock), 1, stream);
    fread(&folder->IDowner, folder->IDblock, 1, stream);
    fread(&folder->objectPropsBlock, sizeof(folder->objectPropsBlock), 1, stream);
    fread(&folder->propsOwner, folder->objectPropsBlock, 1, stream);
    fread(&folder->childCount, sizeof(folder->childCount), 1, stream);
    folder->childSet = malloc(sizeof(struct TreeNode *) * folder->childCount);
    for (int i = 0; i < folder->childCount; ++i)
    {
        folder->childSet[i] = calloc(1, sizeof(struct TreeNode));
        fread(&folder->childSet[i]->typeSymbol, sizeof(folder->childSet[i]->typeSymbol), 1, stream);
        readMetaClass(stream, (void **)(folder->childSet + i), folder->childSet[i]->typeSymbol, flags);
    }
    fread(&folder->nameSymbol, sizeof(folder->nameSymbol), 1, stream);
    fread(&folder->prodReportPropBlock, sizeof(folder->prodReportPropBlock), 1, stream);
    folder->prodReportProp = malloc(folder->prodReportPropBlock - sizeof(folder->prodReportPropBlock));
    fread(folder->prodReportProp, folder->prodReportPropBlock - sizeof(folder->prodReportPropBlock), 1, stream);
}

void DlgRead(FILE *stream, struct Dlg *dlg, uint32_t flags)
{
    fread(&dlg->idOwnerBlock, sizeof(dlg->idOwnerBlock), 1, stream);
    fread(&dlg->idOwner, dlg->idOwnerBlock - sizeof(dlg->idOwnerBlock), 1, stream);
    fread(&dlg->stringBlock, sizeof(dlg->stringBlock), 1, stream);
    readString(stream, (void **)(&dlg->name), 1);
    fread(&dlg->version, sizeof(dlg->version), 1, stream);
    fread(&dlg->idBlock, sizeof(dlg->idBlock), 1, stream);
    fread(&dlg->id, dlg->idBlock - sizeof(dlg->idOwnerBlock), 1, stream);
    fread(&dlg->langBlock, sizeof(dlg->langBlock), 1, stream);
    dlg->db = malloc(dlg->langBlock - sizeof(dlg->langBlock));
    fread(dlg->db, dlg->langBlock - sizeof(dlg->langBlock), 1, stream);
    fread(&dlg->projectID, sizeof(dlg->projectID), 1, stream);
    fread(&dlg->resourceLocationID, sizeof(dlg->resourceLocationID), 1, stream);
    fread(&dlg->chronolgy, sizeof(dlg->chronolgy), 1, stream);
    fread(&dlg->flags, sizeof(dlg->flags), 1, stream);
    fread(&dlg->dependencyBlock, sizeof(dlg->dependencyBlock), 1, stream);
    dlg->dependencyLoader = malloc(dlg->dependencyBlock - sizeof(dlg->dependencyBlock));
    fread(dlg->dependencyLoader, dlg->dependencyBlock - sizeof(dlg->dependencyBlock), 1, stream);
    fread(&dlg->propBlock, sizeof(dlg->propBlock), 1, stream);
    dlg->prodReportProps = malloc(dlg->propBlock - sizeof(dlg->propBlock));
    fread(dlg->prodReportProps, dlg->propBlock - sizeof(dlg->propBlock), 1, stream);
    fread(&dlg->jiraBlock, sizeof(dlg->jiraBlock), 1, stream);
    fread(&dlg->hasToolOnlyData, sizeof(dlg->hasToolOnlyData), 1, stream);
    fread(&dlg->folderCount, sizeof(dlg->folderCount), 1, stream);

    dlg->folders = malloc(sizeof(struct DlgFolder) * dlg->folderCount);

    for (uint32_t i = 0; i < dlg->folderCount; ++i)
    {
        DlgFolderRead(stream, dlg->folders + i, flags);
    }

    fread(&dlg->nodeCount, sizeof(dlg->nodeCount), 1, stream);

    dlg->nodes = malloc(sizeof(struct TreeNode *) * dlg->nodeCount);

    for (uint32_t i = 0; i < dlg->nodeCount; ++i)
    {
        dlg->nodes[i] = calloc(1, sizeof(struct TreeNode));
        fread(&dlg->nodes[i]->typeSymbol, sizeof(dlg->nodes[i]->typeSymbol), 1, stream);
        readMetaClass(stream, (void **)(dlg->nodes + i), dlg->nodes[i]->typeSymbol, flags);
    }

    printf("nodes read\n");

    for (uint32_t i = 0; i < dlg->folderCount; ++i)
    {
        for (uint32_t j = 0; j < dlg->folders[i].childCount; ++j)
        {
            struct TreeNode *node = dlg->folders[i].childSet[j];
            printf("child assemble\n");
            while (node != NULL && node != (struct TreeNode *)0x988d0903f713877b)
            {
                for (uint32_t k = 0; k < dlg->nodeCount; ++k)
                {
                    // printf("debug\n");
                    if (dlg->nodes[k]->idSymbol == (uint64_t)(node->next))
                    {
                        printf("break\n");
                        node->next = dlg->nodes[k];
                        dlg->nodes[k]->parent = node;
                        break;
                    }
                }
                printf("id = %lx\n", (uint64_t)node->idSymbol);
                node = node->next;
                printf("next = %lx\n", (uint64_t)node);
            }
        }
    }

    if (flags & 4)
    {
    }
}

// To use this read the symbol and DO NOT READ THE BLOCK
int DlgNodeLogicRead(FILE *stream, void **treeNode, uint32_t flags)
{
    printf("DlgNodeLogic\n");
    long initialPosition = ftell(stream);
    struct TreeNode *node = *treeNode;
    node->childCount = 0;
    node->children = NULL;

    uint32_t nodeSize = 0;
    uint32_t blockBuffer;
    fread(&blockBuffer, sizeof(blockBuffer), 1, stream);
    nodeSize += blockBuffer;

    fseek(stream, nodeSize - sizeof(uint32_t), SEEK_CUR);
    fread(&blockBuffer, sizeof(blockBuffer), 1, stream);
    nodeSize += blockBuffer;
    fseek(stream, initialPosition, SEEK_SET);

    node->data = malloc(nodeSize);
    fread(node->data, nodeSize, 1, stream);

    uint8_t *buffer = node->data;
    node->next = (struct TreeNode *)(*((uint64_t *)(buffer + nodeSize - blockBuffer - 28)));
    node->parent = (struct TreeNode *)(*((uint64_t *)(buffer + nodeSize - blockBuffer - 52)));
    node->idSymbol = *((uint64_t *)(buffer + 12));

    return 0;
}

int DlgLineCollectionRead(FILE *stream, void **treeNode, uint32_t flags)
{
    printf("DlgLineCollectionRead\n");
    struct TreeNode *node = *treeNode;

    uint32_t blockBuffer;
    fseek(stream, 2 * sizeof(uint32_t), SEEK_CUR);
    fread(&blockBuffer, sizeof(blockBuffer), 1, stream);
    fseek(stream, -3 * sizeof(uint32_t), SEEK_CUR);

    node->data = malloc(blockBuffer + 2 * sizeof(uint32_t));
    fread(node->data, blockBuffer + 2 * sizeof(uint32_t), 1, stream);
}

int NoteCollectionRead(FILE *stream, void **treeNode, uint32_t flags)
{
    DlgLineCollectionRead(stream, treeNode, flags);
}

int DlgNodeExchangeRead(FILE *stream, void **treeNode, uint32_t flags)
{
    printf("DlgNodeExchange\n");
    struct TreeNode *node = *treeNode;
    node->childCount = 0;
    node->children = NULL;

    uint32_t nodeSize = 0;
    long initialPosition = ftell(stream);
    uint32_t blockBuffer;
    uint32_t nodeFlags;

    fseek(stream, sizeof(float) + sizeof(uint32_t) + sizeof(uint64_t), SEEK_CUR);
    fread(&blockBuffer, sizeof(blockBuffer), 1, stream);
    nodeSize += blockBuffer;
    fseek(stream, blockBuffer - sizeof(blockBuffer), SEEK_CUR);
    fread(&blockBuffer, sizeof(blockBuffer), 1, stream);
    nodeSize += blockBuffer + sizeof(float) + sizeof(uint32_t) + sizeof(uint64_t);

    fseek(stream, -3 * sizeof(uint32_t), SEEK_CUR);
    fread(&nodeFlags, sizeof(nodeFlags), 1, stream);

    fseek(stream, initialPosition, SEEK_SET);
    node->data = malloc(nodeSize);
    fread(node->data, nodeSize, 1, stream);

    uint8_t *buffer = node->data;
    node->idSymbol = *((uint64_t *)(buffer + 8 + 4 + sizeof(float) + sizeof(uint32_t) + sizeof(uint64_t)));
    node->next = (struct TreeNode *)(*((uint64_t *)(buffer + nodeSize - blockBuffer - 28)));
    node->parent = (struct TreeNode *)(*((uint64_t *)(buffer + nodeSize - blockBuffer - 52)));

    if (nodeFlags & 0x1)
    {
        struct TreeNode *noteCollection = calloc(1, sizeof(struct TreeNode));
        NoteCollectionRead(stream, (void **)(&noteCollection), flags);
        noteCollection->parent = node;
        node->children = malloc(sizeof(struct TreeNode *));
        *(node->children) = noteCollection;
        node->childCount = 1;
    }

    if (nodeFlags & 0x2)
    {
        struct TreeNode *dlgLineCollection = calloc(1, sizeof(struct TreeNode));
        DlgLineCollectionRead(stream, (void **)(&dlgLineCollection), flags);
        dlgLineCollection->parent = node;
        node->children = malloc(sizeof(struct TreeNode *));
        *(node->children) = dlgLineCollection;
        node->childCount = 1;
    }

    printf(" %lx\n", node->idSymbol);

    return 0;
}

int DlgNodeChoicesRead(FILE *stream, void **treeNode, uint32_t flags)
{
    printf("DlgNodeChoice\n");
    struct TreeNode *node = *treeNode;
    node->childCount = 0;

    long initialPosition = ftell(stream);
    uint32_t blockBuffer;

    fread(&blockBuffer, sizeof(uint32_t), 1, stream);
    node->data = malloc(blockBuffer);

    fseek(stream, initialPosition, SEEK_SET);
    fread(node->data, blockBuffer, 1, stream);

    uint8_t *buffer = node->data;
    node->idSymbol = *((uint64_t *)(buffer + 12));
    node->next = (struct TreeNode *)(*((uint64_t *)(buffer + blockBuffer - 28)));
    node->parent = (struct TreeNode *)(*((uint64_t *)(buffer + blockBuffer - 52)));

    uint32_t childCountBuffer;
    uint32_t i = 0;

    // Choice child set
    fseek(stream, 8, SEEK_CUR); // Skip blocks
    fread(&childCountBuffer, sizeof(childCountBuffer), 1, stream);
    node->childCount += childCountBuffer;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));
    for (i; i < node->childCount; ++i)
    {
        struct TreeNode *child = calloc(1, sizeof(struct TreeNode));
        fread(&child->typeSymbol, sizeof(child->typeSymbol), 1, stream);
        readMetaClass(stream, (void **)(&child), child->typeSymbol, flags);
        child->parent = node;
        node->children[i] = child;
    }

    // Pre choice child set
    fseek(stream, 8, SEEK_CUR); // Skip blocks
    fread(&childCountBuffer, sizeof(childCountBuffer), 1, stream);
    node->childCount += childCountBuffer;
    node->children = realloc(node->children, node->childCount * sizeof(struct TreeNode *));
    for (i; i < node->childCount; ++i)
    {
        struct TreeNode *child = calloc(1, sizeof(struct TreeNode));
        fread(&child->typeSymbol, sizeof(child->typeSymbol), 1, stream);
        readMetaClass(stream, (void **)(&child), child->typeSymbol, flags);
        child->parent = node;
        node->children[i] = child;
    }

    // Post choice child set
    fseek(stream, 8, SEEK_CUR); // Skip blocks
    fread(&childCountBuffer, sizeof(childCountBuffer), 1, stream);
    node->childCount += childCountBuffer;
    node->children = realloc(node->children, node->childCount * sizeof(struct TreeNode *));
    for (i; i < node->childCount; ++i)
    {
        struct TreeNode *child = calloc(1, sizeof(struct TreeNode));
        fread(&child->typeSymbol, sizeof(child->typeSymbol), 1, stream);
        readMetaClass(stream, (void **)(&child), child->typeSymbol, flags);
        child->parent = node;
        node->children[i] = child;
    }
}

int DlgChoiceRead(FILE *stream, void **treeNode, uint32_t flags)
{
    printf("DlgChoiceRead\n");

    struct TreeNode *node = *treeNode;
    long initialPosition = ftell(stream);

    uint32_t blockBuffer;
    uint32_t nodeSize = 0;
    fread(&blockBuffer, sizeof(blockBuffer), 1, stream);
    nodeSize += blockBuffer;
    fseek(stream, blockBuffer - sizeof(blockBuffer), SEEK_CUR);
    fread(&blockBuffer, sizeof(blockBuffer), 1, stream);
    nodeSize += blockBuffer;
    node->data = malloc(nodeSize);
    fseek(stream, initialPosition, SEEK_SET);
    fread(node->data, nodeSize, 1, stream);

    uint8_t *buffer = node->data;
    node->idSymbol = *((uint64_t *)(buffer + 16));
}

int DlgNodeWaitRead(FILE *stream, void **treeNode, uint32_t flags)
{
    printf("DlgNodeWaitRead\n");

    struct TreeNode *node = *treeNode;
    long initialPosition = ftell(stream);

    uint32_t blockBuffer;
    uint32_t nodeSize = 0;
    fread(&blockBuffer, sizeof(blockBuffer), 1, stream);
    nodeSize += blockBuffer;
    fseek(stream, blockBuffer - sizeof(blockBuffer), SEEK_CUR);
    fread(&blockBuffer, sizeof(blockBuffer), 1, stream);
    nodeSize += blockBuffer;
    node->data = malloc(nodeSize);
    fseek(stream, initialPosition, SEEK_SET);
    fread(node->data, nodeSize, 1, stream);

    uint8_t *buffer = node->data;
    node->next = (struct TreeNode *)(*((uint64_t *)(buffer + nodeSize - blockBuffer - 28)));
    node->parent = (struct TreeNode *)(*((uint64_t *)(buffer + nodeSize - blockBuffer - 52)));
    node->idSymbol = *((uint64_t *)(buffer + 12));
}

int DlgNodeMarkerRead(FILE *stream, void **treeNode, uint32_t flags)
{
    printf("DlgNodeMarkerRead\n");

    struct TreeNode *node = *treeNode;

    uint32_t blockBuffer;
    fread(&blockBuffer, sizeof(blockBuffer), 1, stream);
    node->data = malloc(blockBuffer);
    fseek(stream, -sizeof(blockBuffer), SEEK_CUR);
    fread(node->data, blockBuffer, 1, stream);

    uint8_t *buffer = node->data;
    node->next = (struct TreeNode *)(*((uint64_t *)(buffer + blockBuffer - 28)));
    node->parent = (struct TreeNode *)(*((uint64_t *)(buffer + blockBuffer - 52)));
    node->idSymbol = *((uint64_t *)(buffer + 12));

    printf("%lx\n", node->idSymbol);
}

int DlgNodeIdleRead(FILE *stream, void **treeNode, uint32_t flags)
{
    printf("DlgNodeIdleRead\n");

    struct TreeNode *node = *treeNode;

    uint32_t blockBuffer;
    fread(&blockBuffer, sizeof(blockBuffer), 1, stream);
    node->data = malloc(blockBuffer + 32);
    fseek(stream, -sizeof(blockBuffer), SEEK_CUR);
    fread(node->data, blockBuffer + 32, 1, stream);

    uint8_t *buffer = node->data;
    node->next = (struct TreeNode *)(*((uint64_t *)(buffer + blockBuffer - 28)));
    node->parent = (struct TreeNode *)(*((uint64_t *)(buffer + blockBuffer - 52)));
    node->idSymbol = *((uint64_t *)(buffer + 12));
}

int DlgConditionalCaseRead(FILE *stream, void **treeNode, uint32_t flags)
{
    printf("DlgConditionalCaseRead\n");

    struct TreeNode *node = *treeNode;

    uint32_t blockBuffer;
    fread(&blockBuffer, sizeof(blockBuffer), 1, stream);
    node->data = malloc(blockBuffer);
    fseek(stream, -sizeof(blockBuffer), SEEK_CUR);
    fread(node->data, blockBuffer, 1, stream);

    uint8_t *buffer = node->data;
    node->next = (struct TreeNode *)(*((uint64_t *)(buffer + 36)));
    node->parent = (struct TreeNode *)(*((uint64_t *)(buffer + blockBuffer - 12)));
    node->idSymbol = *((uint64_t *)(buffer + 16));
}

int DlgNodeScriptRead(FILE *stream, void **treeNode, uint32_t flags)
{
    printf("DlgNodeScriptRead\n");

    struct TreeNode *node = *treeNode;
    long initialPosition = ftell(stream);

    uint32_t blockBuffer;
    uint32_t nodeSize = 0;
    fread(&blockBuffer, sizeof(blockBuffer), 1, stream);
    nodeSize += blockBuffer;
    fseek(stream, blockBuffer - sizeof(blockBuffer), SEEK_CUR);
    fread(&blockBuffer, sizeof(blockBuffer), 1, stream);
    nodeSize += blockBuffer;
    nodeSize += sizeof(uint8_t) * 2;
    node->data = malloc(nodeSize);
    fseek(stream, initialPosition, SEEK_SET);
    fread(node->data, nodeSize, 1, stream);

    uint8_t *buffer = node->data;
    node->next = (struct TreeNode *)(*((uint64_t *)(buffer + nodeSize - blockBuffer - 28 - sizeof(uint8_t) * 2)));
    node->parent = (struct TreeNode *)(*((uint64_t *)(buffer + nodeSize - blockBuffer - 52 - sizeof(uint8_t) * 2)));
    node->idSymbol = *((uint64_t *)(buffer + 12));
}

int DlgNodeSequenceRead(FILE *stream, void **treeNode, uint32_t flags)
{
    printf("DlgNodeSequenceRead\n");

    struct TreeNode *node = *treeNode;
    long initialPosition = ftell(stream);

    uint32_t nodeBlock;
    uint32_t elementsBlock;
    uint32_t criteriaBlock;
    uint32_t nodeSize = 0;
    fread(&nodeBlock, sizeof(nodeBlock), 1, stream);
    nodeSize += nodeBlock;
    fseek(stream, nodeBlock - sizeof(nodeBlock), SEEK_CUR);
    fread(&elementsBlock, sizeof(elementsBlock), 1, stream);
    nodeSize += elementsBlock;
    fseek(stream, elementsBlock - sizeof(elementsBlock) + 2 * sizeof(uint32_t), SEEK_CUR);
    fread(&criteriaBlock, sizeof(criteriaBlock), 1, stream);
    nodeSize += criteriaBlock + 2 * sizeof(uint32_t);
    node->data = malloc(nodeSize);
    fseek(stream, initialPosition, SEEK_SET);
    fread(node->data, nodeSize, 1, stream);

    uint8_t *buffer = node->data;
    node->next = (struct TreeNode *)(*((uint64_t *)(buffer + nodeSize - criteriaBlock - elementsBlock - 28 - sizeof(uint32_t) * 2)));
    node->parent = (struct TreeNode *)(*((uint64_t *)(buffer + nodeSize - criteriaBlock - elementsBlock - 52 - sizeof(uint32_t) * 2)));
    node->idSymbol = *((uint64_t *)(buffer + 12));
}

int DlgNodeJumpRead(FILE *stream, void **treeNode, uint32_t flags)
{
    printf("DlgNodeJumpRead\n");

    struct TreeNode *node = *treeNode;

    uint32_t blockBuffer;
    fread(&blockBuffer, sizeof(blockBuffer), 1, stream);
    node->data = malloc(blockBuffer + 60);
    fseek(stream, -sizeof(blockBuffer), SEEK_CUR);
    fread(node->data, blockBuffer + 60, 1, stream);

    uint8_t *buffer = node->data;
    node->next = (struct TreeNode *)(*((uint64_t *)(buffer + blockBuffer - 28)));
    node->parent = (struct TreeNode *)(*((uint64_t *)(buffer + blockBuffer - 52)));
    node->idSymbol = *((uint64_t *)(buffer + 12));
}

int DlgNodeStartRead(FILE *stream, void **treeNode, uint32_t flags)
{
    printf("DlgNodeStartRead\n");

    struct TreeNode *node = *treeNode;
    long initialPosition = ftell(stream);

    uint32_t blockBuffer;
    uint32_t nodeSize = 0;
    fread(&blockBuffer, sizeof(blockBuffer), 1, stream);
    nodeSize += blockBuffer;
    fseek(stream, blockBuffer - sizeof(blockBuffer), SEEK_CUR);
    fread(&blockBuffer, sizeof(blockBuffer), 1, stream);
    nodeSize += blockBuffer;
    node->data = malloc(nodeSize);
    fseek(stream, initialPosition, SEEK_SET);
    fread(node->data, nodeSize, 1, stream);

    uint8_t *buffer = node->data;
    node->next = (struct TreeNode *)(*((uint64_t *)(buffer + nodeSize - blockBuffer - 28)));
    node->parent = (struct TreeNode *)(*((uint64_t *)(buffer + nodeSize - blockBuffer - 52)));
    node->idSymbol = *((uint64_t *)(buffer + 12));
}

int DlgNodeNotesRead(FILE *stream, void **treeNode, uint32_t flags)
{
    printf("DlgNodeNotesRead\n");

    struct TreeNode *node = *treeNode;
    long initialPosition = ftell(stream);

    uint32_t blockBuffer;
    uint32_t nodeSize = 0;
    fread(&blockBuffer, sizeof(blockBuffer), 1, stream);
    nodeSize += blockBuffer;
    fseek(stream, blockBuffer - sizeof(blockBuffer), SEEK_CUR);
    fread(&blockBuffer, sizeof(blockBuffer), 1, stream);
    nodeSize += blockBuffer;
    node->data = malloc(nodeSize);
    fseek(stream, initialPosition, SEEK_SET);
    fread(node->data, nodeSize, 1, stream);

    uint8_t *buffer = node->data;
    node->next = (struct TreeNode *)(*((uint64_t *)(buffer + nodeSize - blockBuffer - 28)));
    node->parent = (struct TreeNode *)(*((uint64_t *)(buffer + nodeSize - blockBuffer - 52)));
    node->idSymbol = *((uint64_t *)(buffer + 12));
}

int DlgNodeExitRead(FILE *stream, void **treeNode, uint32_t flags)
{
    DlgNodeMarkerRead(stream, treeNode, flags);
}

int DlgNodeConditionalRead(FILE *stream, void **treeNode, uint32_t flags)
{
    DlgNodeStartRead(stream, treeNode, flags);
}

int DlgFolderChildRead(FILE *stream, void **treeNode, uint32_t flags)
{
    DlgConditionalCaseRead(stream, treeNode, flags);
}

int DlgChoicesChildPreRead(FILE *stream, void **treeNode, uint32_t flags)
{
    DlgConditionalCaseRead(stream, treeNode, flags);
}

int DlgChoicesChildPostRead(FILE *stream, void **treeNode, uint32_t flags)
{
    DlgConditionalCaseRead(stream, treeNode, flags);
}

/* New Code */

int PairRead(FILE *stream, struct TreeNode *pair, uint32_t flags)
{
    assert(pair->childCount == 2);
    readMetaClass(stream, pair->children[0], flags);
    readMetaClass(stream, pair->children[1], flags);

    return 0;
}

int int4Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    node->dataSize = sizeof(uint32_t);
    node->data = malloc(node->data);
    fread(node->data, node->dataSize, 1, stream);

    return 0;
}

int LogicItemRead(FILE *stream, struct TreeNode *logicItem, uint32_t flags)
{
    logicItem->childCount = 6;
    logicItem->children = malloc(logicItem->childCount * sizeof(struct TreeNode *));

    fseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    logicItem->children[0] = calloc(1, sizeof(struct TreeNode));
    logicItem->children[0]->typeSymbol = 0xcd75dc4f6b9f15d2; // crc64 of "PropertySet"
    PropRead(stream, logicItem->children[0], flags);

    fseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    logicItem->children[1] = calloc(1, sizeof(struct TreeNode));
    logicItem->children[1]->typeSymbol = 0xcd9c6e605f5af4b4; // crc64 of "String"
    StringRead(stream, logicItem->children[1], flags);

    fseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    logicItem->children[2] = calloc(1, sizeof(struct TreeNode));
    logicItem->children[2]->typeSymbol = 0x42f3cd4fe3d6d0d; // crc64 of "Map<Symbol,bool,less<Symbol>>"
    fread(&(logicItem->children[2]->childCount), sizeof(logicItem->children[2]->childCount), 1, stream);
    logicItem->children[2]->children = malloc(logicItem->children[2]->childCount * sizeof(struct TreeNode *));
    for (uint32_t i = 0; i < logicItem->children[2]->childCount; ++i)
    {
        logicItem->children[2]->children[i] = calloc(1, sizeof(struct TreeNode));
        fread(&(logicItem->children[2]->children[i]->nameSymbol), sizeof(logicItem->children[2]->children[i]->nameSymbol), 1, stream);
        logicItem->children[2]->children[i]->typeSymbol = 0x9004c5587575d6c0; // crc64 of "bool"
        BoolRead(stream, logicItem->children[2]->children[i], flags);
    }

    fseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    logicItem->children[3] = calloc(1, sizeof(struct TreeNode));
    logicItem->children[3]->typeSymbol = 0x33c56d9e4438fcd9; // crc64 of "Map<Symbol,int,less<Symbol>>"
    fread(&(logicItem->children[3]->childCount), sizeof(logicItem->children[3]->childCount), 1, stream);
    logicItem->children[3]->children = malloc(logicItem->children[3]->childCount * sizeof(struct TreeNode *));
    for (uint32_t i = 0; i < logicItem->children[3]->childCount; ++i)
    {
        logicItem->children[3]->children[i] = calloc(1, sizeof(struct TreeNode));
        fread(&(logicItem->children[3]->children[i]->nameSymbol), sizeof(logicItem->children[3]->children[i]->nameSymbol), 1, stream);
        logicItem->children[3]->children[i]->typeSymbol = 0x99d7c52ea7f0f97d; // crc64 of "int"
        int4Read(stream, logicItem->children[3]->children[i], flags);
    }

    fseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    logicItem->children[4] = calloc(1, sizeof(struct TreeNode));
    logicItem->children[4]->typeSymbol = 0x33c56d9e4438fcd9; // crc64 of "Map<Symbol,int,less<Symbol>>"
    fread(&(logicItem->children[4]->childCount), sizeof(logicItem->children[4]->childCount), 1, stream);
    logicItem->children[4]->children = malloc(logicItem->children[4]->childCount * sizeof(struct TreeNode *));
    for (uint32_t i = 0; i < logicItem->children[4]->childCount; ++i)
    {
        logicItem->children[4]->children[i] = calloc(1, sizeof(struct TreeNode));
        fread(&(logicItem->children[4]->children[i]->nameSymbol), sizeof(logicItem->children[4]->children[i]->nameSymbol), 1, stream);
        logicItem->children[4]->children[i]->typeSymbol = 0x99d7c52ea7f0f97d; // crc64 of "int"
        int4Read(stream, logicItem->children[4]->children[i], flags);
    }

    fseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    logicItem->children[5] = calloc(1, sizeof(struct TreeNode));
    logicItem->children[5]->typeSymbol = 0xcd9c6e605f5af4b4; // crc64 of "String"
    StringRead(stream, logicItem->children[5], flags);

    for (uint32_t i = 0; i < logicItem->childCount; ++i)
    {
        logicItem->children[i]->parent = logicItem;
    }

    return 0;
}

int LogicGroupRead(FILE *stream, struct TreeNode *logicGroup, uint32_t flags)
{
    logicGroup->dataSize = 3 * sizeof(uint32_t);
    logicGroup->data = malloc(logicGroup->dataSize);
    fread(logicGroup->data, sizeof(uint32_t), 1, stream);

    logicGroup->childCount = 3;
    logicGroup->children = calloc(logicGroup->childCount, sizeof(struct TreeNode *));

    fseek(stream, sizeof(uint32_t), SEEK_CUR);                    // Skip block
    logicGroup->children[0] = calloc(1, sizeof(struct TreeNode)); // map child
    logicGroup->children[0]->typeSymbol = 0xa98eae8f6cff1125;     // crc64 of "Map<String,LogicGroup::LogicItem,less<String>>"
    fread(&logicGroup->children[0]->childCount, sizeof(logicGroup->children[0]->childCount), 1, stream);
    logicGroup->children[0]->children = malloc(logicGroup->children[0]->childCount * sizeof(struct TreeNode *));

    for (uint32_t i = 0; i < logicGroup->children[0]->childCount; ++i)
    {
        logicGroup->children[0]->children[i] = calloc(1, sizeof(struct TreeNode));
        // So I am going to skip storing the string for now since the name is already stored in the logic item. Storing the name could have been useful if I was using an actual map.
        uint32_t stringSize;
        fread(&stringSize, sizeof(stringSize), 1, stream);
        fseek(stream, stringSize - sizeof(uint32_t), SEEK_CUR);
        logicGroup->children[0]->children[i]->typeSymbol = 0xb705c1a4e971420e; // crc64 of "LogicGroup::LogicItem"
        LogicItemRead(stream, logicGroup->children[0]->children[i], flags);
        logicGroup->children[0]->children[i]->parent = logicGroup->children[0];
    }

    fseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    logicGroup->children[1] = calloc(1, sizeof(struct TreeNode));
    logicGroup->children[1]->typeSymbol = 0x36d2e9352e9eed3e; // crc64 of "DCArray<LogicGroup>"
    fread(&logicGroup->children[1]->childCount, sizeof(logicGroup->children[1]->childCount), 1, stream);
    logicGroup->children[1]->children = malloc(logicGroup->children[0]->childCount * sizeof(struct TreeNode *));
    for (uint32_t i = 0; i < logicGroup->children[1]->childCount; ++i)
    {
        logicGroup->children[1]->children[i] = calloc(1, sizeof(struct TreeNode));
        logicGroup->children[1]->children[i]->typeSymbol = 0x114636e822613a22; // crc64 of "LogicGroup"
        LogicGroupRead(stream, logicGroup->children[1]->children[i], flags);
        logicGroup->children[1]->children[i]->parent = logicGroup->children[1];
    }

    fread((uint8_t *)(logicGroup->data) + sizeof(uint32_t), 2 * sizeof(uint32_t), 1, stream);
    fseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip block
    StringRead(stream, logicGroup->children[2], flags);

    return 0;
}

int RuleRead(FILE *stream, struct TreeNode *rule, uint32_t flags)
{
    rule->childCount = 6;
    rule->children = malloc(rule->childCount * sizeof(struct TreeNode *));

    fseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip name block
    rule->children[0] = calloc(1, sizeof(struct TreeNode));
    rule->children[0]->typeSymbol = 0xcd9c6e605f5af4b4; // crc64 of "string"
    StringRead(stream, rule->children[0], flags);

    fseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip runtimePropName block
    rule->children[1] = calloc(1, sizeof(struct TreeNode));
    rule->children[1]->typeSymbol = 0xcd9c6e605f5af4b4; // crc64 of "string"
    StringRead(stream, rule->children[1], flags);

    rule->dataSize = sizeof(uint32_t);
    rule->data = malloc(rule->dataSize);
    fread(rule->data, rule->dataSize, 1, stream);

    fseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip conditions block
    rule->children[2] = calloc(1, sizeof(struct TreeNode));
    rule->children[2]->typeSymbol = 0x114636e822613a22; // crc64 of "LogicGroup"
    LogicGroupRead(stream, rule->children[2], flags);

    fseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip actions block
    rule->children[3] = calloc(1, sizeof(struct TreeNode));
    rule->children[3]->typeSymbol = 0x114636e822613a22; // crc64 of "LogicGroup"
    LogicGroupRead(stream, rule->children[3], flags);

    fseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip else (nullptr block :)) block
    rule->children[4] = calloc(1, sizeof(struct TreeNode));
    rule->children[4]->typeSymbol = 0x114636e822613a22; // crc64 of "LogicGroup"
    LogicGroupRead(stream, rule->children[4], flags);

    fseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip agentCategoryBlock block
    rule->children[5] = calloc(1, sizeof(struct TreeNode));
    rule->children[5]->typeSymbol = 0xcd9c6e605f5af4b4; // crc64 of "string"
    StringRead(stream, rule->children[5], flags);

    for (uint32_t i = 0; i < rule->childCount; ++i)
    {
        rule->children[i]->parent = rule;
    }

    return 0;
}

int DlgVisibilityConditonsRead(FILE *stream, struct TreeNode *visCond, uint32_t flags)
{
    visCond->dataSize = 17;
    visCond->data = malloc(visCond->dataSize);
    fread(visCond->data, visCond->dataSize, 1, stream);
    fseek(stream, sizeof(uint32_t), SEEK_CUR); // Skip string block
    visCond->childCount = 1;
    uint32_t visCondFlags = *((uint32_t *)((uint8_t *)(visCond->data) + 1));
    if (visCondFlags & 1)
    {
        ++visCond->childCount;
    }
    visCond->children = calloc(visCond->childCount, sizeof(struct TreeNode *));
    visCond->children[0] = calloc(1, sizeof(struct TreeNode));
    visCond->children[0]->typeSymbol = 0xcd9c6e605f5af4b4; // crc64 of "string"
    StringRead(stream, visCond->children[0], flags);
    visCond->children[0]->parent = visCond;
    if (visCondFlags & 1)
    {
        visCond->children[1]->typeSymbol = 0x5549d9efe67060e1; // crc64 of "rule"
        RuleRead(stream, visCond->children[1], flags);
        visCond->children[1]->parent = visCond;
    }
    return 0;
}

int DlgObjectPropsRead(FILE *stream, struct TreeNode *objectProps, uint32_t flags)
{
    objectProps->dataSize = sizeof(uint32_t);
    objectProps->data = malloc(objectProps->dataSize);
    fread(objectProps->data, objectProps->dataSize, 1, stream);
    objectProps->childCount = ((*(uint32_t *)(objectProps->data) & 0x1) > 0) + ((*(uint32_t *)(objectProps->data) & 0x2) > 0) + ((*(uint32_t *)(objectProps->data) & 0x4) > 0);
    objectProps->children = malloc(objectProps->childCount * sizeof(struct TreeNode *));

    for (uint32_t i = 0; i < objectProps->childCount; ++i)
    {
        objectProps->children[i] = calloc(1, sizeof(struct TreeNode));
        objectProps->children[i]->typeSymbol = 0xcd75dc4f6b9f15d2; // crc64 of "PropertySet"
        PropRead(stream, objectProps->children[i], flags);
        objectProps->children[i]->parent = objectProps;
    }
    return 0;
}

int DlgNodeRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
}
