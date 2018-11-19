CC=gcc
CFLAGS = -Wall -g -ansi


all: myCopy createAndWait2 createAndWait createChild

createAndWait2: createChildAndWait-v2.c
	$(CC) -o createAndWait2 createChildAndWait-v2.c $(CFLAGS)

createAndWait: createChildAndWait.c
	$(CC) -o createAndWait createChildAndWait.c $(CFLAGS)

myCopy: myCopy.c
	$(CC) -o myCopy myCopy.c $(CFLAGS)

createChild: createChild.c
	$(CC) -o createChild createChild.c $(CFLAGS)

clean:
	rm -rf  createAndWait createAndWait2 createChild myCopy *.o *.out
