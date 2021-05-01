.PHONY: clean all

all: cigarette_problem.bin


cigarette_problem.bin: cigarette_problem.o

CFLAGS+=-ggdb
LDFLAGS+=-pthread

%.bin: %.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	rm -f *.o *.bin
