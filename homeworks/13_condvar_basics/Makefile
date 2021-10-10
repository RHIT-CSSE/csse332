all: max inorder prodcons_condvar

.PHONY: clean all

LDLIBS += -pthread

max: max.o

inorder: inorder.o

prodcons_condvar: prodcons_condvar.o

clean:
	rm -rf *.o max inorder prodcons_condvar
