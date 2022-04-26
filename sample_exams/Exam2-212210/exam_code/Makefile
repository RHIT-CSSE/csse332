all: pr_condvar bonding salad

.PHONY: all clean

LDLIBS+=-pthread

pr_condvar: pr_condvar.o

bonding: bonding.o

salad: salad.o

clean:
	rm -f *.o pr_condvar bonding salad
