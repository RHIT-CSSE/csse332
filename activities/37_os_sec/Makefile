.PHONY: all clean

all: part1 part2

CC := gcc
CFLAGS  := -m32 -static -U_FORTIFY_SOURCE
LDFLAGS :=
LDLIBS  :=

part1: CFLAGS += -fno-stack-protector -z execstack
part2: CFLAGS += -fno-stack-protector -z execstack


part1: part1.c

part2: part2.c


clean:
	rm -f part1 part2
