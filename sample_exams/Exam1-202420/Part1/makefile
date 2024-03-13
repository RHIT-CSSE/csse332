SOURCES := problem1.c problem2.c pretty.c prettier.c bonus.c problem3.c
TARGETS := $(SOURCES:%.c=%.bin)

all: $(TARGETS)

CC = gcc
CFLAGS += -ggdb -Werror
LDFLAGS += -lm

.PHONY: clean all

%.bin: %.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf *.o *.bin *.log

