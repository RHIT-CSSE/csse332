
all: kernel shell disk run

kernel: kernel_asm.o main.o
	ld86 -o kernel -d main.o kernel_asm.o  

kernel_asm.o: kernel.asm
	as86 kernel.asm -o kernel_asm.o

main.o: main.c
	bcc -ansi -c -o main.o main.c

shell: lib.o shell.o
	ld86 -o shell -d shell.o lib.o  

lib.o: lib.asm
	as86 lib.asm -o lib.o

shell.o: shell.c
	bcc -ansi -c -o shell.o shell.c

disk: kernel bootload.asm map.img dir.img loadFile shell
	nasm bootload.asm
	dd if=/dev/zero of=floppya.img bs=512 count=2880
	dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc
	dd if=map.img of=floppya.img bs=512 count=1 seek=1 conv=notrunc 
	dd if=dir.img of=floppya.img bs=512 count=1 seek=2 conv=notrunc 
	dd if=kernel of=floppya.img bs=512 conv=notrunc seek=3
	# dd if=message.txt of=floppya.img bs=512 count=1 seek=30 conv=notrunc
	./loadFile shell

loadFile: loadFile.c
	gcc -o loadFile loadFile.c

clean:
	rm *.o kernel floppya.img loadFile shell

run: floppya.img
	qemu-system-i386 -fda floppya.img -device isa-debug-exit,iobase=0xf4,iosize=0x04 -boot order=a &
	# qemu-system-i386 -fda floppya.img -boot order=a &
