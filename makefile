CC = gcc
CFLAGS = -Wall -Wextra

all: maze maze_test

maze:
	$(CC) -DMAIN_PROGRAM -o maze maze.c $(LDFLAGS)

maze_test:
	$(CC) $(CFLAGS) -o maze_test maze_test.c $(LDFLAGS)

# Cible pour exécuter les tests
.PHONY: test
test: maze_test
	./tests.sh

valgrind: maze
	valgrind ./maze 100 100
	valgrind ./tests.sh

clean:
	rm -f maze maze_test *.o
