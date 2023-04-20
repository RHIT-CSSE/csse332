all: sieve.bin shower.bin task_dispatch.bin stoplight.bin

CFLAGS+=-ggdb -Wall
LDLIBS+=-pthread

.PHONY: clean

%.bin: %.o
	$(CC) $(CFLAGS) $(LDLIBS) -o $@ $^

clean:
	rm -rf *.o *.bin
