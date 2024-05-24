#include <tree.h>
#include <stdlib.h>

void treeFree(struct TreeNode *root)
{
    if (root->dataSize > sizeof(root->data))
    {
        free(root->data.dynamicBuffer);
        root->data.dynamicBuffer = NULL;
    }

        for (uint32_t i = 0; i < root->childCount; ++i)
    {
        if (root->children[i] != NULL)
        {
            treeFree(root->children[i]);
            free(root->children[i]);
            root->children[i] = NULL;
        }
    }

    sizeof(struct TreeNode);
}
