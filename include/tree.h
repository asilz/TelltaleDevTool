#ifndef TREE_H
#define TREE_H
#include <inttypes.h>

struct TreeNode
{
    union Data
    {
        uint8_t *dynamicBuffer;
        uint8_t staticBuffer[sizeof(uint8_t *)];
    } data;
    uint64_t typeSymbol;
    struct TreeNode *parent;
    struct TreeNode **children;
    uint32_t dataSize;
    uint16_t childCount; // Reason I placed members like so is to avoid padding. Padding is the bane of my existence.
    uint8_t isBlocked;   // I hate my life. I am forced to spend 8 bytes on a BOOL of all things! This is pain. At least this might allow me to avoid making write functions for literally everything
    uint8_t serializeType;
};

void treeFree(struct TreeNode *root);

#endif