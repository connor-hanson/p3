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
    } else {
        printf("ok\n");
    }

    // first with no target
    runParser(testFile, NULL);
    fclose(testFile);
    return 0;
}

// check graph.c state variables
int testInitGraph() {
    if (initGraph() == 0) {
        return -1;
    } else return 1;
}

int testAddGraphNode() {
    initGraph();
    GraphNode *nodeA = addNode("TestA");
    if (nodeA == NULL) {
        return -1;
    } else if (strcmp(nodeA->target, "TestA") != 0) {
        return -1;
    }

    return 1;
}

int testAddGraphDep() {
    initGraph();
    GraphNode *nodeA = addNode("TestA");
    if (addNodeDep(nodeA, "depA") != 1) {
        return -1;
    }

    if (addNodeDep(nodeA, "depB") != 1) {
        return -1;
    }

    if (addNodeDep(nodeA, "depC") != 1) {
        return -1;
    }

    // should also check that depA, depB, depC are all nodes as well

    return 1;
}

int testAddGraphCmd() {
    return 0;
}

int testFreeGraphMem() {
    return 0;
}

int main() {
    printf("starting tests\n");
    if (testInitGraph() != 1) {
        printf("Failed to correctly set graph state variables. Test Failed.\n");
        exit(0);
    }

    if (testAddGraphNode() != 1) {
        printf("Failed to correctly add a graph node. Tests failed.\n");
        exit(0);
    }

    if (testAddGraphDep() != 1) {
        printf("Failed to correctly add graph dependencies. Tests Failed.\n");
        exit(0);
    }

    if (testAddGraphCmd() != 1) {
        printf("Failed to correctly add commands to graph nodes. Tests Failed.\n");
        exit(0);
    }

    if (testFreeGraphMem() != 1) {
        exit(0);
    }

    // requires graph to work
    if (testRunParser() != 1) {
        printf("Failed to correctly parse makefile input. Test Failed. \n");
        exit(0);
    }

    printf("testing finished\n");
    return 0;
}