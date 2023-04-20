all: barbershop.bin barbershop_starvation.bin\
	barbershop_full.bin barbershop_solution.bin\
	cigarette_smokers.bin cigarette_smokers_solution.bin\
	cigarette_smokers_deadlock.bin

CFLAGS+=-ggdb
LDLIBS+=-pthread

.PHONY: clean

%.bin: %.o
	$(CC) $(CFLAGS) $(LDLIBS) -o $@ $^

clean:
	rm -rf *.o *.bin
