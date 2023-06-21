
#include "utils.h"
#include "rTrees.h"

//Creates new MBR 
MBR *createMBR(int minX, int maxX, int minY, int maxY)
{
    MBR *rectangle = (MBR *)malloc(sizeof(MBR));
    rectangle->pairX.minLimit = minX;
    rectangle->pairX.maxLimit = maxX;
    rectangle->pairY.minLimit = minY;
    rectangle->pairY.maxLimit = maxY;
    return rectangle;
}

// Creates new node
Node *createNode(Entry *parentEntry, Node *parent, rTree *tree)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->noOfEntries = 0;
    node->isLeaf = 0;
    node->parentEntry = parentEntry;
    node->parent = parent;
    node->entries = (Entry **)malloc((tree->maxChildren+1) * (sizeof(Entry *)));
    node->index = tree->no_of_nodes++;
    tree->curr_no_of_nodes++;
    return node;
}

// Creates new entry
Entry *createEntry(MBR *rectangle, Node *child)
{
    Entry *entry = (Entry *)malloc(sizeof(Entry));
    entry->childNode = child;
    entry->rectangle = rectangle;
    return entry;
}

// Create new rTree
rTree *createRtree(int minchild, int maxchild)
{
    rTree *tree= (rTree *)malloc(sizeof(rTree));
    tree->minChildren = minchild;
    tree->maxChildren = maxchild;
    tree->start = (Entry *)malloc(sizeof(Entry));
    tree->start->rectangle = NULL;
    tree->no_of_nodes = 0;
    tree->curr_no_of_nodes = 0;
    tree->root = createNode(tree->start,NULL, tree);
    tree->root->isLeaf = 1;
    return tree;
}