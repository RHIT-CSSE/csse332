---
title: BmOS - Input/Output
layout: togit
---

# BmOS: Input/Output

<!-- markdown-toc start - Don't edit this section. Run M-x markdown-toc-refresh-toc -->
**Table of Contents**

- [BmOS: Input/Output](#bmos-inputoutput)
- [Objective](#objective)
    - [-](#--1)
- [Using the Tools](#using-the-tools)
    - [Tools](#tools)
    - [QEMU](#qemu)
    - [The Bootloader](#the-bootloader)
- [A Hello World Kernel](#a-hello-world-kernel)
    - [Compiling kernel.c](#compiling-kernelc)
- [Makefile](#makefile)
- [Interrupt 0x10](#interrupt-0x10)
- [printString](#printstring)
- [Interrupt 0x16 and readString](#interrupt-0x16-and-readstring)
- [Interrupt 0x13 and readSector](#interrupt-0x13-and-readsector)
- [A User Defined Interrupt Handler](#a-user-defined-interrupt-handler)
- [An Improved User Defined Interrupt Handler](#an-improved-user-defined-interrupt-handler)
- [Running on a Real Computer (possible, but not worth it)](#running-on-a-real-computer-possible-but-not-worth-it)
- [Turning It In](#turning-it-in)
- [Rubric](#rubric)

<!-- markdown-toc end -->


# Objective

When a computer is turned on, it goes through a process known as
"booting".  The computer starts executing the BIOS (which comes with
the computer and is stored in ROM).  The BIOS loads and executes a
very small program called the *bootloader*, which is located at the
beginning of the first bootable disk.  The bootloader then loads and
executes the "kernel" -- a larger program that comprises the bulk of
the operating system.

In this milestone you will write a very small kernel that will print
out "Hello World" to the screen and hang.  Then you will learn how to
use some of the system calls provided by the BIOS.  You will write
your own system calls to print a string to the video display, read in
a line from the keyboard, and read a sector from the disk.

### A note about groups

Though this is a group project, we recommend you do the first hello
world kernel stuff individually to learn how the various tools fit
together.  From the Makefile on, do this project your groups.  You'll
still only submit one program at the end though.

# Using the Tools

## Tools

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

Test QEMU use the included test.img.  If you see some non-error
message appear in the QEMU window, you are ready to proceed.


## The Bootloader

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

Your first program in this milestone should be very simple.  It should
simply print out "Hello World" in the top left corner of the screen
and then stop running.  You should write your program in C and call it
kernel.c.

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

## Compiling kernel.c

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

Note that QEMU will print some other booting-related stuff on the
screen as well, but as long as your hello world is there your code is
working properly.

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

I've [created a list of 3 makefile tests](makefileTests.md) to help you
verify that your Makefile has been constructed correctly.

# Interrupt 0x10

In the previous milestone you saw how to print to the screen by
directly writing characters to the video memory.  The problem with
this is that you have to keep track of the cursor position yourself,
as well as scrolling when you reach the end of the page.
Alternatively, the BIOS provides a software interrupt that will take
care of printing to the screen for you.  Interrupt 0x10 calls
the BIOS to perform a variety of I/O functions.  If you call
interrupt 0x10 with 0xE in the AH register,
the ASCII character in the AL register is printed to the
screen at the current cursor location.

Since interrupts may only be called in assembly language, you are
provided with a function interrupt that makes an interrupt happen.
The interrupt function takes five parameters: the interrupt number,
and the interrupt parameters passed in the AX, BX, CX, and DX
registers, respectively.  It returns the value returned from the
interrupt in the AL register.

To use interrupt 0x10 to print out the letter 'Q',
you will need to do the following:

* Figure out the parameters.  To print out 'Q',
  AH must equal 0xE and AL must equal
  'Q' (0x51)
* Calculate the value of AX.  AX is always
  AH*256 + AL.
* Call the interrupt routine.  Since registers BX, CX, and DX are not
  used, pass 0 for those parameters.

Example:

    char al = 'Q';
    char ah = 0xe;
    int ax = ah * 256 + al;
    interrupt(0x10, ax, 0, 0, 0);

Or you could simply write:

    interrupt(0x10, 0xe*256+'Q', 0, 0, 0);


You can find a good discussion of the various BIOS interrupts at
http://en.wikipedia.org/wiki/BIOS\_interrupt\_call.  Another resource
is http://www.ctyme.com/intr/int.htm.


# printString

Write a function called printString which takes a C string as
an argument and returns void -- void printString(char
  *).  Remember that C strings are terminated with the character
0x0.  Your function should print out each character of the
array except for 0x0.  It should stop when it reaches
0x0.

You should test your function by calling: 
    
    printString("Hello World\0");

When adding functions to your C program, make sure they always follow
main.  main should always be your first function.
Make sure main returns a value of the correct type.  You may
also add ``while(1);'' near the end of main (see milestone
1).

When adding a function, you will need to include its signature or
prototype at the top of your .c file. For example, if your
function is:


    void printString(char *chars) {
        ...
    }

You will need to include void printString(char *); near the
top (before main) of your .c file.


# Interrupt 0x16 and readString


The BIOS interrupt for reading a character from the keyboard is
0x16.  When called, AH must equal 0 (actually, it is
okay if AX equals 0 since AL does not matter.  The interrupt
returns the ASCII code for the key pressed.

We've provided the code below for a function called readString which
takes as an argument an empty character array with space for at least
80 characters.  readString should calls interrupt 0x16 repeatedly and
saves the return value (i.e. the key pressed) in successive locations
in the character array until the ENTER key is pressed (ASCII 0xd).  It
then then adds a 0xa (line feed) and 0x0 (end of string) character to
the end of the array and return.


    /*  Reads a line from the console using Interrupt 0x16. */
    void readString(char *line)
    {
      int i, lineLength, ax;
      char charRead, backSpace, enter;
      lineLength = 80;
      i = 0;
      ax = 0;
      backSpace = 0x8;
      enter = 0xd;
      charRead = interrupt(0x16, ax, 0, 0, 0);
      while (charRead != enter && i < lineLength-2) {
        if (charRead != backSpace) {
          interrupt(0x10, 0xe*256+charRead, 0, 0, 0);
          line[i] = charRead;
          i++;
        } else {
          i--;
          if (i >= 0) {
    	interrupt(0x10, 0xe*256+charRead, 0, 0, 0);
    	interrupt(0x10, 0xe*256+'\0', 0, 0, 0);
    	interrupt(0x10, 0xe*256+backSpace, 0, 0, 0);
          }
          else {
    	i = 0;
          }
        }
        charRead = interrupt(0x16, ax, 0, 0, 0);  
      }
      line[i] = 0xa;
      line[i+1] = 0x0;
      
      /* correctly prints a newline */
      printString("\r\n");

      return;
    }


All characters typed are also printed to the screen and BACKSPACE is handled. 

Test both input and output by putting this in main:

    char line[80]; // Hint: this line needs to be at the top of main
                   // in ansi C declarations must be at the start of a function
    printString("Enter a line: \0");
    readString(line);
    printString(line);
    
    while(1); /* never forget this */


When you run this in Qemu, it should prompt you to enter a line.
When you press ENTER, it should echo what you typed back to
you at the start of the next line.

# Interrupt 0x13 and readSector

Interrupt 0x13 can be used to read or write sectors from the
floppy disk.  Reading sectors takes the following parameters:
  * AH = 2 (this number tells the BIOS to read a sector
  as opposed to write)
  * AL = number of sectors to read (use 1)
  * BX = address where the data should be stored (pass your char * array here)
  * CH = track number
  * CL = relative sector number
  * DH = head number
  * DL = device number (for the floppy disk, use 0)

Note that CH and CL, and DH and
DL, can be combined to CX and DX by
applying the formulas: CX=CH*256+CL and DX=DH*256+DL.

This interrupt requires you to know the cylinder, head, and track
number of the sector you want to read.  In this project we will be
dealing with absolute sector numbers.  Fortunately, there is a
conversion.

For floppy disks:
  * relative sector = ( sector MOD 18 ) + 1
  * head = ( sector DIV 18 ) MOD 2
  * track = ( sector DIV 36 )

Note: DIV is integer division and should round the result down.

Write a function called readSector which takes two arguments:
a predefined character array of 512 bytes or bigger and the sector
number to read -- readSector(char *buffer, int sector).  Your
function should compute the relative sector, head, and track, and call
interrupt 0x13 to read the sector into
buffer.

Unfortunately, bcc does not support MOD and DIV.  You will need to
write your own mod and div functions.

For mod(a, b) use:

    int mod(int a, int b)
    {
      int temp;
      temp = a;
      while (temp >= b) {
        temp = temp-b;
      }
      return temp;
    }

For div(a, b) use:

    int div(int a, int b)
    {
      int quotient;
      quotient = 0;
      while ((quotient + 1) * b <= a) {
        quotient++;
      }
      return quotient;
    }

To test your readSector function, you should read in a sector
containing ASCII text and print it out using printString.

Add the following to main:

    char buffer[512];
    readSector(buffer, 30);
    printString(buffer);

Use the file message.txt, which is in your m2 directory.

After you compile your floppya.img, type the following to put
message.txt at sector 30:
  
    dd if=message.txt of=floppya.img bs=512 count=1 seek=30 conv=notrunc

Run Qemu.  If the message in message.txt prints out, your readSector
  function works.

# A User Defined Interrupt Handler

An operating system should provide services to user programs by
creating its own interrupts.  You will now create an interrupt 0x21
handler.  When an interrupt 0x21 is called, it should run your code.

Creating an interrupt service routine is simply a matter of creating a
function, and putting the address of that function in the correct
entry of the interrupt vector table.  The interrupt vector table sits
at the absolute bottom of memory and contains a 4 byte address for
each interrupt number.  To add a service routine for interrupt
0x21, write a function to be called when interrupt
0x21 occurs and then put the address of that function at
0x00084 (21*4) in memory.

Setting up interrupts must be done in assembly code.
kernel.asm provides two functions to help.
makeInterrupt21 sets up the interrupt 0x21 service routine.
From this point on, the function interrupt21ServiceRoutine
will be called whenever an interrupt 0x21 occurs.  It calls a
function in your C code handleInterrupt21(int ax, int bx, int
  cx, int dx) that you must write.  The AX, BX,
CX, DX parameters passed in the interrupt call will
show up in your handleInterrupt21 function as parameters
ax, bx, cx, dx.


In kernel.asm uncomment (remove the semicolons) the
  line ".extern \_handleInterrupt21" and all lines under
  \_interrupt21ServiceRoutine:.

In your C program, create the function void
    handleInterrupt21(int ax, int bx, int cx, int dx).  In it, use
  printString to print out a message (like ``Hello world'').

In main, add a call to makeInterrupt21 and
  then generate interrupt 0x21: interrupt(0x21, 0, 0, 0, 0).

Compile and run your program.  If it works, Qemu will print
  out your message.



# An Improved User Defined Interrupt Handler

Modify your interrupt 0x21 handler handleInterrupt21
to provide printString, readString and
readSector services.  Your interrupt 0x21 will be
defined as follows:

* AX = a number that determines which function to run
* print string:
  - AX = 0
  - BX = address of the string to print
* read string:
  - AX = 1
  - BX = address of the character array where the keys entered will go
* read sector:
  - AX = 2
  - BX = address of the character array where the sector will go
  - CX = the sector number
* if AX = 3 or more, print an error message

You should test your work by making interrupt 0x21 calls and seeing
that they work correctly.

In main add the following:

    char line[80];
    makeInterrupt21();
    interrupt(0x21,1,line,0,0);
    interrupt(0x21,0,line,0,0);


If your program works, it should wait for you to type in a line and
hit ENTER.  Then it should echo it back to you at the start
of the next line.

Unlike the printString and readString functions,
which can only be called from within your kernel.c program,
these interrupt 0x21 routines can be called from other
programs that do not have these functions.

# Running on a Real Computer (possible, but not worth it)

Up until a few years ago, it was possible to run this on modern
computers after installing it on a USB stick.  More recent computers
though have a different boot sequence that is not compatible and so
this is no longer easy.

# Turning It In

Be sure to submit your updated kernel.c and Makefile along with all of
the files necessary to build from scratch and run your OS to your team
repository in the the InputOutput folder.

Your makefile should include:
1. Compiling the bootloader using nasm
2. Zeroing the floppy image
3. Coping bootloader to floppy image
4. Compile kernel with bcc and  links with kernel.asm
5. Coping kernel to floppy image in sector 3 and message.txt to sector 30

Your main function should include:
1. Printing out hello world at the top left using raw memory commands
2. Using printString to display a string to the console
3. Reading and displaying characters entered with a keyboard
4. Displaying the string read back to the console at the start of a new line
5. Using readSector to read the sector that contains message.txt and
   displays the string to the console

Steps 2-5 should be accomplished by using your interrupt handler, not
by direct calls to printstring etc.

Please include a comment at the top of each file with the name of each
team member and your team number. 

# Rubric

Total 140 points

| Part                                                                                                                      | Point Value |
|:--------------------------------------------------------------------------------------------------------------------------|-------------|
| Makefile compiles someting that runs in qemu from source files                                                            | 20          |
| All the Makefile tests pass                                                                                               | 25          |
| Hello world prints on screen using putInMemory commands in main                                                           | 20          |
| Defines printString, which works                                                                                          | 15          |
| Defines readString, which works                           s                                                               | 10          |
| Defines readSector, which works                                                                                           | 20          |
| Implements handleInterrupt21, responds to BIOS interrupt 21, and calls different functions when passed correct parameters | 15          |
| A main that demos all functionality                                                                                       | 15          |
    

