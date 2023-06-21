#ifndef UTILS_H_
#define UTILS_H_
#include "rTrees.h"

#define min(a,b) (a)<(b)?(a):(b)
#define max(a,b) (a)>(b)?(a):(b)
#define INF 1e9
#define yellow(x...) {printf("\033[1;33m");printf(x);printf("\033[0m");}
#define red(x...) {printf("\033[1;31m");printf(x);printf("\033[0m");}
#define green(x...) {printf("\033[1;32m");printf(x);printf("\033[0m");}
// #define blue(x...) {printf("\033[1;34m");printf(x);printf("\033[0m");}
#define blue(x...)

void swap(int *a,int *b);
void printNode(Node *node);
void printEntry(Entry *Entry);
int findArea(MBR *rectangle);
MBR* findMBR(Node* currNode);
bool isOverlapping(MBR *rect1, MBR *rect2);
MBR* unionMBR(MBR *rect1, MBR *rect2);
void insertPolygon(rTree* tree,point* pts,int n);
void insertPoint(rTree* tree,point pt);
#endif

