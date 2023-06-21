#include "rTrees.h"
#include "utils.h"

// Swaps two integer variables.
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Print the details of an entry
void printEntry(Entry *Entry)
{
    MBR *MBR = Entry->rectangle;

    if (MBR == NULL)
    {
        return;
    }

    green("\t\tTop Right -> %d,%d\n", MBR->pairX.maxLimit, MBR->pairY.maxLimit);
    green("\t\tBottom Left -> %d,%d\n\n", MBR->pairX.minLimit, MBR->pairY.minLimit);

    return;
}

// Print the details of an entry, plainly
void printEntryPlain(Entry *Entry)
{
    MBR *MBR = Entry->rectangle;

    if (MBR == NULL)
    {
        return;
    }

    green("\t\t%d,%d;", MBR->pairX.maxLimit, MBR->pairY.maxLimit);
    green("\t%d,%d\n", MBR->pairX.minLimit, MBR->pairY.minLimit);

    return;
}

void printNode(Node *node)
{
    yellow("Entering Node %d\n", node->index);
    
    // print all the child nodes of the current node
    yellow("Child Nodes :- [ ");
    for(int i = 0; i<node->noOfEntries; i++)
    {
        if(node->entries[i]!=NULL && node->entries[i]->childNode != NULL)
        {
            yellow("%d,", node->entries[i]->childNode->index);
        }
    }
    yellow("]\n");
    
    // print the entries of the current node
    for (int i = 0; i < node->noOfEntries; i++)
    {
        green("\tEntry %d\n", i);
        printEntry(node->entries[i]);
    }
    yellow("Exiting Node %d\n", node->index);

}

// Find the area of a rectangle
int findArea(MBR *rectangle)
{
    int length = rectangle->pairX.maxLimit - rectangle->pairX.minLimit;
    int breadth = rectangle->pairY.maxLimit - rectangle->pairY.minLimit;

    return length * breadth;
}

// Find the MBR of a node given all its entries
MBR *findMBR(Node *currNode)
{
    // to skip the dummy node in the beginning
    if (currNode->noOfEntries == 0)
    {
        return NULL;
    }

    // initialize the min and max values
    int minx = currNode->entries[0]->rectangle->pairX.minLimit;
    int miny = currNode->entries[0]->rectangle->pairY.minLimit;
    int maxx = currNode->entries[0]->rectangle->pairX.maxLimit;
    int maxy = currNode->entries[0]->rectangle->pairY.maxLimit;

    // find the min and max values
    for (int i = 1; i < currNode->noOfEntries; i++)
    {
        minx = min(minx, currNode->entries[i]->rectangle->pairX.minLimit);
        miny = min(miny, currNode->entries[i]->rectangle->pairY.minLimit);
        maxx = max(maxx, currNode->entries[i]->rectangle->pairX.maxLimit);
        maxy = max(maxy, currNode->entries[i]->rectangle->pairY.maxLimit);
    }

    // create the MBR and return it
    return createMBR(minx, maxx, miny, maxy);
}

// checks if two rectangles are overlapping
bool isOverlapping(MBR *rect1, MBR *rect2)
{
    if (rect1->pairX.minLimit >= rect2->pairX.maxLimit || rect1->pairX.maxLimit <= rect2->pairX.minLimit)
        return false;

    if (rect1->pairY.minLimit >= rect2->pairY.maxLimit || rect1->pairY.maxLimit <= rect2->pairY.minLimit)
        return false;

    return true;
}

// finds the union of two rectangles
MBR *unionMBR(MBR *rect1, MBR *rect2)
{
    int minx = min(rect1->pairX.minLimit, rect2->pairX.minLimit);
    int miny = min(rect1->pairY.minLimit, rect2->pairY.minLimit);
    int maxx = max(rect1->pairX.maxLimit, rect2->pairX.maxLimit);
    int maxy = max(rect1->pairY.maxLimit, rect2->pairY.maxLimit);
    
    return createMBR(minx, maxx, miny, maxy);
}

void insertPolygon(rTree* tree,polygon pts,int n)
{
    int minx=pts[0].x,maxx=pts[0].x,miny=pts[0].y,maxy=pts[0].y;
    for(int i=1;i<n;i++)
    {
        minx=min(minx,pts[i].x);
        maxx=max(maxx,pts[i].x);
        miny=min(miny,pts[i].y);
        maxy=max(maxy,pts[i].y);
    }
    insert(tree,minx,maxx,miny,maxy);
}

void insertPoint(rTree* tree, point p)
{
    insert(tree,p.x,p.x,p.y,p.y);
}