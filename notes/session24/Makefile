all: barbershop.bin barbershop_solution.bin

CFLAGS+=-ggdb
LDLIBS+=-pthread

.PHONY: clean

%.bin: %.o
	$(CC) $(CFLAGS) $(LDLIBS) -o $@ $^

clean:
	rm -rf *.o *.bin
