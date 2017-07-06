CC=gcc -w -g
INC=-I.
CARGS=

all: clean graphics2do linalgo thiso pipelineo datastructureso pongo objectso prepipelineo
	$(CC) *.o ${INC} ${CARGS} -o display_xwindow -lX11 -lXext -lm

run: all
	./display_xwindow

thiso:
	$(CC) -c *.c ${INC} ${CARGS}

pipelineo:
	$(CC) -c pipeline/*.c ${INC} ${CARGS}

datastructureso:
	$(CC) -c datastructures/*.c ${INC} ${CARGS}

graphics2do:
	$(CC) -c graphics2d/*.c ${INC} ${CARGS}

linalgo:
	$(CC) -c linalg/*.c ${INC} ${CARGS}

pongo:
	$(CC) -c pong/*.c ${INC} ${CARGS}

objectso:
	$(CC) -c objects/*.c ${INC} ${CARGS}

prepipelineo:
	$(CC) -c prepipeline/*.c ${INC} ${CARGS}

clean:
	-rm *.o
	-rm display_xwindow
