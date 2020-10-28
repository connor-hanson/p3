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
GraphNode *addNode(char* tar) {
	int nameSize = 0;
	while(*(name+nameSize) != '\0'){
		nameSize++;
	}
	
	GraphNode *newNode = malloc(sizeof(GraphNode));
	if (newNode != NULL) {
		newNode->target = malloc(tarSize*sizeof(char));
		if (newNode->target != NULL) {
			newNode->target = tar;
			graph[numNodes] = newNode;
			numNodes++;
			if(numNodes >= graphSize){
				if((graph = realloc(graph, (graphSize * 2)*sizeof(graphNode*))) != NULL){
					graphSize = graphSize*2;
				}
			}
		}
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
	int execStat = 0;
	int cmdSize = 0;
	while(*(cmd+cmdSize) != '\0'){
		cmdSize++;
	}
	if((node->commands = malloc(cmdSize*sizeof(char))) != NULL){
		node->commands = cmd;
		execStat = 1;
	}
	return execStat;
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
