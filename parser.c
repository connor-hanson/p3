#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "graph.h"

// makefile -> Makefile. If neither found, exit
FILE* getMakeFile() {
    FILE *make = fopen("makefile", "r");

    if (make == NULL) {
        make = fopen("Makefile", "r");
        if (make == NULL) {
            // error
            printf("No makefile found\n");
            exit(0);
        }
    }

    //printf("file opened\n");
    return make;
}

// optionally specify a target to start at
// Pass NULL as target if wanting to start from beginning of makefile
void runParser(FILE *fp) {
    if (fp == NULL) {
        printf("file pointer is NULL. terminating.\n");
        exit(0);
    }
    // find location of target in file
    char* str = malloc(4096 * sizeof(char)); // 4K. Including terminating null byte

    int lineNum = 0;

    GraphNode *masterNode = addNode("Master_Node");

    // line by line bby
    while (fgets(str, 4096, fp) != NULL) {
        //printf("%s%d%s\n", "lineNum: <", lineNum, ">");
        lineNum++;
        GraphNode *parentNode;

        if (str[0] == '#' || str[0] == '\0' || str[0] == '\n') {
            continue; // blank line or comment
        }


        // if not target or comment, must be a command
        if (str[0] == '\t') {
            char* tok = strtok(&str[1], "#");
            tok = strtok(tok, "\n");
            addNodeCmd(parentNode, tok);
        }

        // MUST be a target line. Else is an error
        if (str[0] != '\t') {
            char *substr = str;

            // TODO use strchr instead of strtok ya dummy
            substr = strtok(substr, "#"); // split before/after possible comments
            // returns entire string if no #
            //printf("%s\n", str);

            char* tarChar = strchr(substr, ':'); // messy but whatevs
            char* tok = strtok(substr, ":");

            if (tarChar == NULL) { 
                printf("%d%s%s\n", lineNum, ": <no ':' found in target line>", tok);
                exit(0);
            } else { // valid target. ':' exists
                // ensure no extra whitespace between target name and :
                for (size_t i = 0; i < strlen(tok); ++i) {
                    if (tok[i] == ' ') {
                        printf("%s%d%s\n", "<", lineNum, ">: invalid whitespace found in target line. terminating\n");
                        exit(0);
                    }
                }

                //parentNode = addNode(tok);
                //printf("%s%d%s\n", "lineNum: <", lineNum, ">");
                addNodeDep(masterNode, tok); // link every target to "master_node"
                //printf("Adding dep:...%s->%s...\n", masterNode->name, tok);
                parentNode = getNode(masterNode, tok);

                if (parentNode == NULL) {
                    printf("ERROR GETTING NODE\n");
                    exit(0);
                }

                // get the dependencies now. NULL in strtok to get next part of substr
                while (tok != NULL) {
                    tok = strtok(NULL, " ");
                    int valid = 0;

                    if (tok != NULL) {
                        // check that 'dependencies' are not just whitespace
                        for (size_t i = 0; i < strlen(tok); ++i) {
                            if (isspace(tok[i]) == 0) { // is not space
                                valid = 1;
                                break;
                            }
                        }

                        if (valid) {
                            //printf("Adding dep:...%s->%s...\n", parentNode->name, tok);
                            addNodeDep(parentNode, tok); // parent -> tok
                        }
                    }
                }  
            }
        }

    }
}
