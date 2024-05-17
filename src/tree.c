#include <tree.h>
#include <stdlib.h>

void treeFree(struct DlgTreeNode *root)
{
    if (root->data != NULL)
    {
        free(root->data);
    }
    if (root->next != NULL)
    {
        treeFree(root->next);
    }
    for (uint32_t i = 0; i < root->childCount; ++i)
    {
        if (root->children[i] != NULL)
        {
            treeFree(root->children[i]);
        }
    }
}
