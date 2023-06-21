#include "utils.h"
#include "rTrees.h"

// recursive function to search for all entries that overlap with search_rect
void search_utility(Node *currNode, MBR *search_rect)
{
    // base case
    if (currNode == NULL)
    {
        return;
    }
    
    // if currNode is a leaf node, print all entries that overlap with search_rect
    if (currNode->isLeaf)
    {
        for (int i = 0; i < currNode->noOfEntries; i++)
        {
            if (isOverlapping(currNode->entries[i]->rectangle, search_rect))
            {
                printEntry(currNode->entries[i]);
            }
        }
        return;
    }

    // if currNode is not a leaf node, recursively call search_utility on all entries that overlap with search_rect
    for (int i = 0; i < currNode->noOfEntries; i++)
    {
        if (isOverlapping(currNode->entries[i]->rectangle, search_rect))
        {
            search_utility(currNode->entries[i]->childNode, search_rect);
        }
    }
}

// wrapper function for search_utility
void search(rTree *tree, int minX, int maxX, int minY, int maxY)
{
    blue("[Entering Search]\n");

    MBR *search_rect = createMBR(minX, maxX, minY, maxY);
    
    search_utility(tree->root, search_rect);
    
    blue("[Exiting Search]\n")
}