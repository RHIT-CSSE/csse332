CC = gcc
CFLAGS = -Wall -g -pthread -c
LDFLAGS = -pthread
SOURCES = add_a_lot.c red_blue_purple.c
PROG1 = addALot
PROG2 = redBluePurple

all: $(PROG1) $(PROG2)

# build executables
# $^ matches all the dependencies
$(PROG1): add_a_lot.o
	$(CC) $(LDFLAGS) $^ -o $@

$(PROG2): red_blue_purple.o
	$(CC) $(LDFLAGS) $^ -o $@

# convert .c files to .o files
# $< matches the first dependency
# $@ matches the target
$(SOURCES:%.c.o):
	$(CC) $(CFLAGS) $< $@

clean:
	rm -rf *.o *~ $(PROG1) $(PROG2)