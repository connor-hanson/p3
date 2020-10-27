#ifndef GRAPH_H
#define GRAPH_H

typedef struct {
	char *target;
	char *dependencies;
	char *commands;
} GraphNode;

int initGraph();
int addNode(char* tar);
int removeNode(char* tar);
int addNodeDep(char* node, char* dep);
int addNodeCmd(char* node, char* cmd);
int executeNodes(char* tar);
char **getDependencies(char* dep);

#endif
