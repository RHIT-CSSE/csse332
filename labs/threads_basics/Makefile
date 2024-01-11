CC = gcc
CFLAGS = -Wall -g -pthread -c
LDFLAGS = -pthread
SOURCES = threadsExample.c threadsBasicExample.c
PROG1 = threadsExample
PROG2 = threadsBasicExample

all: $(PROG1) $(PROG2)

# build executables
# $^ matches all the dependencies
$(PROG1): threadsExample.o
	$(CC) $(LDFLAGS) $^ -o $@

$(PROG2): threadsBasicExample.o
	$(CC) $(LDFLAGS) $^ -o $@

# convert .c files to .o files
# $< matches the first dependency
# $@ matches the target
$(SOURCES:%.c.o):
	$(CC) $(CFLAGS) $< $@

clean:
	rm -rf *.o *~ $(PROG1) $(PROG2)