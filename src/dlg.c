#include <types.h>
#include <tree.h>
#include <stdlib.h>
#include <meta.h>
#include <dlg.h>

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
    folder->childSet = malloc(sizeof(struct DlgTreeNode *) * folder->childCount);
    for (int i = 0; i < folder->childCount; ++i)
    {
        folder->childSet[i] = calloc(1, sizeof(struct DlgTreeNode));
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

    dlg->nodes = malloc(sizeof(struct DlgTreeNode *) * dlg->nodeCount);

    for (uint32_t i = 0; i < dlg->nodeCount; ++i)
    {
        dlg->nodes[i] = calloc(1, sizeof(struct DlgTreeNode));
        fread(&dlg->nodes[i]->typeSymbol, sizeof(dlg->nodes[i]->typeSymbol), 1, stream);
        readMetaClass(stream, (void **)(dlg->nodes + i), dlg->nodes[i]->typeSymbol, flags);
    }

    printf("nodes read\n");

    for (uint32_t i = 0; i < dlg->folderCount; ++i)
    {
        for (uint32_t j = 0; j < dlg->folders[i].childCount; ++j)
        {
            struct DlgTreeNode *node = dlg->folders[i].childSet[j];
            printf("child assemble\n");
            while (node != NULL && node != (struct DlgTreeNode *)0x988d0903f713877b)
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
    struct DlgTreeNode *node = *treeNode;
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
    node->next = (struct DlgTreeNode *)(*((uint64_t *)(buffer + nodeSize - blockBuffer - 28)));
    node->parent = (struct DlgTreeNode *)(*((uint64_t *)(buffer + nodeSize - blockBuffer - 52)));
    node->idSymbol = *((uint64_t *)(buffer + 12));

    return 0;
}

int DlgLineCollectionRead(FILE *stream, void **treeNode, uint32_t flags)
{
    printf("DlgLineCollectionRead\n");
    struct DlgTreeNode *node = *treeNode;

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
    struct DlgTreeNode *node = *treeNode;
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
    node->next = (struct DlgTreeNode *)(*((uint64_t *)(buffer + nodeSize - blockBuffer - 28)));
    node->parent = (struct DlgTreeNode *)(*((uint64_t *)(buffer + nodeSize - blockBuffer - 52)));

    if (nodeFlags & 0x1)
    {
        struct DlgTreeNode *noteCollection = calloc(1, sizeof(struct DlgTreeNode));
        NoteCollectionRead(stream, (void **)(&noteCollection), flags);
        noteCollection->parent = node;
        node->children = malloc(sizeof(struct DlgTreeNode *));
        *(node->children) = noteCollection;
        node->childCount = 1;
    }

    if (nodeFlags & 0x2)
    {
        struct DlgTreeNode *dlgLineCollection = calloc(1, sizeof(struct DlgTreeNode));
        DlgLineCollectionRead(stream, (void **)(&dlgLineCollection), flags);
        dlgLineCollection->parent = node;
        node->children = malloc(sizeof(struct DlgTreeNode *));
        *(node->children) = dlgLineCollection;
        node->childCount = 1;
    }

    printf(" %lx\n", node->idSymbol);

    return 0;
}

int DlgNodeChoicesRead(FILE *stream, void **treeNode, uint32_t flags)
{
    printf("DlgNodeChoice\n");
    struct DlgTreeNode *node = *treeNode;
    node->childCount = 0;

    long initialPosition = ftell(stream);
    uint32_t blockBuffer;

    fread(&blockBuffer, sizeof(uint32_t), 1, stream);
    node->data = malloc(blockBuffer);

    fseek(stream, initialPosition, SEEK_SET);
    fread(node->data, blockBuffer, 1, stream);

    uint8_t *buffer = node->data;
    node->idSymbol = *((uint64_t *)(buffer + 12));
    node->next = (struct DlgTreeNode *)(*((uint64_t *)(buffer + blockBuffer - 28)));
    node->parent = (struct DlgTreeNode *)(*((uint64_t *)(buffer + blockBuffer - 52)));

    uint32_t childCountBuffer;
    uint32_t i = 0;

    // Choice child set
    fseek(stream, 8, SEEK_CUR); // Skip blocks
    fread(&childCountBuffer, sizeof(childCountBuffer), 1, stream);
    node->childCount += childCountBuffer;
    node->children = malloc(node->childCount * sizeof(struct DlgTreeNode *));
    for (i; i < node->childCount; ++i)
    {
        struct DlgTreeNode *child = calloc(1, sizeof(struct DlgTreeNode));
        fread(&child->typeSymbol, sizeof(child->typeSymbol), 1, stream);
        readMetaClass(stream, (void **)(&child), child->typeSymbol, flags);
        child->parent = node;
        node->children[i] = child;
    }

    // Pre choice child set
    fseek(stream, 8, SEEK_CUR); // Skip blocks
    fread(&childCountBuffer, sizeof(childCountBuffer), 1, stream);
    node->childCount += childCountBuffer;
    node->children = realloc(node->children, node->childCount * sizeof(struct DlgTreeNode *));
    for (i; i < node->childCount; ++i)
    {
        struct DlgTreeNode *child = calloc(1, sizeof(struct DlgTreeNode));
        fread(&child->typeSymbol, sizeof(child->typeSymbol), 1, stream);
        readMetaClass(stream, (void **)(&child), child->typeSymbol, flags);
        child->parent = node;
        node->children[i] = child;
    }

    // Post choice child set
    fseek(stream, 8, SEEK_CUR); // Skip blocks
    fread(&childCountBuffer, sizeof(childCountBuffer), 1, stream);
    node->childCount += childCountBuffer;
    node->children = realloc(node->children, node->childCount * sizeof(struct DlgTreeNode *));
    for (i; i < node->childCount; ++i)
    {
        struct DlgTreeNode *child = calloc(1, sizeof(struct DlgTreeNode));
        fread(&child->typeSymbol, sizeof(child->typeSymbol), 1, stream);
        readMetaClass(stream, (void **)(&child), child->typeSymbol, flags);
        child->parent = node;
        node->children[i] = child;
    }
}

int DlgChoiceRead(FILE *stream, void **treeNode, uint32_t flags)
{
    printf("DlgChoiceRead\n");

    struct DlgTreeNode *node = *treeNode;
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

    struct DlgTreeNode *node = *treeNode;
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
    node->next = (struct DlgTreeNode *)(*((uint64_t *)(buffer + nodeSize - blockBuffer - 28)));
    node->parent = (struct DlgTreeNode *)(*((uint64_t *)(buffer + nodeSize - blockBuffer - 52)));
    node->idSymbol = *((uint64_t *)(buffer + 12));
}

int DlgNodeMarkerRead(FILE *stream, void **treeNode, uint32_t flags)
{
    printf("DlgNodeMarkerRead\n");

    struct DlgTreeNode *node = *treeNode;

    uint32_t blockBuffer;
    fread(&blockBuffer, sizeof(blockBuffer), 1, stream);
    node->data = malloc(blockBuffer);
    fseek(stream, -sizeof(blockBuffer), SEEK_CUR);
    fread(node->data, blockBuffer, 1, stream);

    uint8_t *buffer = node->data;
    node->next = (struct DlgTreeNode *)(*((uint64_t *)(buffer + blockBuffer - 28)));
    node->parent = (struct DlgTreeNode *)(*((uint64_t *)(buffer + blockBuffer - 52)));
    node->idSymbol = *((uint64_t *)(buffer + 12));

    printf("%lx\n", node->idSymbol);
}

int DlgNodeIdleRead(FILE *stream, void **treeNode, uint32_t flags)
{
    printf("DlgNodeIdleRead\n");

    struct DlgTreeNode *node = *treeNode;

    uint32_t blockBuffer;
    fread(&blockBuffer, sizeof(blockBuffer), 1, stream);
    node->data = malloc(blockBuffer + 32);
    fseek(stream, -sizeof(blockBuffer), SEEK_CUR);
    fread(node->data, blockBuffer + 32, 1, stream);

    uint8_t *buffer = node->data;
    node->next = (struct DlgTreeNode *)(*((uint64_t *)(buffer + blockBuffer - 28)));
    node->parent = (struct DlgTreeNode *)(*((uint64_t *)(buffer + blockBuffer - 52)));
    node->idSymbol = *((uint64_t *)(buffer + 12));
}

int DlgConditionalCaseRead(FILE *stream, void **treeNode, uint32_t flags)
{
    printf("DlgConditionalCaseRead\n");

    struct DlgTreeNode *node = *treeNode;

    uint32_t blockBuffer;
    fread(&blockBuffer, sizeof(blockBuffer), 1, stream);
    node->data = malloc(blockBuffer);
    fseek(stream, -sizeof(blockBuffer), SEEK_CUR);
    fread(node->data, blockBuffer, 1, stream);

    uint8_t *buffer = node->data;
    node->next = (struct DlgTreeNode *)(*((uint64_t *)(buffer + 36)));
    node->parent = (struct DlgTreeNode *)(*((uint64_t *)(buffer + blockBuffer - 12)));
    node->idSymbol = *((uint64_t *)(buffer + 16));
}

int DlgNodeScriptRead(FILE *stream, void **treeNode, uint32_t flags)
{
    printf("DlgNodeScriptRead\n");

    struct DlgTreeNode *node = *treeNode;
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
    node->next = (struct DlgTreeNode *)(*((uint64_t *)(buffer + nodeSize - blockBuffer - 28 - sizeof(uint8_t) * 2)));
    node->parent = (struct DlgTreeNode *)(*((uint64_t *)(buffer + nodeSize - blockBuffer - 52 - sizeof(uint8_t) * 2)));
    node->idSymbol = *((uint64_t *)(buffer + 12));
}

int DlgNodeSequenceRead(FILE *stream, void **treeNode, uint32_t flags)
{
    printf("DlgNodeSequenceRead\n");

    struct DlgTreeNode *node = *treeNode;
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
    node->next = (struct DlgTreeNode *)(*((uint64_t *)(buffer + nodeSize - criteriaBlock - elementsBlock - 28 - sizeof(uint32_t) * 2)));
    node->parent = (struct DlgTreeNode *)(*((uint64_t *)(buffer + nodeSize - criteriaBlock - elementsBlock - 52 - sizeof(uint32_t) * 2)));
    node->idSymbol = *((uint64_t *)(buffer + 12));
}

int DlgNodeJumpRead(FILE *stream, void **treeNode, uint32_t flags)
{
    printf("DlgNodeJumpRead\n");

    struct DlgTreeNode *node = *treeNode;

    uint32_t blockBuffer;
    fread(&blockBuffer, sizeof(blockBuffer), 1, stream);
    node->data = malloc(blockBuffer + 60);
    fseek(stream, -sizeof(blockBuffer), SEEK_CUR);
    fread(node->data, blockBuffer + 60, 1, stream);

    uint8_t *buffer = node->data;
    node->next = (struct DlgTreeNode *)(*((uint64_t *)(buffer + blockBuffer - 28)));
    node->parent = (struct DlgTreeNode *)(*((uint64_t *)(buffer + blockBuffer - 52)));
    node->idSymbol = *((uint64_t *)(buffer + 12));
}

int DlgNodeStartRead(FILE *stream, void **treeNode, uint32_t flags)
{
    printf("DlgNodeStartRead\n");

    struct DlgTreeNode *node = *treeNode;
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
    node->next = (struct DlgTreeNode *)(*((uint64_t *)(buffer + nodeSize - blockBuffer - 28)));
    node->parent = (struct DlgTreeNode *)(*((uint64_t *)(buffer + nodeSize - blockBuffer - 52)));
    node->idSymbol = *((uint64_t *)(buffer + 12));
}

int DlgNodeNotesRead(FILE *stream, void **treeNode, uint32_t flags)
{
    printf("DlgNodeNotesRead\n");

    struct DlgTreeNode *node = *treeNode;
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
    node->next = (struct DlgTreeNode *)(*((uint64_t *)(buffer + nodeSize - blockBuffer - 28)));
    node->parent = (struct DlgTreeNode *)(*((uint64_t *)(buffer + nodeSize - blockBuffer - 52)));
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
