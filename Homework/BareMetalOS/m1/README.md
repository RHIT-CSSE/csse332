---
title: Milestone 1
layout: togit
---

# Milestone 1

# Objective

When a computer is turned on, it goes through a process known as
"booting".  The computer starts executing the BIOS (which comes with
the computer and is stored in ROM).  The BIOS loads and executes a
very small program called the *bootloader*, which is located at the
beginning of the first bootable disk.  The bootloader then loads and
executes the "kernel" -- a larger program that comprises the bulk of
the operating system.

In this milestone you will write a very small kernel that will print
out "Hello World" to the screen and hang.

This is a warm-up milestone intended to get you familiar with the tools
and simulator that you will use in subsequent milestones.


# Tools

You will need the following utilities to complete this and subsequent
milestones:

* QEMU - a generic and open source machine emulator and virtualizer supporting x86.
* bcc (Bruce Evan's C Compiler) - A 16-bit C compiler
* as86, ld86 - A 16 bit assembler and linker that typically comes with bcc
* gcc - The standard 32-bit GNU C compiler
* nasm - The Netwide Assembler
* hexedit - A utility that allows you to edit a file in hexadecimal byte-by-byte
* dd - A standard low-level copying utility. This generally comes with UNIX.
* a text editor of your choice


## QEMU

QEMU is an x86 hardware platform simulator.

You can install Qemu on Ubuntu by running "sudo apt-get
  install qemu" in a terminal.

To start Qemu with a floppy disk image, pass the image file as an
argument to the "-fda" flag like this:

    qemu-system-i386 -fda floppya.img -boot order=a &

Optionally, to start Qemu in a way that makes shutdown relatively easy
(in code) you may run it with the following additional parameters:

    qemu-system-i386 -fda floppya.img -device isa-debug-exit,iobase=0xf4,iosize=0x04 -boot order=a &

To test QEMU use test.img.  If you see some non-error message 
appear in the QEMU window, you are ready to proceed.


# The Bootloader

The first thing that the computer does after powering on is read the
bootloader from the first sector of the floppy disk into memory and
start it running. A floppy disk is divided into sectors, where each
sector is 512 bytes. All reading and writing to the disk must be in
whole sectors -- it is impossible to read or write a single
byte. The bootloader is required to fit into Sector 0 of the disk, be
exactly 512 bytes in size, and end with the special hexadecimal code
"55 AA."  Since there is not much that can be done with a 510 byte
program, the whole purpose of the bootloader is to load the larger
operating system from the disk to memory and execute it.

Since bootloaders have to be very small and handle such operations as
setting up registers, it does not make sense to write it in any
language other than assembly.  Consequently you are not required to
write a bootloader in this project -- one is supplied to you in
bootload.asm. You will need to assemble and execute it.

If you look at bootload.asm, you will notice that it is a
very small program that does three things.  First it sets up the
segment registers and the stack to memory 0x10000 (in hex). This is where it
puts the kernel in memory. Second, it reads 10 sectors (5120 bytes)
from the disk starting at sector 3 and puts them at 0x10000. This
would be fairly complicated if it had to talk to the disk driver
directly, but fortunately the BIOS has a disk read function. This disk
read function is accessed by putting the various parameters into
various registers, and calling Interrupt 13 (in hex).  After the
interrupt, the program at sectors 3-12 is now in memory at 0x10000.  The
last thing that the bootloader does is it jumps to 0x10000, starting
whatever program it just placed there.  That program is the one that
you will write.  Notice that after the jump it fills out the remaining
bytes with 0, and then sets the last two bytes to 55 AA, telling the
computer that this is a valid bootloader.

To install the bootloader, you first have to assemble it.  The
bootloader is written in x86 assembly language. To assemble it, type

    nasm bootload.asm 

The output file bootload is the actual machine language file.

You can look at the bootloader using the hexedit utility.
Type "hexedit bootload".  You will see a few lines of numbers,
which is the machine code in hexadecimal.  Below you will see a lot of
00s.  At the end, you will see the magic number 55 AA.

Next you should make an image file of a floppy disk that is filled
with zeros.  You can do this using the dd utility. Type

    dd if=/dev/zero of=floppya.img bs=512 count=2880
    
This will copy 2880 blocks of 512 bytes each from /dev/zero and put it
in file floppya.img. 2880 is the number of sectors on a
3.5 inch floppy, and /dev/zero is a phony file
containing only zeros.  What you will end up with is a 1.47 megaByte
file floppya.img filled with zeros.

Finally you should copy bootload to the beginning of
floppya.img. Type 

    dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc}
    
If you look at floppya.img now with hexedit, you will notice that the contents of
bootload are at the beginning of floppya.img.

If you want, you can try running floppya.img with Qemu.
Nothing meaningful will happen because the bootloader just loads and
runs garbage.  You need to write your program now and put it at sector
3 of floppya.img}.


# A Hello World Kernel

Your program in this milestone should be very simple.  It should
simply print out "Hello World" in the top left corner of
the screen and then stop running.  You should write your program in C
and call it kernel.c.

When writing C programs for your operating system, you should note
that all the typical C library functions, such as printf,
scanf, putchar, etc. are unavailable to
you.  This is because these functions make use of services provided by
Linux.  Since Linux will not be running when your operating system is
running, these functions will not work (or even compile).  You can
only use the basic C commands.

Stopping the kernel is the simple part.  After printing out
"Hello World", you don't want anything else to run.
*The simplest way to tie up the computer is to put your program
  into an infinite while loop.*

Printing "Hello World" is a little more difficult since you
cannot use the C printf or putchar commands.
Instead you have to write directly to video memory.  Video memory
starts at address 0xB8000.  Every byte of video memory refers to the
location of a character on the screen.  In text mode, the screen is
organized as 25 lines with 80 characters per line.  Each character
takes up two bytes of video memory.  The first byte is the ASCII code
for the character and the second byte tells what color to draw the
character.  The memory is organized line-by-line.  To draw the letter
'A' at the beginning of the third line down, you would have to do the
following:


  1. Compute the address relative to the beginning of video memory: 80 * (3-1) = 160
  2. Multiply that by 2 bytes / character: 160 * 2 = 320
  3. Convert that to hexadecimal 320 = 0x140
  4. Add that to 0xB8000: 0xB8000+0x140 = 0xB8140 - this is the address in memory
  5. Write the letter 'A' to address 0xB8140.  The letter A is represented in ASCII by the hexadecimal number 0x41.
  6. Write the color white (0x7) to address 0xB8141

Since 16 bit C provides no built in mechanism for writing to memory,
you are provided with an assembly file, kernel.asm. This file
contains a function putInMemory, which writes a byte to
memory.  The function putInMemory can be called from your C
program and takes three parameters:

  1. The first hexadecimal digit of the address, times 0x1000.  This
  is called the segment.
  2. The remaining four hexadecimal digits of the address.
  3. The character to be written

To write 'A' to address 0xB8140, you should call:

    putInMemory(0xB000, 0x8140, 'A');

To print out the letter A in white at the beginning of the third line
of the screen, call:

    putInMemory(0xB000, 0x8140, 'A');
    putInMemory(0xB000, 0x8141, 0x7);

You should now be able to write a C kernel program to print out
"Hello World" at the top left corner of the screen.

# Compiling kernel.c

To compile your C program you cannot use the standard Linux C compiler
gcc.  This is because gcc generates 32-bit machine code, while the
computer during start up runs only 16-bit machine code.  Most real
operating systems force the processor to make a transition from 16-bit
mode to 32-bit mode, but we are not going to do this. bcc is a 16-bit
C compiler.  It is fairly primitive and requires you to use the early
Kernighan and Ritchie C syntax rather than later dialects.  For
example, you have to use /* */ for all comments.  // is not
recognized.  You should not expect programs that compile with gcc to
necessarily compile with bcc (though if bcc is giving you an obtuse
error message, you can try compiling with gcc -ansi to see if there
error is clearer there).

To compile your kernel, type 

    bcc -ansi -c -o kernel.o kernel.c
    
The -c flag tells the compiler to compile only
this file. The -o flag tells it to produce an output file
called kernel.o.

kernel.o is not your final machine code file.  It needs to be
linked with kernel.asm so that the final file contains both
your code and the int10 assembly function.  You will need to
type two more lines:

    as86 kernel.asm -o kernel_asm.o (to assemble kernel.asm)
    ld86 -o kernel -d kernel.o kernel_asm.o (to link them all together and produce kernel}

The file kernel is your program in machine code.  To run it,
you will need to copy it to floppya.img at sector 3, where
the bootloader is expecting to find it.  In an upcoming milestone you
will find out why sector 3 and not sector 1.  To copy it, type

    dd if=kernel of=floppya.img bs=512 conv=notrunc seek=3

where "seek=3" tells it to copy kernel to the third
sector.

Try running Qemu.  If your program is correct, you should see "Hello
World" printed out.


# Makefile

Notice that producing a final floppya.img file requires you
to type several lines that are very tedious.  An easier alternative is
to use a Makefile.

Be sure your Makefile correctly represents the dependencies of all the
various files (e.g. kernel should depend on kernel.o and
kernel_asm.o).  It should only rebuild what is necessary.

When you make a new floppy image, though, it is wise to construct it
from a full set of zeros each time, rather than re-editing the same
image file (which will otherwise accumulate garbage, because your
compiled kernel will be various lengths).


# Running on a Real Computer (optional)

Up until a few years ago, it was possible to run this on modern
computers after installing it on a USB stick.  More recent computers
though have a different boot sequence that is not compatible and so
this is no longer easy.

# Turning It In

You will submit this code via your Git repo.

Be sure to submit your kernel.c and Makefile or compileOS.sh files to
your team repository in the the m1 folder.  Please include a comment
at the top of each file with the name of each team member and your
team number.  In addition, be sure to submit all other deliverables
that are highlighted on the project’s main page.  I encourage you to
check out a clean copy of your submission from your team repository
and ensure that it builds completely from sources (i.e. bootload.asm},
kernel.asm, kernel.c), regenerates floppya.img and runs correctly.
