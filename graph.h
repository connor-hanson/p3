#ifndef GRAPH_H
#define GRAPH_H

typedef struct {
	char *target;
	char *dependencies;
	char *commands;
} GraphNode;

int initGraph();
GraphNode *addNode(char* tar);
int removeNode(char* tar);
int addNodeDep(GraphNode *node, char *dep);
int addNodeCmd(GraphNode *node, char *cmd);
int executeNodes(char* tar);
char **getDependencies(char* dep);

#endif
