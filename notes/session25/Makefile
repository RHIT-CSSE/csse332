all: barbershop_solution.bin barbershop_full.bin

CFLAGS+=-ggdb -Wall
LDLIBS+=-pthread

.PHONY: clean

%.bin: %.o
	$(CC) $(CFLAGS) $(LDLIBS) -o $@ $^

clean:
	rm -rf *.o *.bin
