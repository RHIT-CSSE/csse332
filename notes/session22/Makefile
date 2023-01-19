all: readers_writers.bin\
	readers_writers_solution.bin\
	readers_writers_starvation.bin\
	thread_ABC.bin\
	thread_ABC_solution.bin

CFLAGS+=-ggdb
LDLIBS+=-pthread

.PHONY: clean

%.bin: %.o
	$(CC) $(CFLAGS) $(LDLIBS) -o $@ $^

clean:
	rm -rf *.o *.bin
