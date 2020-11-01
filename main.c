#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

int main(int argc, char* argv[]) {
    FILE *make = getMakeFile(); // file
    /*int graphMade = initGraph();
    if (graphMade == 0) {
        printf("error creatig graph");
        exit(0);
    }*/

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
