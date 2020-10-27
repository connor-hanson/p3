#include <stdlib.h>
#include <string.h>
#include "graph.h"

GraphNode **graph;
int numNodes;
int graphSize;

// initialize graph, return 1 if successful, 0 otherwise
int initGraph() {
	if ((graph = malloc(sizeof(GraphNode*))) != NULL) {
		numNodes = 0;
		graphSize = 1;
		return 1;
	} else {
		return 0;
	}
}
/**
 * @param:
 *		tar - target name
 *		dep - all dependencies
 * return 1 if successful, 0 otherwise
 */
int addNode(char* tar) {
	int execStat = 0;
	
	int tarSize = 0;
	while(*(tar+tarSize) != '\0'){
		tarSize++;
	}
	
	/*
	int depSize = 0;
	while(*(dep+depSize) != '\0'){
		depSize++;
	}
	
	// Parsing commands happens in a different step than adding the initial node. this might not work 
	int comSize = 0;
	while(*(com+comSize) != '\0'){
		comSize++;
	}*/
	
	GraphNode *newNode = malloc(sizeof(GraphNode));
	if (newNode != NULL) {
		newNode->target = malloc(tarSize*sizeof(char));
		if (newNode->target != NULL) {
			newNode->target = tar;
			graph[numNodes] = newNode;
			numNodes++;
			// FIXME: realloc's every time a node is added. Maybe make *2 when needs resizing to 
			// reduce the amount of realloc operations
			if ((graph = realloc(graph, (numNodes+1)*sizeof(GraphNode))) != NULL){
				graphSize++;
				execStat = 1;
			}
		}
	}


	// if((newNode = malloc(sizeof(GraphNode))) != NULL){
	// 	if((newNode->target = malloc(tarSize*sizeof(char))) != NULL){
	// 		if((newNode->dependencies = malloc(depSize*sizeof(char))) != NULL){
	// 			if((newNode->commands = malloc(comSize*sizeof(char))) != NULL){
	// 				newNode->target = tar;
	// 				newNode->dependencies = dep;
	// 				newNode->commands = com;
	// 				graph[numNodes] = newNode;
	// 				numNodes++;
	// 				if((graph = realloc(graph, (numNodes+1)*sizeof(GraphNode))) != NULL){
	// 					graphSize++;
	// 					execStat = 1;
	// 				}
	// 			}
	// 		}
	// 	}
	// }
	
	return execStat;
}

int addNodeDep(char* node, char* dep) {
	return -1;
}

int addNodeCmd(char* node, char* cmd) {
	return -1;
}

/**
 * @param:
 *		tar - target name
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
