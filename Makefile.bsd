CFLAGS=-Wall
LDFLAGS=-lm
CC=clang

ttf: ttf.o
	${CC} -o ${@} ${>} ${LDFLAGS}

ttf.o: ttf.c

.c.o:
	${CC} -c ${<} ${CFLAGS}

clean:
	rm -f ttf *.core *.o
