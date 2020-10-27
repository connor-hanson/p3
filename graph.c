#include <stdlib.h>
#include <string.h>
#include "graph.h"

graphNode **graph;
int numNodes, graphSize;

// initialize graph, return 1 if successful, 0 otherwise
int initGraph(){
	int execStat = 0;
	if(graph = malloc(sizeof(*graphNode)){
		numNodes = 0;
		graphSize = 1;
		execStat = 1;
	}
	return execStat;
}
/**
 * @param:
 *		tar - target name
 *		dep - all dependencies
 * return 1 if successful, 0 otherwise
 */
int addNode(char* tar, char* dep, char* com){
	int execStat = 0;
	
	int tarSize = 0;
	while(*(tar+tarSize) != '\0'){
		tarSize++;
	}
	
	int depSize = 0;
	while(*(dep+depSize) != '\0'){
		depSize++;
	}
	
	int comSize = 0;
	while(*(com+comSize) != '\0'){
		comSize++;
	}
	
	graphNode *newNode;
	if(newNode = malloc(sizeof(graphNode))){
		if(newNode->target = malloc(tarSize*sizeof(char))){
			if(newNode->dependencies = malloc(depSize*sizeof(char))){
				if(newNode->commands = malloc(comSize*sizeof(char))){
					newNode->target = tar;
					newNode->dependencies = dep;
					newNode->commands = com;
					graph[numNodes] = newNode;
					numNodes++;
					if(graph = realloc(graph, (numNodes+1)*sizeof(*graphNode))){
						graphSize++;
						execStat = 1;
					}
				}
			}
		}
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
	graphNode *node = graph[nodeIndex];
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
