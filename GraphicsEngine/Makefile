CC=gcc -g
INC=-I.
CARGS=

all: clean displayer.o graphics2d.o vector.o matrix.o g2dwrapper.o stack.o pong.o helper.o # recompiles
	$(CC) displayer.o graphics2d.o g2dwrapper.o vector.o matrix.o stack.o pong.o helper.o ${INC} ${CARGS} -o display_xwindow -lX11 -lXext

run: all
	./display_xwindow

displayer.o: displayer.c
	$(CC) -c displayer.c ${INC} ${CARGS}

graphics2d.o: graphics2d/graphics2d.c
	$(CC) -c -mavx graphics2d/graphics2d.c ${INC} ${CARGS}

vector.o: linalg/vector.c
	$(CC) -c linalg/vector.c ${INC} ${CARGS}

matrix.o: linalg/matrix.c
	$(CC) -c linalg/matrix.c ${INC} ${CARGS}

g2dwrapper.o: graphics2d/g2dwrapper.c
	$(CC) -c graphics2d/g2dwrapper.c ${INC} ${CARGS}

stack.o: stack.c
	$(CC) -c stack.c ${INC} ${CARGS}

pong.o: pong.c
	$(CC) -c pong.c ${INC} ${CARGS}

helper.o: helper.c
	$(CC) -c helper.c ${INC} ${CARGS}

clean:
	-rm *.o
	-rm display_xwindow
