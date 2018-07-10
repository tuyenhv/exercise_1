CC=gcc 
CFLAGS=-Wall

all: main
main: main.o
main.o: main.c points.h

clean:
	rm -f main *.o
