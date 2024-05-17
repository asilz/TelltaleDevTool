#include <inttypes.h>

struct DlgTreeNode
{
    uint64_t idSymbol;
    uint64_t typeSymbol;
    struct DlgTreeNode *parent;
    struct DlgTreeNode *next;
    uint32_t childCount;
    uint16_t parentIsHash;
    uint16_t nextIsHash;
    struct DlgTreeNode **children;
    void *data;
};

void treeFree(struct DlgTreeNode *root);