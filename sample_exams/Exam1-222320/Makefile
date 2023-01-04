all: server.bin client.bin loopy.bin

CFLAGS += -Werror -ggdb

server.bin: server.o connection.o colors.o
	cc ${CFLAGS} -o $@ $^

client.bin: client.o colors.o
	cc ${CFLAGS} -o $@ $^

loopy.bin: loopy.o
	cc ${CFLAGS} -o $@ $^

.PHONY: clean


clean:
	rm -rf *.bin *.o
