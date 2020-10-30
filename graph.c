#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "graph.h"

GraphNode **graph;
GraphNode *graphRoot; // First node to be added to the graph
int numNodes;
int graphSize;

// initialize graph, return 1 if successful, 0 otherwise
int initGraph() {
	if ((graph = malloc(sizeof(GraphNode*))) != NULL) {
		numNodes = 0;
		graphSize = 10;
		return 1;
	} else {
		return 0;
	}
}
/**
 * @param:
 *		tar - target name
 *		dep - all dependencies
 * return node if successful, NULL otherwise
 */
GraphNode *addNode(char* name) {
	int nameSize = 0;
	while(*(name+nameSize) != '\0'){ // prolly wanna optimize?
		nameSize++;
	}
	
	GraphNode *newNode = malloc(sizeof(GraphNode));

	if (newNode != NULL) {
		if (numNodes == 0) { // init graph
			graphRoot = newNode;
		}

		// malloc commands array & associated fields
		newNode->commands = malloc(5 * sizeof(char*)); // room for 5 strings at first. Might not need to alloc?
		if (newNode->commands == NULL) {
			printf("Error allocating memory space for commands array.\n");
			return NULL;
		}
		newNode->numCommands = 0;
		newNode->commandCap = 5;

		// name allocation
		newNode->target = malloc(nameSize*sizeof(char));
		if (newNode->target != NULL) {
			newNode->target = name;
			numNodes++;
		} else {
			// malloc failed
			printf("Error allocating memory for new node\n");
			return NULL;
		}
	} else {
		printf("Error allocating memory for new Node\n");
		return NULL;
	}

	return newNode;
}

int addNodeDep(GraphNode *node, char *dep) {
	int execStat = 0;
	int depSize = 0;
	while(*(dep+depSize) != '\0'){
		depSize++;
	}
	if((node->dependencies = malloc(depSize*sizeof(char))) != NULL){
		node->dependencies = dep;
		execStat = 1;
	}
	return execStat;
}

int addNodeCmd(GraphNode *node, char *cmd) {
	//int execStat = 0;
	int cmdSize = 0;
	while(*(cmd+cmdSize) != '\0'){
		cmdSize++;
	}

	// allocate string at correct array spot
	(node->commands)[node->numCommands] = malloc(cmdSize * sizeof(char));
	if (((node->commands)[node->numCommands]) == NULL) {
		printf("Error allocating memory for command String");
		return -1;
	} else {
		(node->commands)[node->numCommands] = cmd;
		node->numCommands += 1;

		// if at cap-1, resize *2
		if ( node->numCommands == (node->commandCap - 1) ) {
			node->commands = realloc(node->commands, (node->commandCap * 2) * sizeof(char*));
			if (node->commands == NULL) {
				printf("Error reallocating memory for command string array.");
				return -1; // realloc failed
			} else {
				node->commandCap *= 2;
			}
		}

		return 1; // successful
	}

	// if((node->commands = malloc(cmdSize*sizeof(char))) != NULL){
	// 	node->commands = cmd;
	// 	execStat = 1;
	// }
	// return execStat;
}

/**
 * @param:
 *		tar - target name
 * REDO
 */
int removeNode(char* tar){
	int execStat = 0;
	int nodeIndex = 0;
	while(strcmp(tar, graph[nodeIndex]->target) != 0){
		nodeIndex++;
	}
	GraphNode *node = graph[nodeIndex];
	for(int i = nodeIndex; i < graphSize-1; i++){
		graph[i] = graph[i+1];
	}
	free(node);
	execStat = 1;
	return execStat;
}
int executeNodes(char* tar){
	return 0;
}
char **getDependencies(char* dep){
	return NULL;
}

int getNumNodes() {
	return numNodes;
}

GraphNode* getGraphRoot() {
	return graphRoot;
}

