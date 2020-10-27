#ifndef GRAPH_H
#define GRAPH_H

typedef struct {
	char *target;
	char *dependencies;
	char *commands;
} graphNode;

int initGraph();
int addNode(char* tar, char* dep, char* com);
int removeNode(char* tar);
int executeNodes(char* tar);
char **getDependencies(char* dep);

#endif
