---
title: BmOS - Filesystem
layout: togit
---

# BmOs: Filesystem

<!-- markdown-toc start - Don't edit this section. Run M-x markdown-toc-refresh-toc -->
**Table of Contents**

- [Objective](#objective)
- [Tools](#tools)
- [The File System](#the-file-system)
- [Initial Map and Directory](#initial-map-and-directory)
- [loadFile](#loadfile)
- [Load A File And Print It](#load-a-file-and-print-it)
- [Testing](#testing)
- [Load A Program And Execute It](#load-a-program-and-execute-it)
- [A Warning](#a-warning)
- [Testing](#testing-1)
- [Terminate Program System Call](#terminate-program-system-call)
- [The Shell - Making Your Own User Program](#the-shell---making-your-own-user-program)
    - [Kernel adjustments](#kernel-adjustments)
    - [Shell Command "type"](#shell-command-type)
    - [Shell Command "execute"](#shell-command-execute)
    - [Shell Command "dir"](#shell-command-dir)
- [Write Sector](#write-sector)
- [Writing A File](#writing-a-file)
- [Copying A File](#copying-a-file)
- [Turning It In](#turning-it-in)
- [Rubric](#rubric)

<!-- markdown-toc end -->


# Objective

In this milestone you will write routines to read files into memory and
execute programs.  You will then write a basic shell program that will
execute other programs and print out ASCII text files.


# Tools

You will use the same utilities you used in the last milestone, and
you will also need to have completed the previous milestones
successfully.

This milestone directory includes a new bunch of files.  Copy your
existing BareMetalOs version from the InputOutput directory into a new
directory called "Filesystem" and then copy these new files over the
top.  You'll keep your milestone 1 submission in its own directory to
make everybody's grading life easier.

# The File System

One of the key functions of a file system is to keep a record of the
names and sectors of files on the disk.  The file system in this
operating system utilizes two sectors at the beginning of the disk.
The Disk Map sits at sector 1, and the Directory
sits at sector 2.  This is the reason your kernel starts at sector 3.

The Map records which sectors are available and which sectors
are currently used by files.  This makes it easy to find a free sector
when writing a file.  Each sector on the disk is represented by one
byte in the Map.  A byte entry of 0xFF means that the sector
is used.  A byte entry of 0x00 means that the sector is free.  You
will not need to read or modify the Map in this milestone
since you are only reading files in this milestone.  You will need to
read and modify the Map in the next milestone.

The Directory lists the names and locations of the files.
There are 16 file entries in the Directory and each entry
contains 32 bytes (32 times 16 = 512, which is the storage capacity of
a sector).  The first six bytes of each directory entry is the file
name.  The remaining 26 bytes are sector numbers, which tell where the
file is on the disk.  If the first byte of the entry is 0x0, then
there is no file at that entry.

For example, a file entry of:


    4B 45 52 4E 45 4C 03 04 05 06 00 00 00 00 00 00 00 00 00 00...
    K  E  R  N  E  L


means that this is a valid file, with name ``KERNEL'', located at
sectors 3, 4, 5, 6.  (00 is not a valid sector number but a
filler since every entry must be 32 bytes).

If a file name is less than 6 bytes, the remainder of the 6 bytes
should be filled with 00s.

Note that this file system is very restrictive.  Since one byte
represents a sector, there can be no more than 256 sectors used on the
disk (128KB of storage).  Additionally, since a file can have no more
than 26 sectors, file sizes are limited to 13KB.  For this project,
this is adequate storage, but for a modern operating system, this
would be grossly inadequate.


# Initial Map and Directory

The new files map.img and dir.img contain a
Map and Directory for a file system consisting of
only the kernel.  Modify your Makefile to include the
following lines in the appropriate place:

    dd if=map.img of=floppya.img bs=512 count=1 seek=1 conv=notrunc
    dd if=dir.img of=floppya.img bs=512 count=1 seek=2 conv=notrunc


This sets up your initial file system.


# loadFile

You are provided with a utility loadFile.c, which can be
compiled with gcc (e.g. gcc -o loadFile loadFile.c).
loadFile reads a file and writes it to floppya.img,
modifying the Map and Directory appropriately.  For
example, to copy message.txt from the previous milestone to
the file system, type:

    ./loadFile message.txt

This saves the trouble of using dd and modifying the
Map and Directory.

Note that "message.txt" is more than six letters.  loadFile
will truncate the name to six letters on loading it.  The file name
will become "messag".  Note also that unless otherwise indicated,
all implementation must be done in kernel.c.


# Load A File And Print It

Create a new function readFile that takes a character array containing
a file name and reads the file into a buffer.  After completing the
readFile function, make it an interrupt 0x21 call:


Read File:
* AX = 3
* BX = address of character array containing the file name
* CX = address of a buffer to hold the file


The readFile function should work as follows:

1. Load the directory sector into a 512 byte character array using
  readSector.
2. Go through the directory trying to match the file name.  If you do
  not find it, return.
3. If found, using the sector numbers in the directory, load the file,
  sector by sector, into the buffer array.  You should add 512 to the
  buffer address every time you call readSector.
4. Return.


# Testing

Use the readFile function to read in a text file and print it
out.  You can use the message.txt file from the previous
milestone.

In main:

    char buffer[13312]  /* this is the maximum size of a file */
    makeInterrupt21();

    interrupt(0x21, 3, “messag\0”, buffer, 0);  /* read the file into buffer */
    interrupt(0x21, 0, buffer, 0, 0);	    /* print out the file */

    while(1);			            /* hang */


Then:

    ./loadFile message.txt


It is also a good idea to test with a file of more than 512 bytes,
because there are often bugs that only surface when you try and load
more than one block.  I've included bigmess.txt for this purpose.

# Load A Program And Execute It

The next step is to load a program into memory and execute it.  This
really consists of four steps:

1. Loading the program into a buffer (a big character array).
2. Transferring the program into the bottom of the segment where
  you want it to run.
3. Setting the segment registers to that segment and setting the
  stack pointer to the program's stack.
4. Jumping to the program.


To try this out, you are provided with a test program tstprg.
Write a function to load tstprg into memory and start it
running.  Then, after compiling, use loadFile to load
tstprg into floppya.img.

Write a new executeProgram function that takes as a parameter
the name of the program you want to run (as a character array) and the
segment where you want it to run (e.g. void
  executeProgram(char* name, int segment))

The segment should be a multiple of 0x1000 (remember that a
segment of 0x1000 means a base memory location of 0x10000).
0x0000 should not be used because it is reserved for
interrupt vectors.  0x1000 also should not be used because
your kernel lives there and you do not want to overwrite it.  Segments
above 0xA000 are unavailable because the original IBM-PC was
limited to 640k of memory.  (Memory, incidentally, begins again at
address 0x100000, but you cannot address this in 16-bit real
mode.  This is why all modern operating systems run in at least 32-bit
protected mode).

Your function should do the following:

  1. Call readFile to load the file into a buffer.
  2. In a loop, transfer the file from the buffer into the start
  (0000) of memory at the segment in the parameter.  You should use
  putInMemory to do this.
  3. Call the assembly function launchProgram, which takes the segment
  number as a parameter (e.g. void launchProgram(int segment)).  This
  is because setting the registers cannot be done in C.  The assembly
  function will set up the registers and jump to the program.  The
  thread of execution will never return from this function.


Finally, make your function a new interrupt 0x21 call, using
the following:

Load program and execute it:
* AX = 4
* BX = address of character array holding the name of the program
* CX = segment in memory to put the program


# A Warning

When your program calls the kernel via an interrupt, it's very
important that the kernel code not use any kernel global variables.
In particular, you should NOT have your kernel code use any double
quoted strings - these are stored as globals, even if you declare them
in a function.  This can make your life hard, as it means that
printing debugging messages from the kernel usually requires strings.

The solution is to declare strings as character arrays in the function.


    char abc[7];
    abc[0]= ‘s’;
    abc[1]= ‘t’;
    abc[2]= ‘r’;
    abc[3]= ‘i’;
    abc[4]= ‘n’;
    abc[5]= ‘g’;
    abc[6]= ‘\0’;


# Testing

In main:

    makeInterrupt21();
    interrupt(0x21, 4, “tstprg\0”, 0x2000, 0);
    while(1);


If your interrupt works and tstprg runs, your kernel will
never make it to the while(1).  Instead, the tstprg
will print out a message and then hang.


# Terminate Program System Call

This step is simple but essential.  When a user program finishes, it
should make an interupt 0x21 call to return to the operating
system.  This call terminates the program.  For now, you should just
have a terminate call hang up the computer, though you will soon
change it to make the system reload the shell.

Make a terminate function, which for now contains an infinite
while loop.

Next, make an interrupt 0x21 to terminate a program.  It
should be defined as:

Terminate program:
* AX = 5

Verify with the tstpr2 program provided.  Unlike
tstprg, tstpr2 does not hang up at the end but calls
the terminate program interrupt.


# The Shell - Making Your Own User Program

You now are ready to make the shell!  Use the file lib.asm.
It contains a single assembly language function: interrupt.
Your shell should not need any more assembly functions since all low
level functions are provided by the kernel.

Your shell should be called shell.c and should be compiled
the same way the kernel is compiled.  However, in this case, you will
need to assemble lib.asm intead of kernel.asm and
link lib.o instead of kernel\_asm.o.  Your final file
should be called ``shell''.

After you compile the shell, use loadFile to load the shell
onto floppya.img.  You should add all of these commands to
your Makefile.

Your initial shell should run in an infinite loop.  On each iteration,
it should print a prompt ("SHELL> "  or "A:> " or
something like that).  It should then read in a line and try to match
that line to a command.  If it is not a valid command, it should print
an error message (e.g. ``Bad Command!'' or something similar)
and prompt again.  Since you do not have any shell commands yet,
anything typed in should cause an error message to be printed.

All input/output in your shell should be implemented using
interrupt 0x21 calls.  You should not rewrite or reuse any of
the kernel functions.  This makes the OS modular: if you want to
change the way things are printed to the screen, you only need to
change the kernel, not the shell or any other user program.

## Kernel adjustments

In your kernel:

  1. Modify main so that it simply sets up the
  interrupt 0x21 using makeInterrupt21, and calls an
  interrupt 0x21 to load and execute "shell" at
  segment 0x2000.
  2. Modify terminate so that it no longer hangs.  Instead
  it should use interrupt 0x21 to reload and execute
  ``shell'' at segment 0x2000.



## Shell Command "type"

Modify your shell to recognize the command "type filename".
If the user types "type messag" at the shell prompt, the
shell should load messag into memory and print out the
contents.  You should implement this using interrupt 0x21
calls.


## Shell Command "execute"

Modify your shell to recognize the command "execute
  filename".  If the user types "execute tstpr2" at the
shell prompt, the shell should call the interrupt 0x21 to
load and execute tstpr2 at segment 0x2000
(overwriting the shell).

Test this by typing "execute tstpr2" at the shell prompt.
If you are successful, tstpr2 should run, print out its message, and
then you should get a shell prompt again.

## Shell Command "dir"

Write a shell command "dir".  This command should print out
the files in the directory. 

# Write Sector

Create a writeSector function in kernel.c to go with
the readSector function.  Writing sectors is provided by the
same BIOS call as reading sectors, and is almost identical.  The only
difference is that AH should equal 3 instead of 2.  Your
write sector function should be added to interrupt 0x21, and
should be handled as follows:


Write Sector
* AX = 6
* BX = address of a character array holding the sector to write
* CX = the sector number

If you implemented readSector correctly, this step should be
very simple.

# Writing A File

Add a writeFile function to the kernel that takes three
arguments, a character array holding the file name, a character array
holding the file contents, and the number of sectors to be written and
writes a file to the disk (e.g. void writeFile(char* name,
  char* buffer, int numberOfSectors)).  Then add writeFile
to interrupt 0x21 as follows:

Write file:
* AX = 8
* BX = address of character array holding the file name
* CX = address of character array holding the file to be written
* DX = number of sectors


Your function should:

* Load the Map and Directory sectors into separate buffers
* Find a free directory entry (i.e. one that begins with 0x00)
* Copy the first six characters of the name to that directory
  entry.  If the name is less than six bytes, fill in the remaining
  bytes with 0x00
* For each sector making up the file:
  
    1. Find a free sector by searching through the Map for a 0x00
    2. Set that sector to 0xFF in the Map
    3. Add that sector number to the file's directory entry
    4. Write 512 bytes from the buffer holding the file to that sector

* Fill in the remaining bytes in the directory entry to 0x00
* Write the Map and Directory sectors back to the disk


If there are no free directory entries or insufficient free sectors,
your writeFile function should just return.

# Copying A File

Write a copy command for the shell.  The copy command should
have the syntax "copy filename1 filename2".  Without
deleting filename1, the copy command should create a file with name
filename2 and copy all the bytes of filename1 to filename2.  Your copy
command should use only interrupt 0x21s for reading and
writing files.

You can test this by loading message.txt onto
floppya.img.  At the shell prompt, type "copy messag
  m2".  Then type "type m2".  If the contents of
message.txt print, your copy function most likely works.

You should check the directory and map in floppya.img using
hexedit after copying to verify that your writing function
works correctly.

If your copy function works correctly, you should be able to copy
shell to another file.  Then try executing the duplicate
shell.  If you get the shell prompt, it works correctly.

It's a good idea to test your copy with a file of more that 512 bytes
(try bigmess.txt).

# Turning It In

Be sure to submit your updated kernel.c and Makefile along with all of
 the files necessary to build and run your OS to your team repository
 in the the Filesystem folder.
 
Also ensure that your Makefile includes both message.txt and
bigmess.txt in the floppy image when built in a standard way.
 
# Rubric

Total 160 points

| Part                                                                                                  | Point Value |
|:------------------------------------------------------------------------------------------------------|-------------|
| readFile (see note below)                                                                             | 25          |
| Invokes shell and boot which responds to user input (be sure shell is a seperate program from kernel) | 25          |
| Type command works (be sure to test it with bigmess.txt)                                              | 30          |
| Execute command works (-10 if it does not return to shell after)                                      | 25          |
| Dir command works                                                                                     | 25          |
| Copy command works (be sure to test it with bigmess.txt)                                              | 30          |

Note: normally the TA's wont test readfile directly, because the shell
itself and type command can prove it works correctly.  However, if you
can't get the later steps working but readfile works you can put the
file dumping code in the kernel and at least get those 25 points.
