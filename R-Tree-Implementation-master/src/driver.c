#include "rTrees.h"

int main()
{
    rTree *tree = createRtree(2, 4);
    // read the input from the file
    FILE *fp = fopen("data/complete_data.txt", "r");
    while(!feof(fp))
    {
        int x1, y1;
        fscanf(fp, "%d %d", &x1, &y1);
        point pt={x1,y1};
        insertPoint(tree, pt);
    }
    fclose(fp);
    printf("Number of nodes in the tree: %d\n", tree->curr_no_of_nodes);
    printf("Number of nodes created: %d\n", tree->no_of_nodes);
    // preOrderTraversal(tree);
    return 0;
    
}