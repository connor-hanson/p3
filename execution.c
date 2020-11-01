#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "execution.h"

void executeLine(char *line) {
	int lineLength = (int)strlen(line);
	char *tempLine1 = malloc(lineLength * sizeof(char));
	if (tempLine1 != NULL) {
		char *tempLine2 = malloc(lineLength * sizeof(char));
		if (tempLine2 != NULL) {
			strcpy(tempLine1, line);
			strcpy(tempLine2, line);
			char *separator = " ";
			char *arg = strtok(tempLine1, separator);
			arg = strtok(NULL, separator);
			int argNum = 0;
			while (arg != NULL) {
				argNum++;
				arg = strtok(NULL, separator);
			}
			char *args[argNum];
			char *cmd = strtok(tempLine2, separator);
			arg = strtok(NULL, separator);
			int i = 0;
			while(arg != NULL) {
				args[i] = arg;
				i++;
				arg = strtok(NULL, separator);
			}
			execvp(cmd, args);
			free(tempLine1);
			free(tempLine2);
		}
		else {
			fprintf(stderr, "Error: malloc tempLine2\n");
			exit(0);
		}
	}
	else {
		fprintf(stderr, "Error: malloc tempLine1\n");
		exit(0);
	}
}

void executeCmd(char **commands, int numCmd) {
	if (fork() == 0) {
		int i;
		for (i = 0; i < numCmd; i++) {
			executeLine(commands[i]);
		}
	}
}
