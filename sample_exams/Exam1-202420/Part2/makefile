SOURCES := problem4.c problem5.c buffalosay.c buffalopipe.c
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

