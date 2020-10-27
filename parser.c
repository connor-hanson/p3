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
    // find location of target in file
    char str[4096]; // 4K. Including terminating null byte

    // TODO: add specific build specs later
    if (target != NULL) {

    }

    // line by line bby
    while (fgets(str, 4096, fp) != NULL) {
        char* lastTarget;
        // check first char not whitespace or invalid
        if (str[0] == '#') {
            continue; // line is a comment line
        }

        // MUST be a target line. Else is an error
        if (str[0] != '\t') {
            char *substr = str;
            substr = strtok(str, "#"); // split before/after possible comments
            // returns entire string if no #

            char* tok = strtok(substr, ":"); // find : in line. if none -> fail

            if (strcmp(tok, substr) == 0) { // ie no :
                printf("no ':' found in target line. terminating\n");
                exit(0);
            } else { // valid target. ':' exists
                // ensure no extra whitespace between target name and :
                for (size_t i = 0; i < strlen(tok); ++i) {
                    if (tok[i] == ' ') {
                        printf("invalid whitespace found in target line. terminating\n");
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

int main(int argc, char* argv[]) {
    FILE *make = getMakeFile(); // file
    int graphMade = initGraph();
    if (graphMade == 0) {
        printf("error creatig graph");
        exit(0);
    }

    // if no additional arg, first build spec found in file
    if (argc == 1) {
        runParser(make, NULL);
    }
    else if (argc == 2) {
        char *target = argv[1];
        runParser(make, target);
    }
    else {
        printf("Only one target can be specified. Terminating.");
        exit(0);
    }



    free(make);
    return 0;
}