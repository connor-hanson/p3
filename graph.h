#ifndef GRAPH_H
#define GRAPH_H

typedef struct {
	char *target; // same as node name
	char *dependencies;
	//char *commands;
	char **commands;
	int numCommands;
	int commandCap;
} GraphNode;

int initGraph();
GraphNode *addNode(char* tar);
int removeNode(char* tar);
int addNodeDep(GraphNode *node, char *dep);
int addNodeCmd(GraphNode *node, char *cmd);
int executeNodes(char* tar);
char **getDependencies(char* dep); // what does this do again?

int getNumNodes();
GraphNode* getGraphRoot();

#endif
