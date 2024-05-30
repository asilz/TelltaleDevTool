#include <container.h>
#include <stdlib.h>
#include <intrinsic.h>

int genericArrayRead(FILE *stream, struct TreeNode *node, uint32_t flags, serializeFunction readFunction, uint64_t typeSymbol)
{
    node->childCount = 1;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));
    node->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->typeSymbol = 0x99d7c52ea7f0f97d; // crc64 of "int"
    intrinsic4Read(stream, node->children[0], flags);
    node->children[0]->parent = node;

    node->childCount += *(uint32_t *)(node->children[0]->data.staticBuffer);
    node->children = realloc(node->children, node->childCount * sizeof(struct TreeNode *));

    for (uint32_t i = 1; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->typeSymbol = typeSymbol;
        node->children[i]->parent = node;
        readFunction(stream, node->children[i], flags);
    }
    return 0;
}

int genericMapRead(FILE *stream, struct TreeNode *node, uint32_t flags, serializeFunction readFunction1, serializeFunction readFunction2, uint64_t typeSymbol1, uint64_t typeSymbol2)
{
    node->childCount = 1;
    node->children = malloc(node->childCount * sizeof(struct TreeNode *));
    node->children[0] = calloc(1, sizeof(struct TreeNode));
    node->children[0]->parent = node;
    node->children[0]->typeSymbol = 0x99d7c52ea7f0f97d; // crc64 of "int"
    intrinsic4Read(stream, node->children[0], flags);
    node->childCount += (*(uint32_t *)(node->children[0]->data.staticBuffer)) * 2;
    node->children = realloc(node->children, node->childCount * sizeof(struct TreeNode *));

    for (uint32_t i = 1; i < node->childCount; ++i)
    {
        node->children[i] = calloc(1, sizeof(struct TreeNode));
        node->children[i]->parent = node;
        if (i % 2) // if odd
        {
            node->children[i]->typeSymbol = typeSymbol1;
            readFunction1(stream, node->children[i], flags);
        }
        else
        {
            node->children[i]->typeSymbol = typeSymbol2;
            readFunction2(stream, node->children[i], flags);
        }
    }
    return 0;
}