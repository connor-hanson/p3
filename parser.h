#ifndef PARSER_H
#define PARSER_H

FILE* getMakeFile();

void runTarget(FILE *fp, char *target);

#endif
