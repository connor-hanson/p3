#ifndef GRAPH_H
#define GRAPH_H

typedef struct {

} Graph;
typedef struct {

} Node;

Graph* createGraph();
int addNode(char* str);
Node *removeNode(char* str);
int checkCycles(char* root);
Node **getDependencies(char* str);
int addDependency(char* parent, char* depend);

#endif