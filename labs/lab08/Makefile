all: band.bin littleredhen.bin priority.bin threeJobs.bin

CFLAGS+=-ggdb
LDFLIBS+=-pthread -lc

%.bin: %.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

.PHONY: clean

clean:
	rm -rf *.o *.bin
