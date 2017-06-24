CC=gcc
CFLAGS=-I.

all: clean displayer.o image.o drawer.o# recompiles
	$(CC) displayer.o image.o drawer.o -I. -o display_xwindow -lX11 -lXext

run: all
	./display_xwindow

displayer.o: displayer.c
	$(CC) -c displayer.c -I.

image.o: image.c
	$(CC) -c image.c -I.

drawer.o: drawer.c
	$(CC) -c drawer.c -I.

clean:
	-rm *.o
	-rm display_xwindow