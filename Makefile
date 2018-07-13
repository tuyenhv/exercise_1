CC = gcc

CFLAGS = -Wall

LDFLAGS=-lm

all: main

main: main.o
	$(CC) $(CFLAGS) main.o -o main $(LDFLAGS)

clean:
	rm -f main.o main
