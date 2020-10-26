CC = gcc
CFLAGS = -c -Wall -pedantic

all: 
	$(CC) $(CFLAGS) graph.c
	$(CC) $(CFLAGS) parser.c
	$(CC) -Wall -pedantic -o 537make parser.o graph.o