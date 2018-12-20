---
title: Milestone 2
layout: togit
---


# Milestone 2

# Objective

One of the major services an operating system provides are system
calls.  In this milestone you will learn how to use some of the system
calls provided by the BIOS.  You will then write your own system calls
to print a string to the video display, read in a line from the
keyboard, and read a sector from the disk.  This will create the
foundation needed for the next milestone.


# Tools

You will use the same tools you used in the last milestone, and you
will also need to have completed the previous milestone successfully.
Additionally, copy m2 from the 332 github into your project folder,,
which contains the kernel.asm needed for this project.


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

Write a function called readString which takes as an argument
an empty character array with space for at least 80 characters.
readString should call interrupt 0x16 repeatedly and
save the return value (i.e. the key pressed) in successive locations
in the character array until the ENTER key is pressed (ASCII
0xd).  It should then add a 0xa (line feed) and
0x0 (end of string) character to the end of the array and
return.

All characters typed should also be printed to the screen (otherwise
the user will not see what is typed).  After reading a character, the
character should be printed to the screen using interrupt
0x10.

Your function should be able to handle the BACKSPACE key.
When a backspace (ASCII 0x8) is pressed, it should print the
backspace to the screen but not store it in the array.  Instead it
should decrease the array index.  Make sure the array index does not
go below zero.

If your function works, you should be able to call the following code
in main:

    char line[80];
    printString("Enter a line: \0");
    readString(line);
    printString(line);


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

    while a > = b
        a = a - b
    return a

For div(a, b) use:

    let quotient = 0
    while (quotient  + 1)* b  <= a
        quotient  = quotient  + 1
    return quotient  

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


# Turning It In

Be sure to submit your updated kernel.c and Makefile along with all of
the files necessary to build from scratch and run your OS to your team
repository in the the m2 folder.  Please include a comment at the top
of each file with the name of each team member and your team number.
In addition, be sure to submit all other deliverables that are
highlighted on the project’s main page.
