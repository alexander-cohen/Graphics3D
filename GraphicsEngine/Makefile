CC=gcc
CFLAGS=-I.

all: clean displayer.o image.o vector.o # recompiles
	$(CC) displayer.o image.o vector.o -I. -o display_xwindow -lX11 -lXext -lm

run: all
	./display_xwindow

displayer.o: displayer.c
	$(CC) -c displayer.c -I.

image.o: image.c
	$(CC) -c image.c -I.

vector.o: vector.c
	$(CC) -c vector.c -I. 

clean:
	-rm *.o
	-rm display_xwindow