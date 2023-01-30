all: count_chars_solution.bin\
	rooms_solution.bin\
	tunnel_solution.bin\
	leader_solution.bin

CFLAGS+=-Wall -ggdb
LDLIBS+=-pthread

.PHONY: clean

%.bin: %.o
	${CC} ${CFLAGS} ${LDLIBS} -o $@ $^

clean:
	rm -rf *.o *.bin files/*.txt
