#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "execution.h"

/**
 * Parse line into args then execvp
 *
 * @param:
 *	line - command string
 */
void executeLine(char *line) {
	pid_t pid;
  	int status;
  	pid = fork();
  	int lineLength = (int)strlen(line)+1;
	char *tempLine1 = malloc(lineLength * sizeof(char));
	char *tempLine2 = malloc(lineLength * sizeof(char));
	if (pid == 0) {
		if (tempLine1 != NULL) {
			if (tempLine2 != NULL) {
				strcpy(tempLine1, line);
				strcpy(tempLine2, line);
				char *separator = " ";
				char *arg = strtok(tempLine1, separator);
				arg = strtok(NULL, separator);
				int argNum = 1;
				while (arg != NULL) {
					argNum++;
					arg = strtok(NULL, separator);
				}
				char *args[argNum+1];
				char *cmd = strtok(tempLine2, separator);
				arg = strtok(NULL, separator);
				args[0] = cmd;
				int i = 1;
				while(arg != NULL) {
					args[i] = arg;
					i++;
					arg = strtok(NULL, separator);
				}
				args[argNum] = NULL;
				i = 0;
				while (i < argNum+1) {
					//printf("arg: %s\n", args[i]);
					i++;
				}
				execvp(args[0], args);
				fprintf(stderr, "Error: execvp failed\n");
				exit(0);
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
	
	else {
		while (wait(&status) != pid){
		}
		
		if (tempLine1 != NULL) {
			free(tempLine1);
		}
		if (tempLine2 != NULL) {
			free(tempLine2);
		}
	}
}

/**
 * Executes each command in array
 *
 * @param:
 *	commands - array of command strings
 *	numCmd - number of command strings
 */
void executeCmd(char **commands, int numCmd) {
	for (int i = 0; i < numCmd; i++) {
		//printf("Running line:%s.\n", commands[i]);
		executeLine(commands[i]);
	}
}
