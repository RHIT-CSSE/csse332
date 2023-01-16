all: band.bin littleredhen.bin priority.bin threeJobs.bin

CFLAGS+=-ggdb
LDLIBS+=-pthread

%.bin: %.o
	$(CC) $(CFLAGS) $(LDLIBS) -o $@ $^

.PHONY: clean

clean:
	rm -rf *.o *.bin
