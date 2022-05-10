all: function.bin stack.bin

%.bin: %.c
	gcc -ggdb -m32 -static -fomit-frame-pointer -fno-stack-protector -z execstack -o $@ $^

clean:
	rm -rf *.bin *.o
