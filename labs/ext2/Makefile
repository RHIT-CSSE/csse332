CC= gcc
CFLAGS= -Wall -ggdb

all: testcode filereader

testcode: ext2access.o testcode.o
	${CC} -o $@ ${CFLAGS} $^ -lm

filereader: ext2access.o filereader.o
	${CC} -o $@ ${CFLAGS} $^ -lm

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f filereader testcode *.o *~ #*
