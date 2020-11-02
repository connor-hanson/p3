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
    //FILE *testFile = fopen("tests/test.txt", "r");
    FILE *testFile = fopen("testmakefile", "r");
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
    // should point to same reference
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
        printf("Nodes should be the same reference!");
    	return -1;
    }

    freeNode(nodeA);

    if (getNumNodes() != 0) {
        printf("Error freeing memory. NumNodes in graph: %d\n", getNumNodes());
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

    freeNode(nodeA);
    if (getNumNodes() != 0) {
        printf("Error freeing memory\n");
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

    freeNode(nodeA);
    if (getNumNodes() != 0) {
        printf("Error freeing memory\n");
        return -1;
    }

    // add as much weird input as you can think of
    // TODO: test what happens when we reach command capacity

    return 1;
}

// test w no cycles and new nodes
int testGetNode() {
    // basic search cases; 1->2->3
    GraphNode *node1 = addNode("node1");
    addNodeDep(node1, "node2");
    GraphNode *node2 = getNode(node1, "node2");
    if (node2 == NULL) {
        printf("node 2 be null wtf\n");
        return -1;
    }
    if (strcmp(node2->name, "node2") != 0)  {
        printf("node 2 be name wrong\n");
        return -1;
    }

    addNodeDep(node2, "node3");
    GraphNode *node3 = getNode(node1, "node3");
    if (node3 == NULL || strcmp(node3->name, "node3") != 0)  {
        printf("error getting node3");
        return -1;
    }

    addNodeDep(node3, "node4");
    GraphNode *node4 = getNode(node1, "node4");
    if (node4 == NULL)  {
        printf("error getting node4, is null\n");
        return -1;
    } else if (strcmp(node4->name, "node4") != 0) {
        printf("error getting node4, wrong name\n");
        return -1;
    }

    addNodeDep(node2, "node5");
    GraphNode *node5 = getNode(node1, "node5");
    if (node5 == NULL || strcmp(node5->name, "node5") != 0)  {
        printf("error getting node5");
        return -1;
    }

    addNodeDep(node5, "node6");
    GraphNode *node6 = getNode(node1, "node6");
    if (node6 == NULL || strcmp(node6->name, "node6") != 0)  {
        printf("error getting node6");
        return -1;
    }
    
    freeNode(getGraphRoot());
    if (getNumNodes() != 0) {
        printf("Error freeing memory. %d nodes left in memory\n", getNumNodes());
        return -1;
    }

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

    // printf("testing addNode...\n");
    // if (testAddGraphNode() != 1) {
    //     printf("Failed to correctly add a graph node. \nTests failed.\n");
    //     exit(0);
    // }

    printf("testing getNode...\n");
    if (testGetNode() != 1) {
        printf("Failed getting nodes. \nTest failed\n");
        exit(0);
    }

    printf("testing addNodeDep...\n");
    if (testAddGraphDep() != 1) {
        printf("Failed to correctly add graph dependencies. \nTests Failed.\n");
        exit(0);
    }

    printf("testing addNodeCmd...\n");
    if (testAddGraphCmd() != 1) {
        printf("Failed to correctly add commands to graph nodes. \nTests Failed.\n");
        exit(0);
    }

    // printf("testing childModded...\n");
    // if (testChildModded() != 1) {
    //     printf("Failed to correctly asses build times.\nTest failed.\n");
    //     exit(0);
    // }


    // if (testFreeGraphMem() != 1) {
    //     printf("Failed to correctly free graph memory. \nTest Failed.\n");
    //     exit(0);
    // }

    // requires graph to work
    printf("testing parser...\n");
    if (testRunParser() != 1) {
        printf("Failed to correctly parse makefile input. \nTest Failed. \n");
        exit(0);
    }

    printf("testing finished\n");
    return 0;
}
