all: example.bin

.PHONY: clean all

CFLAGS+=-ggdb

%.bin: %.o
	${CC} ${CFLAGS} ${LDLIBS} -o $@ $^

clean:
	rm -rf *.bin *.o
