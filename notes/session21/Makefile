all: prodcons.bin onebuff.bin

.PHONY: clean

CFLAGS+=-ggdb
LDLIBS+=-pthread

%.bin: %.o
	$(CC) $(CFLAGS) $(LDLIBS) -o $@ $^

clean:
	rm -rf *.o *.bin
