CC=gcc
CFLAGS=-I.

all: clean displayer.o image.o graphics2d.o vector.o matrix.o # recompiles
	$(CC) displayer.o image.o graphics2d.o vector.o matrix.o -I. -o display_xwindow -lX11 -lXext

run: all
	./display_xwindow

displayer.o: displayer.c
	$(CC) -c displayer.c -I.

image.o: image.c
	$(CC) -c image.c -I.

graphics2d.o: graphics2d.c
	$(CC) -c graphics2d.c -I.

vector.o: vector.c
	$(CC) -c vector.c -I. -lm

matrix.o: matrix.c
	$(CC) -c matrix.c -I.

clean:
	-rm *.o
	-rm display_xwindow