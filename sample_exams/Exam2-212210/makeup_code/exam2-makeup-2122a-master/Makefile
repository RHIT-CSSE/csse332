EXECUTABLES:=packets sushi canoe

LDLIBS+=-pthread
CFLAGS+=-ggdb

all: ${EXECUTABLES}

.PHONY: clean all

packets.o: packets.c

sushi.o: sushi.c

canoe.o: canoe.c


clean:
	rm -f *.o ${EXECUTABLES}
