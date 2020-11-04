#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include "graph.h"
#include "execution.h"

GraphNode *graphRoot; // First node to be added to the graph -> master_node
int numNodes = 0;
int visitedBool = 1; // incs each time search is used

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

// like java's trim()
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
	//printf("in: %s, out: %s,\n", str, newStr);

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
 * 		NULL if error occurs (node exists, malloc error)
 */
GraphNode *addNode(char* name) {
	// check if node exists
	//name = trim(name);
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
			//printf("Node added: %s.\n", newNode->name);
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
		//execStat = 1;
		return 1;
	}

	printf("Added command:%s.\n", node->commands[node->numCmd-1]);
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

// DFS
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
				printf("recompile,%s.\n", root->name);
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
	
	if (root->numDep > 0 && strcmp(root->name, "Master_Node") != 0) {
		int nameLength = (int)strlen(root->name)+1;
		FILE *sourceFile;
		if (root->name[nameLength-2] == '\n') {
			char tempName[nameLength-1];
			memcpy(tempName, root->name, nameLength-2);
			tempName[nameLength-2] = '\0';
			printf("opening: %s,\n", tempName);
			sourceFile = fopen(tempName, "r");
		}
		else {
			sourceFile = fopen(root->name, "r");
			printf("opening %s,\n", root->name);
		}

		if (sourceFile == NULL) {
			fprintf(stderr, "Error: %s not found\n", root->name);
			exit(0);
		}
		fclose(sourceFile);
	}
}

// switch flags and use main thang
void executeNode(GraphNode *root) {
	visitedBool++;
	if (root == NULL) {
		printf("Can't execute null dependency graph");
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
	visitedBool++;
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
	// double free somewhere in here
	free(root->name);
	root->name = NULL;

	free(root->dependencies);
	root->dependencies = NULL;

	// for (int i = 1; i < root->numCmd; ++i) {
	// 	free(root->commands[i]);
	// 	root->commands[i] = NULL;
	// }
	// free(root->commands);
	// root->commands = NULL;

	// root->depSize = 0;
	// root->numDep = 0;
	// root->cmdSize = 0;
	// root->numCmd = 0;
	// root->visited = 0;

	// free(root);
	// root = NULL;
	// numNodes--;
}

// helper method. param checks and flip visited flag
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

		// printf("parent:%s, child:%s.\n", parent->name, child->name);
		// printf("Diff:%ld.\n", parentFile.st_mtime-childFile.st_mtime);

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

/*char **getDependencies(char* dep){
	return NULL;
}*/

int getNumNodes() {
	return numNodes;
}

GraphNode* getGraphRoot() {
	return graphRoot;
}

