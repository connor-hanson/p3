#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include "graph.h"
#include "execution.h"

GraphNode *graphRoot; // First node to be added to the graph -> master_node
int numNodes = 0;
int visitedBool = 1; // incs each time search is used

/**
 * Trims string (java style)
 *
 * @param:
 *	str - input string
 * @return: trimmed string
 */
char* trim(char* str) {
	int start = 0;
	int end = (int)strlen(str);

	while (start < end && (str[start] == ' ' || str[start] == '\t')) {
		start++;
	}

	while(end > start &&(str[end] == ' ' || str[end] == '\n' || str[end] == '\t' || str[end]=='\0')) {
		end--;
	}

	str[end+1] = '\0';
	char* newStr = realloc(str, end-start+2);

	return newStr;
}

/**
 * Add node {name} to graph. 
 * 
 * @params: 
 * 		name: node to be added
 * 
 * @return:
 * 		Pointer to node if successful
 *		Pointer to existing node
 * 		NULL if error occurs (malloc error)
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
			newNode->name = trim(newNode->name);
			newNode->name[nameSize-1] = '\0';
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
	//create copy of cmd string
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

	// command array exists
	else {
		//resize if needed
		if (node->numCmd >= node->cmdSize) {
			int currentSize = node->cmdSize;
			size_t newSize = currentSize * 2 * sizeof(char*);
			node->commands = realloc(node->commands, newSize);
			if (node->commands != NULL) {
				node->cmdSize = currentSize * 2;
			}
			else {
				printf("Error reallocating memory for command array\n");
				return 0;
			}
		}
		node->commands[node->numCmd] = entry;
		node->numCmd++;
		return 1;
	}
	return execStat;
}

/**
 * Traverse Graph & execute commands on each node
 * 
 * @params
 * 		root: root node for traversal. Changes depending on whether user specifies build spec
 * 			  *graphRoot if none specified
 * 		visitedNodes: string containing space separated names of nodes which have been visited
 * @return: exits on failure
 */
void executeNodeHelper(GraphNode *root) {
	// check for cycles
	if (root->visited == visitedBool) {
		printf("Error, cycle detected while executing graph: %s\n", root->name);
		exit(0);
	}

	root->visited = visitedBool;
	int depsModded = 0;

	// go thru deps, DFS recursive case
	for (int i = 0; i < root->numDep; ++i) {
		if (root->dependencies[i] != NULL && root->dependencies[i]->visited != visitedBool) {
			executeNodeHelper(root->dependencies[i]);
			// check if build times require re-compilation, ie child has been modified
			if (childModded(root, root->dependencies[i])) {
				depsModded = 1;
			}
		} else if (root->dependencies[i] == NULL) {
			printf("Error, null dependency found while executing graph");
			exit(0);
		}
	}

	// if root has dependencies and they require re-compilation
	if (root->numDep > 0 && depsModded) {
		executeCmd(root->commands, root->numCmd);
	} 
	// or if target without deps (eg 'clean') and has associated commands
	else if (root->numDep == 0 && root->numCmd > 0) {
		executeCmd(root->commands, root->numCmd);
	}
	//check compile was successful and/or file now exists
	if (root->numDep > 0 && strcmp(root->name, "Master_Node") != 0) {
		int nameLength = (int)strlen(root->name)+1;
		FILE *sourceFile;
		if (root->name[nameLength-2] == '\n') {
			char tempName[nameLength-1];
			memcpy(tempName, root->name, nameLength-2);
			tempName[nameLength-2] = '\0';
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
}

/**
 * Signals to execute commands on node traversal
 *
 * @param:
 *	root - pointer to root node
 */
void executeNode(GraphNode *root) {
	visitedBool++;
	if (root == NULL) {
		printf("Error: No rule to make target. Stop\n");
		exit(0);
	}

	executeNodeHelper(root);
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

/**
 * Helper function for node traversing
 */
GraphNode *getNode(GraphNode *startNode, char *name) {
	visitedBool++;
	return getNodeHelper(startNode, name);
}

/**
 * Recursively free graph starting from node {root}. DFS
 *
 * @param:
 *	root - pointer to root node
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
	// double free somewhere in here
	free(root->name);
	root->name = NULL;

	free(root->dependencies);
	root->dependencies = NULL;

	free(root->commands);
	root->commands=NULL;
}

/**
 * Free node memory
 *
 * @param:
 *	root - pointer to root node
 */
void freeNode(GraphNode *root) {
	visitedBool++;
	if (root == NULL) {
		printf("Can't free null parameter");
	} else {
		freeNodeHelper(root);
		graphRoot = root;
	}
	
}

/**
 * Check if {parent} is older than {child}. If not, return 1 - indicating that {parent} needs to be recompiled
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
			fprintf(stderr, "<childModded>Error: %s \tnot found for %s\n", child->name, parent->name);
			exit(0);
		}

		// if parent older than child, recompile (was >)
		if (parentFile.st_mtime < childFile.st_mtime) {
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

/**
 * @return: Number of nodes
 */
int getNumNodes() {
	return numNodes;
}

/**
 * @return: Get pointer to root node
 */
GraphNode* getGraphRoot() {
	return graphRoot;
}

