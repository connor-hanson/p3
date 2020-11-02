#ifndef GRAPH_H
#define GRAPH_H

typedef struct GraphNode GraphNode;

struct GraphNode{
	char *name; // same as node name
	GraphNode **dependencies;
	char **commands;
	int depSize;
	int numDep;
	int cmdSize;
	int numCmd;
	int hasBeenVisited;
};

//int initGraph();
GraphNode *addNode(char* name);
//int removeNode(char* name);
int addNodeDep(GraphNode *node, char *dep);
int addNodeCmd(GraphNode *node, char *cmd);
void executeNode(GraphNode *root, char *visitedNodes);
GraphNode *getNode(GraphNode *startNode, char *name);
//char **getDependencies(char* dep); // what does this do again?
void freeNode(GraphNode *root);
int getNumNodes();
int childModded(GraphNode *parent, GraphNode *child);
GraphNode* getGraphRoot();

#endif
