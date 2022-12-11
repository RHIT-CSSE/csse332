all: simple_test.bin primes.bin

simple_test.bin: simple_test.o

primes.bin: primes.o

CFLAGS += -ggdb

.PHONY: clean

%.bin: %.o
	${CC} ${CFLAGS} -o $@ $^

clean:
	rm -rf *.bin *.o
