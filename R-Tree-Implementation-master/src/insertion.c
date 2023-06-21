#include "rTrees.h"
#include "utils.h"

// choose the most optimal leaf node to insert the new rectangle
Node *chooseLeaf(Node *currNode, MBR *targetRect)
{
    blue("[Entering chooseLeaf]\n");

    // if currNode is a leaf node, return it
    if (currNode->isLeaf)
    {
        return currNode;
    }

    int minArea = INF;
    int minIndex = 0;
    int minCurrArea = INF;

    // find the entry with the least area enlargement
    for (int i = 0; i < currNode->noOfEntries; i++)
    {
        // find area of the rectangle formed by union of the entry and the target rectangle
        int currArea = findArea(currNode->entries[i]->rectangle);
        MBR *newRect = unionMBR(currNode->entries[i]->rectangle, targetRect);
        int newArea = findArea(newRect);
        free(newRect);

        // find the difference in old area and the new area
        int diffArea = newArea - currArea;
        int minCurrArea = min(minCurrArea, currArea);

        // if the difference is less than the minimum difference, update the minimum difference and the index
        if ((diffArea < minArea) || (diffArea == minArea && currArea < minCurrArea))
        {
            minArea = diffArea;
            minIndex = i;
        }
    }

    blue("[Exiting chooseLeaf]\n");
    return chooseLeaf(currNode->entries[minIndex]->childNode, targetRect);
}

// pick group representative for splitting current node
void pickSeeds(Node *currNode, int *seed1, int *seed2)
{
    blue("[Picking seeds]\n");
    // store the max difference of two rectangles
    int maxDiff = -1;
    for (int i = 0; i < currNode->noOfEntries; i++)
    {
        for (int j = i + 1; j < currNode->noOfEntries; j++)
        {
            MBR *unionRect = unionMBR(currNode->entries[i]->rectangle, currNode->entries[j]->rectangle);

            // find the difference in area of the union rectangle and the sum of the areas of the two rectangles
            int diff = findArea(unionRect) - findArea(currNode->entries[i]->rectangle) - findArea(currNode->entries[j]->rectangle);
            free(unionRect);
            if(i==0 && j==1)
            {
                maxDiff = diff;
                *seed1 = i;
                *seed2 = j;
            }
            // if the difference is greater than the maximum difference, update the maximum difference and the seeds
            if (diff > maxDiff)
            {
                maxDiff = diff;
                *seed1 = i;
                *seed2 = j;
            }
        }
    }
    blue("[Exiting pickSeeds]\n");
    return;
}

// decide which entry to choose next, and which group to put it in
int pickNext(Node *currNode, Entry *group1, Entry *group2, bool *res)
{
    blue("[Entering pickNext]\n");

    int maxDiff = 0;
    int maxIndex = 0;

    MBR *g1rect = group1->rectangle;
    MBR *g2rect = group2->rectangle;

    for (int i = 0; i < currNode->noOfEntries; i++)
    {
        // find the difference in area of the new rectangle formed by union of the entry and the group rectangle of group1
        MBR *Rect1 = unionMBR(g1rect, currNode->entries[i]->rectangle);
        int diff1 = findArea(Rect1) - findArea(g1rect);
        free(Rect1);

        // find the difference in area of the new rectangle formed by union of the entry and the group rectangle of group2
        MBR *Rect2 = unionMBR(g2rect, currNode->entries[i]->rectangle);
        int diff2 = findArea(Rect2) - findArea(g2rect);
        free(Rect2);

        // find the difference between the two differences
        int diff = abs(diff1 - diff2);
        // condition and tie breakers for assigning the entry to a group
        if (diff > maxDiff)
        {
            maxDiff = diff;
            maxIndex = i;
            if (diff1 < diff2)
                *res = true;
            else if (diff1 > diff2)
                *res = false;
            else if (findArea(g1rect) < findArea(g2rect))
                *res = true;
            else if (findArea(g1rect) > findArea(g2rect))
                *res = false;
            else if (group1->childNode->noOfEntries < group2->childNode->noOfEntries)
                *res = true;
            else
                *res = false;
        }
    }

    blue("[Exiting pickNext]\n");
    return maxIndex;
}

// split the node into two groups
void quadraticSplit(Node *currNode, rTree *tree)
{
    blue("[Entering Split]\n");

    int seed1, seed2;

    // pick two entries to be the first elements of the groups
    pickSeeds(currNode, &seed1, &seed2);
    // printf("%d,%d\n",seed1,seed2);
    // create two new nodes to be the groups
    Node *group1 = createNode(NULL, currNode, tree);
    Node *group2 = createNode(NULL, currNode, tree);

    // if the current node is a leaf node, set the leaf flag of the groups
    if (currNode->isLeaf == true)
    {
        group1->isLeaf = true;
        group2->isLeaf = true;
        currNode->isLeaf = false;
    }
    // else set the leaf flag of the groups to false
    else
    {
        group1->isLeaf = false;
        group2->isLeaf = false;
    }

    // add the seeds to respective groups

    group1->noOfEntries = 1;
    group2->noOfEntries = 1;

    group1->entries[0] = currNode->entries[seed1];
    group2->entries[0] = currNode->entries[seed2];

    // remove the seeds from the current node
    currNode->entries[seed2] = currNode->entries[currNode->noOfEntries - 1];
    currNode->noOfEntries--;

    currNode->entries[seed1] = currNode->entries[currNode->noOfEntries - 1];
    currNode->noOfEntries--;

    // while the current node is not empty
    while (currNode->noOfEntries > 0)
    {
        // if one of the groups has maxChildren entries, add all the remaining entries to the other group
        if (group2->noOfEntries+currNode->noOfEntries == tree->minChildren)
        {
            for (int i = 0; i < currNode->noOfEntries; i++)
            {
                group2->entries[group2->noOfEntries] = currNode->entries[i];
                group2->noOfEntries++;
            }

            currNode->noOfEntries = 0;
            break;
        }
        else if (group1->noOfEntries+currNode->noOfEntries == tree->minChildren)
        {
            for (int i = 0; i < currNode->noOfEntries; i++)
            {
                group1->entries[group1->noOfEntries] = currNode->entries[i];
                group1->noOfEntries++;
            }

            currNode->noOfEntries = 0;
            break;
        }
        else
        {
            // pick the next entry to be added to a group, and the group to which it should be added
            bool group_flag;
            int index = pickNext(currNode, group1->entries[0], group2->entries[0], &group_flag);

            // add to group 1
            if (group_flag)
            {
                group1->entries[group1->noOfEntries] = currNode->entries[index];
                group1->noOfEntries++;
            }

            // add to group 2
            else
            {
                group2->entries[group2->noOfEntries] = currNode->entries[index];
                group2->noOfEntries++;
            }

            // remove the entry from the current node
            currNode->entries[index] = currNode->entries[currNode->noOfEntries - 1];
            currNode->noOfEntries--;
        }
    }
    for(int i=0;i<group1->noOfEntries;i++)
    {
        if(group1->entries[i]->childNode!=NULL)
        group1->entries[i]->childNode->parent=group1;
    }
    for(int i=0;i<group2->noOfEntries;i++)
    {
        if(group2->entries[i]->childNode!=NULL)
        group2->entries[i]->childNode->parent=group2;
    }
    // if the current node is the root node, create a new root node and add the groups as its children
    if (currNode->parent == NULL)
    {
        currNode->noOfEntries = 2;
        currNode->entries[0] = createEntry(findMBR(group1), group1);

        group1->parentEntry = currNode->entries[0];
        currNode->entries[1] = createEntry(findMBR(group2), group2);

        group2->parentEntry = currNode->entries[1];
        group1->parent = currNode;
        group2->parent = currNode;

        tree->root = currNode;
        currNode->isLeaf = false;
    }

    // else add the groups as entries to the parent node and remove the current node.
    else
    {
        currNode->parentEntry->childNode = group1;
        currNode->parentEntry->rectangle = findMBR(group1);

        currNode->parent->entries[currNode->parent->noOfEntries] = createEntry(findMBR(group2), group2);
        currNode->parent->noOfEntries++;

        group1->parent = currNode->parent;
        group1->parentEntry = currNode->parentEntry;

        group2->parentEntry = currNode->parent->entries[currNode->parent->noOfEntries - 1];
        group2->parent = currNode->parent;

        tree->curr_no_of_nodes--;
    }

    blue("[Exiting Split]\n");
}

// function to adjust the tree after insertion
void adjustTree(Node *currNode, rTree *tree)
{
    blue("[Entering Adjust]\n");

    if (currNode == NULL)
    {
        return;
    }

    // if the current node is the root node, set it as the root of the tree and return
    if (currNode->parent == NULL)
    {
        tree->root = currNode;
        return;
    }

    // else adjust the MBR of the parent node and check if it needs to be split
    Node *parent = currNode->parent;

    // update the MBR of the parent node
    MBR *newVal = findMBR(currNode);
    if (newVal)
        currNode->parentEntry->rectangle = newVal;

    // if the parent node needs to be split, split it and adjust the tree
    if (parent->noOfEntries > tree->maxChildren)
    {
        
        quadraticSplit(parent, tree);
        if(parent!=tree->root)
        adjustTree(parent, tree);
        if(parent!=tree->root){free(parent);}
    }
    // else adjust the tree
    else
    {
        adjustTree(parent, tree);
    }

    blue("[Exiting Adjust]\n");
}

// insert a rectangle into the tree
void insert(rTree *tree, int minX, int maxX, int minY, int maxY)
{
    blue("[Entering Insert]\n");

    // create a rectangle and an entry for the rectangle
    MBR *rect = createMBR(minX, maxX, minY, maxY);
    Entry *entry = createEntry(rect, NULL);

    // choose the leaf node to which the entry should be added
    Node *currNode = chooseLeaf(tree->root, rect);

    // add the entry to the leaf node
    currNode->entries[currNode->noOfEntries] = entry;
    currNode->noOfEntries++;

    // if the leaf node needs to be split, split it
    if (currNode->noOfEntries > tree->maxChildren)
    {
        quadraticSplit(currNode, tree);
        adjustTree(currNode, tree);
        if(currNode->parent!=NULL){free(currNode);}
    }
    else
    {
    adjustTree(currNode, tree);
    }

    blue("[Exiting Insert]\n");
}
