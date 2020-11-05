
537make: main.o parser.o graph.o execution.o
	gcc -Wall -pedantic -o 537make graph.o execution.o parser.o main.o
	echo built successfully....

main.o: main.c parser.h graph.h
	gcc -c -Wall -Wextra -pedantic -g main.c

parser.o: parser.c parser.h graph.h
	gcc -c -Wall -Wextra -pedantic -g parser.c

graph.o: graph.c graph.h execution.h
	gcc -c -Wall -Wextra -pedantic -g graph.c

execution.o: execution.c execution.h
	gcc -c -Wall -Wextra -pedantic -g execution.c

clean:
	rm -f graph.o parser.o main.o testParser.o testGraph.o tests.o execution.o maketest

clean-all:
	rm -f graph.o parser.o main.o testParser.o testGraph.o tests.o execution.o maketest 537make