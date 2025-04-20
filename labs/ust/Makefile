all: basic_threads_in_use_example.bin example1.bin tests.bin

CFLAGS+=-ggdb -Wall

.PHONY: clean

basic_threads_in_use_example.bin: basic_threads_in_use_example.o basic_threads.o
	${CC} ${CFLAGS} -o $@ $^

example1.bin: example1.o basic_threads.o
	${CC} ${CFLAGS} -o $@ $^

tests.bin: tests.o basic_threads.o CuTest.o
	${CC} ${CFLAGS} -o $@ $^

clean:
	rm -rf *.o *.bin
