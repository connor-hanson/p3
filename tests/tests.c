#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../parser.h"
#include "../graph.h"

#include <unistd.h>
#include <stdio.h>
#include <limits.h>

int testGetMakeFile() {
    return 0;
}

// Trty to put as much unusual input into this file as possible
int testRunParser() {
    FILE *testFile = fopen("tests/test.txt", "r");
    if (testFile == NULL) {
        char cwd[PATH_MAX];
        getcwd(cwd, sizeof(cwd));
        printf("%s\n", cwd);
    }

    // first with no target
    runParser(testFile);
    fclose(testFile);
    return 1;
}

// check graph.c state variables
/*int testInitGraph() {
    if (initGraph() == 0) {
        return -1;
    } else return 1;
}*/

int testAddGraphNode() {
    //initGraph();
    GraphNode *nodeA = addNode("TestA");
    GraphNode *nodeB = addNode("TestA");
    if (nodeA == NULL) {
        return -1;
    } else if (strcmp(nodeA->name, "TestA") != 0) {
        return -1;
    }
    if (nodeB == NULL) {
        return -1;
    } else if (strcmp(nodeB->name, "TestA") != 0) {
        return -1;
    } else if (nodeA != nodeB) {
    	return -1;
    }

    return 1;
}

int testAddGraphDep() {
    //initGraph();
    GraphNode *nodeA = addNode("TestA");
    if (addNodeDep(nodeA, "depA") != 1) {
        return -1;
    }
    //printf("A added succ\n");
    if (addNodeDep(nodeA, "depB") != 1) {
        return -1;
    }
    //printf("B added succ\n");
    if (addNodeDep(nodeA, "depC") != 1) {
        return -1;
    }
    //printf("C added succ\n");
    // should also check that depA, depB, depC are all nodes as well
    // try weird dependency names too

    return 1;
}

int testAddGraphCmd() {
    //initGraph();
    GraphNode *nodeA = addNode("TestA");
    if (addNodeCmd(nodeA, "stupid cmd 1") != 1) {
        return -1;
    }

    if (addNodeCmd(nodeA, "stupid\ncmd\n1") != 1) {
        return -1;
    }

    // add as much weird input as you can think of
    // TODO: test what happens when we reach command capacity

    return 1;
}

int testFreeGraphMem() {
    return 0;
}

int testExecNode() {
    return 0;
}

int main() {
    printf("starting tests\n");
    /*if (testInitGraph() != 1) {
        printf("Failed to correctly set graph state variables. \nTest Failed.\n");
        exit(0);
    }*/

    if (testAddGraphNode() != 1) {
        printf("Failed to correctly add a graph node. \nTests failed.\n");
        exit(0);
    }

    if (testAddGraphDep() != 1) {
        printf("Failed to correctly add graph dependencies. \nTests Failed.\n");
        exit(0);
    }

    if (testAddGraphCmd() != 1) {
        printf("Failed to correctly add commands to graph nodes. \nTests Failed.\n");
        exit(0);
    }

    // if (testFreeGraphMem() != 1) {
    //     printf("Failed to correctly free graph memory. \nTest Failed.\n");
    //     exit(0);
    // }

    // requires graph to work
    if (testRunParser() != 1) {
        printf("Failed to correctly parse makefile input. \nTest Failed. \n");
        exit(0);
    }

    printf("testing finished\n");
    return 0;
}
