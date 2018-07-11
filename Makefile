CC=gcc 
CFLAGS=-Wall
LDFLAGS=-lm

all: main
main: main.o
main.o: main.c points.h

clean:
	rm -f main *.o
