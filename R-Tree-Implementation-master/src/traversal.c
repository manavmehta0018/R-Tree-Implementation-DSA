#include "utils.h"
#include "rTrees.h"

// To traverse the tree in pre-order fashion
void preOrderTraversal_Utility(Node *currNode)
{
    // base case
    if (currNode == NULL)
    {
        return;
    }

    printNode(currNode);
    
    // recursively call the function for the child nodes
    for (int i = 0; i < currNode->noOfEntries; i++)
    {
        preOrderTraversal_Utility(currNode->entries[i]->childNode);
    }
    return;
}

// wrapper function for the traverse function
void preOrderTraversal(rTree *tree)
{
    printEntry(tree->start);
    preOrderTraversal_Utility(tree->root);
}
