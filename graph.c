#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include "graph.h"
#include "execution.h"

GraphNode *graphRoot; // First node to be added to the graph
int numNodes = 0;
int visitedBool = 1; // flips each time search is used

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
 * Add node {name} to graph. 
 * 
 * @params: 
 * 		name: node to be added
 * 
 * @return:
 * 		Pointer to node if successful
 * 		NULL if error occurs (node exists, malloc error)
 */
GraphNode *addNode(char* name) {
	// check if node exists
	GraphNode *nodeExists = getNode(graphRoot, name);
	if(nodeExists != NULL) { // node already in graph, return reference
		return nodeExists;
	}

	GraphNode *newNode = malloc(sizeof(GraphNode));
	if (newNode != NULL) {
		if (numNodes == 0) { // init graph
			graphRoot = newNode;
		}
		int nameSize = (int)strlen(name)+1;
		newNode->name = malloc(nameSize*sizeof(char));

		// init important node fields, inc node count
		if (newNode->name != NULL) {
			strcpy(newNode->name, name);
			newNode->name[nameSize-1] = '\0';
			//graph[numNodes] = newNode;
			numNodes++;
			newNode->visited = 0;
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
		return NULL;
	}
	//printf("added node %s\n", newNode->name);
	return newNode;
}

/**
 * Add node dependency {dep} to parent node {node}. node -> dep
 * Create node for dependency if it does not yet exist
 * 
 * @params:
 * 		node: Node ptr to add depency to
 * 		dep: String naming the dependency to add
 * 
 * @returns: 
 * 		1: {node} valid and dependency successfully added
 * 		0: Error
 */ 
int addNodeDep(GraphNode *node, char *dep) {
	int execStat = 0;
	GraphNode *depNode = addNode(dep); //create/get node for dep

	if (node == NULL) {
		return 0;
	}
	
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
			//printf("went in here\n");
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
	//printf("\tadded node dependency %s to node %s\n", depNode->name, node->name);
	return execStat;
}

/**
 * Add command string {cmd} to {node}
 * 
 * @params: 
 * 		node: node pointer to add command to
 * 		cmd: string containing command line to be parsed and executed later
 * 
 * @return:
 * 		0 - Some error adding command to node
 * 		1 - Command correctly added to node
 */ 
int addNodeCmd(GraphNode *node, char *cmd) {
	int execStat = 0;
	int cmdLen = (int)strlen(cmd)+1;
	char *entry = malloc(cmdLen*sizeof(char));

	if (entry == NULL){
		printf("Error allocating memory to command string");
		return 0;
	}
	strcpy(entry, cmd);

	entry[cmdLen-1] = '\0';
	if (node->commands == NULL) { //if no command array
		node->commands = malloc(10 * sizeof(char*));
		if (node->commands != NULL) { //init command array
			node->cmdSize = 10;
			node->numCmd = 1;
			node->commands[0] = entry;
			execStat = 1;
		} else {
			printf("Error allocating memory to command array\n");
			return 0;
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
		//execStat = 1;
		return 1;
	}
	//printf("added: %s\n",node->commands[node->numCmd-1]);
	//printf("\tadded command %s to node %s\n", node->commands[node->numCmd-1], node->name);
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

/**
 * Traverse Graph (DFS?) & execute commands on each node
 * 
 * @params
 * 		root: root node for traversal. Changes depending on whether user specifies build spec
 * 			  *graphRoot if none specified
 * 		visitedNodes: string containing space separated names of nodes which have been visited
 * 
 * @return
 * 		1: Good
 * 		0: Failure (cycle, file missing)
 */ 
void executeNode(GraphNode *root, char *visitedNodes) {

	// nodes have been visited, and one of the names nodes is the same as the current root -> error
	if (visitedNodes != NULL && strstr(visitedNodes, root->name) != NULL) {
		fprintf(stderr, "Error: dependency cycle in %s\n", visitedNodes);
		exit(0);
	}

	root->visited = 1;

	char *tempStack;
	int depsModded = 0;

	//dumpNode(root);

	// loop through current node's dependency list
	for (int i = 0; i < root->numDep; i++) {
		int visited = root->dependencies[i]->visited; //dont visit if already done
		if (!visited) {
			if (visitedNodes != NULL) {
				int stackLength = (int)strlen(visitedNodes)+1;
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
 
		//int shouldCompile = childModded(root, root->dependencies[i]);
		// check if build times require re-compilation
		if (childModded(root, root->dependencies[i])) {
			depsModded = 1;
		}
	}
	
	// if root has dependencies and they require re-compilation
	if (root->numDep > 0 && depsModded) {
		//if (depsModded) {
		printf("execing %s\n", root->name);
		executeCmd(root->commands, root->numCmd);
		//}
	}
	
	int nameLength = (int)strlen(root->name)+1;
	FILE *sourceFile;
	if (root->name[nameLength-2] == '\n') {
		char tempName[nameLength-1];
		memcpy(tempName, root->name, nameLength-2);
		tempName[nameLength-2] = '\0';
		printf("opening: %s\n", tempName);
		sourceFile = fopen(tempName, "r");
	}
	else {
		sourceFile = fopen(root->name, "r");
	}
	if (sourceFile == NULL) {
		fprintf(stderr, "Error: %s not found\n", root->name);
		exit(0);
	}
	fclose(sourceFile);
}

/**
 * (Recursively) Get node {name} using {startNode} as base Node. 
 * @params: 
 * 		startNode: node to start search from
 * 		name: search for node with name {name}
 * @return: 
 * 		result: Pointer to node {name} if found
 * 		Null: No matching node found or invalid params
 */
GraphNode *getNodeHelper(GraphNode *startNode, char *name){

	if (startNode == NULL) { // base case, bad param
		printf("No graph root parameter provided for search\n");
		return NULL;
	}

	if (strcmp(startNode->name, name) == 0) { // base case, name found
		return startNode;
	}

	startNode->visited = visitedBool; // mark visited

	for (int i = 0; i < startNode->numDep; ++i) {
		// recursive case, visit all dependent nodes
		if (startNode->dependencies[i]->visited != visitedBool) {
			GraphNode *res = getNodeHelper(startNode->dependencies[i], name);
			if (res != NULL) {
				return res;
			}
		}
	}

	return NULL; // not found
}

// helper function. just switch up flags
GraphNode *getNode(GraphNode *startNode, char *name) {
	visitedBool *= -1;
	return getNodeHelper(startNode, name);
}

/**
 * Recursively free graph starting from node {root}. DFS
 */
void freeNodeHelper(GraphNode *root) {
	if (root == NULL) { // base case, bad param
		printf("No graph root parameter provided for search\n");
		return;
	}

	root->visited = visitedBool; // mark visited

	for (int i = 0; i < root->numDep; ++i) {
		// recursive case, visit all dependent nodes
		if (root->dependencies[i] != NULL && root->dependencies[i]->visited != visitedBool) {
			freeNodeHelper(root->dependencies[i]);
		}
	}

	// free each of the malloc'd pointers inside the struct, set all vals to NULL/0
	free(root->name);
	root->name = NULL;

	free(root->dependencies);
	root->dependencies = NULL;

	for (int i = 0; i < root->numCmd; ++i) {
		free(root->commands[i]);
		root->commands[i] = NULL;
	}
	free(root->commands);
	root->commands = NULL;

	root->depSize = 0;
	root->numDep = 0;
	root->cmdSize = 0;
	root->numCmd = 0;
	root->visited = 0;

	free(root);
	root = NULL;
	numNodes--;
}

// helper method. param checks and flip visited flag
void freeNode(GraphNode *root) {
	visitedBool *= -1;
	if (root == NULL) {
		printf("Can't free null parameter");
	} else {
		freeNodeHelper(root);
		graphRoot = root;
	}
	
}

/**
 * Check if {parent} is older than {child}. If so, return 1 - indicating that {parent} needs to be recompiled
 * 
 * @params: 
 * 		parent: should be older
 * 		child: should be newer
 * 
 * @return: 
 * 		1: recompile parent 
 * 		0: good
 * 		-1: Error
 */ 
int childModded(GraphNode *parent, GraphNode *child){
	if (parent != NULL && child != NULL) {
		// store data values in parent/childfile
		struct stat parentFile, childFile;

		// check if files exist?
		int parentSucceed = stat(parent->name, &parentFile);
		int childSucceed = stat(child->name, &childFile);

		// if parent file DNE, needs compilation anyway
		if (parentSucceed != 0) {
			return 1;
		}

		// if child file DNE, no dependency
		if (childSucceed != 0) {
			fprintf(stderr, "Error: %s not found for %s\n", child->name, parent->name);
			exit(0);
		}

		// if parent older than child, recompile
		if (parentFile.st_mtime > childFile.st_mtime) {
			return 1;
		}
		else { // no recompile
			return 0;
		}
	}

	else {
		fprintf(stderr, "Error: nodes not found\n");
		exit(0);
	}
}

/*char **getDependencies(char* dep){
	return NULL;
}*/

int getNumNodes() {
	return numNodes;
}

GraphNode* getGraphRoot() {
	return graphRoot;
}

