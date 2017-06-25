CC=gcc -O3
INC=-I.

all: clean displayer.o graphics2d.o vector.o matrix.o g2dwrapper.o stack.o pong.o # recompiles
	$(CC) displayer.o graphics2d.o g2dwrapper.o vector.o matrix.o stack.o pong.o ${INC} -o display_xwindow -lX11 -lXext

run: all
	./display_xwindow

displayer.o: displayer.c
	$(CC) -c displayer.c ${INC} 

graphics2d.o: graphics2d/graphics2d.c
	$(CC) -c graphics2d/graphics2d.c ${INC} 

vector.o: linalg/vector.c
	$(CC) -c linalg/vector.c ${INC}

matrix.o: linalg/matrix.c
	$(CC) -c linalg/matrix.c ${INC} 

g2dwrapper.o: graphics2d/g2dwrapper.c
	$(CC) -c graphics2d/g2dwrapper.c ${INC} 

stack.o: stack.c
	$(CC) -c stack.c ${INC}

pong.o: pong.c
	$(CC) -c pong.c ${INC}

clean:
	-rm *.o
	-rm display_xwindow
