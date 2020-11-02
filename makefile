CC = gcc
CFLAGS = -c -Wall -pedantic -g

all: 
	$(CC) $(CFLAGS) execution.c
	$(CC) $(CFLAGS) graph.c
	$(CC) $(CFLAGS) parser.c
	$(CC) $(CFLAGS) main.c
	$(CC) -Wall -pedantic -o 537make parser.o graph.o execution.o main.o

test:
	$(CC) $(CFLAGS) execution.c
	$(CC) $(CFLAGS) graph.c
	$(CC) $(CFLAGS) parser.c
	$(CC) $(CFLAGS) tests/tests.c
	$(CC) -Wall -pedantic -o maketest parser.o graph.o execution.o tests.o

clean: 
	rm -f graph.o parser.o main.o testParser.o testGraph.o tests.o execution.o maketest 537make
