all: max inorder prodcons_condvar rooms tunnel

.PHONY: clean all

LDLIBS += -pthread
CFLAGS += -ggdb

max: max.o

inorder: inorder.o

prodcons_condvar: prodcons_condvar.o

rooms: rooms.o

tunnel: tunnel.o

clean:
	rm -rf *.o max inorder prodcons_condvar rooms tunnel
