#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include "graph.h"
#include "execution.h"

GraphNode *graphRoot; // First node to be added to the graph
int numNodes = 0;

// initialize graph, return 1 if successful, 0 otherwise
/*int initGraph() {
	if ((graphRoot = malloc(sizeof(GraphNode))) != NULL) {
		numNodes = 0;
		graphSize = 10;
		return 1;
	} else {
		return 0;
	}
}*/

/**
 * @param:
 *		name - node name
 * return pointer to node if successful, null otherwise
 */
GraphNode *addNode(char* name) {
	GraphNode *nodeExists = findNode(graphRoot, name);
	if(nodeExists != NULL){
		return nodeExists;
	}

	GraphNode *newNode = malloc(sizeof(GraphNode));
	if (newNode != NULL) {
		if (numNodes == 0) { // init graph
			graphRoot = newNode;
		}
		int nameSize = (int)strlen(name);
		newNode->name = malloc(nameSize*sizeof(char));
		if (newNode->name != NULL) {
			strcpy(newNode->name, name);
			//graph[numNodes] = newNode;
			numNodes++;
			newNode->hasBeenVisited = 0;
			newNode->numDep = 0;
			newNode->numCmd = 0;
		}
		else {
			fprintf(stderr, "Error: malloc node name\n");
			return NULL;
		}
	}
	else {
		fprintf(stderr, "Error: malloc node\n");
	}
	return newNode;
}

int addNodeDep(GraphNode *node, char *dep) {
	int execStat = 0;
	GraphNode *depNode = addNode(dep); //create node for dep
	
	if (node->dependencies == NULL) { //if no dep array
		node->dependencies = malloc(10*sizeof(GraphNode*));
		if (node->dependencies != NULL) { //init dep array
			node->depSize = 10;
			node->numDep = 1;
			node->dependencies[0] = depNode;
			execStat = 1;
		}
	}
	else {
		if (node->numDep >= node->depSize) {
			printf("went in here\n");
			int currentSize = node->depSize;
			size_t newSize = currentSize * 2 * sizeof(GraphNode*);
			node->dependencies = realloc(node->dependencies, newSize);
			if (node->dependencies != NULL) {
				node->depSize = currentSize * 2;
			}
			else {
				return 0;
			}
		}
		node->dependencies[node->numDep] = depNode;
		node->numDep++;
		execStat = 1;
	}
	return execStat;
}

int addNodeCmd(GraphNode *node, char *cmd) {
	int execStat = 0;
	int cmdLen = (int)strlen(cmd);
	char *entry = malloc(cmdLen*sizeof(char));
	if (entry == NULL){
		return 0;
	}
	strcpy(entry, cmd);
	
	if (node->commands == NULL) { //if no command array
		node->commands = malloc(10 * sizeof(char*));
		if (node->commands != NULL) { //init command array
			node->cmdSize = 10;
			node->numCmd = 1;
			node->commands[0] = entry;
			execStat = 1;
		}
	}
	else {
		if (node->numCmd >= node->cmdSize) {
			int currentSize = node->cmdSize;
			size_t newSize = currentSize * 2 * sizeof(char*);
			node->commands = realloc(node->commands, newSize);
			if (node->commands != NULL) {
				node->cmdSize = currentSize * 2;
			}
			else {
				return 0;
			}
		}
		node->commands[node->numCmd] = entry;
		node->numCmd++;
		execStat = 1;
	}
	return execStat;
}

/**
 * @param:
 *		name - target name
 * REDO
 */
/*int removeNode(char* name){
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
}*/

//return 0 if failure (cycle, file missing)
//		 1 otherwise
void executeNode(GraphNode *root, char *visitedNodes) {
	if (visitedNodes != NULL && strstr(visitedNodes, root->name) != NULL) {
		fprintf(stderr, "Error: dependency cycle in %s\n", visitedNodes);
		exit(0);
	}
	char *tempStack;
	int depsModded = 0;
	int i;
	for (i = 0; i < root->numDep; i++) {
		int visited = root->dependencies[i]->hasBeenVisited; //dont visit if already done
		if (!visited) {
			if (visitedNodes != NULL) {
				int stackLength = (int)strlen(visitedNodes);
				tempStack = malloc(stackLength * sizeof(char));
			}
			else {
				tempStack = malloc(sizeof(char));
			}
			if (tempStack != NULL) {
				if (visitedNodes != NULL) {
					strcpy(tempStack, visitedNodes);
				}
				else {
					strcpy(tempStack, "");
				}
				strncat(tempStack, root->name, strlen(root->name)); //add root to stack
				executeNode(root->dependencies[i], tempStack);
				/*if (modded == 0) { //make sure there's no cycles
					return 0;
				}*/
				free(tempStack);
			}
		}
		int shouldCompile = childModded(root, root->dependencies[i]);
		if (shouldCompile) {
			depsModded = 1;
		}
	}
	
	if (root->numDep > 0) {
		if (depsModded) {
			executeCmd(root->commands, root->numCmd);
		}
	}
	FILE *sourceFile = fopen(root->name, "r");
	if (sourceFile == NULL) {
		fprintf(stderr, "Error: %s not found\n", root->name);
		exit(0);
	}
	fclose(sourceFile);
}

GraphNode *findNode(GraphNode *startNode, char *name){
	if (startNode != NULL) {
		int compare = strcmp(startNode->name, name);
		if (compare == 0) {
			return startNode;
		}
		
		int i;
		for (i = 0; i < startNode->numDep; i++) {
			GraphNode *result = findNode(startNode->dependencies[i], name);
			if (result != NULL) {
				return result;
			}
		}
	}
	return NULL;
}

void freeNode(GraphNode *root) {
	int i;
	for (i = 0; i < root->numDep; i++) {
		if (root->dependencies[i]->name != NULL) {
			freeNode(root->dependencies[i]);
		}
	}
	free(root);
}

//return 1 if should compile
//		 0 if not
int childModded(GraphNode *parent, GraphNode *child){
	if (parent != NULL && child != NULL) {
		struct stat parentFile, childFile;
		int parentSucceed = stat(parent->name, &parentFile);
		int childSucceed = stat(child->name, &childFile);
		if (parentSucceed != 0) {
			return 1;
		}
		if (childSucceed != 0) {
			fprintf(stderr, "Error: %s not found for %s\n", child->name, parent->name);
			exit(0);
		}
		if (parentFile.st_mtime > childFile.st_mtime) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else {
		fprintf(stderr, "Error: nodes not found\n");
		exit(0);
	}
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

