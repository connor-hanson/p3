#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

    printf("file opened\n");
    return make;
}

// return 1 if target is in makefile. else -1
int getTarget(FILE *fp, char* target) {
    return 0;
}

// optionally specify a target to start at
// Pass NULL as target if wanting to start from beginning of makefile
void runParser(FILE *fp, char* target) {
    if (fp == NULL) {
        printf("file pointer is NULL. terminating.\n");
        exit(0);
    }
    // find location of target in file
    char* str = malloc(4096 * sizeof(char)); // 4K. Including terminating null byte

    // TODO: add specific build specs later
    if (target != NULL) {

    }

    int lineNum = 0;

    // line by line bby
    while (fgets(str, 4096, fp) != NULL) {
        lineNum++;
        char* lastTarget;
        // check first char not whitespace or invalid
        if (str[0] == '#') {
            continue; // line is a comment line
        }

        // MUST be a target line. Else is an error
        if (str[0] != '\t') {
            char *substr = str;

            // TODO use strchr instead of strtok ya dummy
            substr = strtok(substr, "#"); // split before/after possible comments
            // returns entire string if no #
            printf("%s\n", str);

            char* tarChar = strchr(substr, ':'); // messy but whatevs
            char* tok = strtok(substr, ":");

            if (tarChar == NULL) { 
                printf("%d%s%s\n", lineNum, ": <no ':' found in target line>", substr);
                exit(0);
            } else { // valid target. ':' exists
                // ensure no extra whitespace between target name and :
                for (size_t i = 0; i < strlen(tok); ++i) {
                    if (tok[i] == ' ') {
                        printf("%s%d%s\n", "<", lineNum, ">: invalid whitespace found in target line. terminating\n");
                        exit(0);
                    }
                }

                addNode(tok);
                char* parentNode = tok;
                lastTarget = tok; // might be issue w memory

                // get the dependencies now. NULL in strtok to get next part of substr
                while (tok != NULL) {
                    tok = strtok(NULL, " ");
                    if (tok != NULL) {
                        addNodeDep(parentNode, tok); // parent -> tok
                    }
                }  
            }
        }

        // if not target or comment, must be a command
        if (str[0] == '\t') {
            char* tok = strtok(str, "#");
            addNodeCmd(lastTarget, tok);
        }

        // if none of these run, is blank. nothing happens
    }
}