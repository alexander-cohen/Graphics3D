CC=gcc
INC=-I.

all: clean displayer.o image.o graphics2d.o vector.o matrix.o g2dwrapper.o # recompiles
	$(CC) displayer.o image.o graphics2d.o g2dwrapper.o vector.o matrix.o ${INC} -o display_xwindow -lX11 -lXext

run: all
	./display_xwindow

displayer.o: displayer.c
	$(CC) -c displayer.c ${INC} 

image.o: image.c
	$(CC) -c image.c ${INC} 

graphics2d.o: graphics2d/graphics2d.c
	$(CC) -c graphics2d/graphics2d.c ${INC} 

vector.o: vector.c
	$(CC) -c vector.c ${INC} 

matrix.o: matrix.c
	$(CC) -c matrix.c ${INC} 

g2dwrapper.o: graphics2d/g2dwrapper.c
	$(CC) -c graphics2d/g2dwrapper.c ${INC} 

clean:
	-rm *.o
	-rm display_xwindow
