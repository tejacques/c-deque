HEADERS = utility.h deque.h
OBJECTS = utility.o deque.o
CC = gcc
CFLAGS = -Wall -Werror -g -std=c99

default: test-deque
all: default

utility.o: utility.c utility.h
	$(CC) $(CFLAGS) -c $< -o $@

deque.o: deque.c utility.o $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

test-deque: test.c test.h $(OBJECTS)
	$(CC) $(CFLAGS) $< $(OBJECTS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f deque
