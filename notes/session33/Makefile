all: function.bin stack.bin wonderful.bin nasty.bin

CFLAGS+=-ggdb -m32 -static -U_FORTIFY_SOURCE -fomit-frame-pointer
CFLAGS+=-fno-stack-protector -z execstack

%.bin: %.c
	gcc ${CFLAGS} -o $@ $^

clean:
	rm -rf *.bin *.o
