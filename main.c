#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "graph.h"

int main(int argc, char* argv[]) {
    FILE *make = getMakeFile(); // file
    /*int graphMade = initGraph();
    if (graphMade == 0) {
        printf("error creatig graph");
        exit(0);
    }*/

    // if no additional arg, first build spec found in file
    runParser(make);
    GraphNode *root = getGraphRoot();
    
    if (argc > 1) {
        if (argc > 2) {
            printf("Can't specify more than one target. terminating\n");
            exit(0);
        } else {
            char* buildTarget = argv[1];
            printf("argv: %s\n", buildTarget);
            GraphNode *tar = getNode(root, buildTarget);
            if (tar == NULL) {
                printf("fuckme\n");
            }
            executeNode(tar);
        }
    } else {
        executeNode(root);
    }

    freeNode(root);
    
    // if (argc == 1) {
    //     runParser(make, NULL);
    // }
    // else if (argc == 2) {
    //     char *target = argv[1];
    //     runParser(make, target);
    // }
    // else {
    //     printf("Only one target can be specified. Terminating.");
    //     exit(0);
    // }

    fclose(make);
    return 0;
}
