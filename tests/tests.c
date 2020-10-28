#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../parser.h"
#include "../graph.h"

#include <unistd.h>
#include <stdio.h>
#include <limits.h>

int testGetMakeFile() {
    return 0;
}

// Trty to put as much unusual input into this file as possible
int testRunParser() {
    FILE *testFile = fopen("tests/test.txt", "r");
    if (testFile == NULL) {
        char cwd[PATH_MAX];
        getcwd(cwd, sizeof(cwd));
        printf("%s\n", cwd);
    } else {
        printf("ok\n");
    }

    // first with no target
    runParser(testFile, NULL);
    fclose(testFile);
    return 0;
}

int main() {
    printf("starting tests\n");
    testRunParser();
    printf("testing finished\n");
    return 0;
}