all: dotproduct lounge tool savages

CFLAGS+=-ggdb
LDFLAGS+=-pthread

dotproduct: dotproduct.o

lounge: lounge.o	

tool: tool.o	

savages: savages.o

.PHONY: clean all

clean:
	rm -f *.o *.bin dotproduct tool lounge savages
