---
layout: post
title: Lab 00 -- C Review
readtime: true
date: Sat Oct 29 20:08:27 2022 
---


<!-- vim-markdown-toc GFM -->

* [Introduction](#introduction)
  * [Learning Objectives](#learning-objectives)
  * [The xv6 Operating System](#the-xv6-operating-system)
* [Startup: Building xv6](#startup-building-xv6)
  * [Installing prerequisites](#installing-prerequisites)
  * [Getting the source code](#getting-the-source-code)
  * [Booting xv6](#booting-xv6)
  * [Troubleshooting](#troubleshooting)
* [Exercises](#exercises)
  * [Warmup](#warmup)
    * [Running the grading script](#running-the-grading-script)
  * [ArrayList](#arraylist)
    * [Testing](#testing)
    * [Running the grading script](#running-the-grading-script-1)
  * [Implementing `sleep`](#implementing-sleep)
    * [Implementation plan](#implementation-plan)
    * [Building and testing](#building-and-testing)
    * [Running the grading script](#running-the-grading-script-2)
  * [Implementing `find`](#implementing-find)
    * [Implementation plan](#implementation-plan-1)
    * [Building and testing](#building-and-testing-1)
    * [Running the grading script](#running-the-grading-script-3)
  * [Running the full grading script](#running-the-full-grading-script)
  * [Submitting your code](#submitting-your-code)

<!-- vim-markdown-toc -->

# Introduction

It is crucial for this class that you have a solid foundation in C programming,
as it is most widely used language for systems programming (though now facing a
relatively decent challenge from Rust). Therefore, we have prepare a set of
practice problems for you to solve and refresh your memory when it comes to C
programming concepts, as well as introduce some minor new concepts like making
system calls and parsing command line arguments. 

## Learning Objectives

At the end of the lab assignment, you should be able to:
+ Implement basic pointer manipulation operations in C.
+ Implement basic structures and structure creation/manipulation operations. 
+ Compile and boot the xv6 operating system.
+ Implement standard Unix utilities in the xv6 operating system.

## The xv6 Operating System

xv6 is a simple operating system, created by MIT faculty and students, to
serve as a teaching tool for operating systems classes. xv6 is based on Dennis
Ritchie's and Ken Thompson's Unix Version 6 (v6), and is implemented in C to run
on a multi-core RISC-V (virtual) machine.

One of the main features of xv6 is that its code is very readable for an
operating system. The accompanying book is a great resource to understand how a
basic operating system functions, handles system calls, interrupts, memory
management, etc.  In this class, we will use xv6 as a toy operating system to
implement several tools and features that we discuss in class.

# Startup: Building xv6

Before we get started, let's install the needed software requirements. xv6 runs
in a virtual environment that uses the `qemu` RISC-V emulator. Note that we have
tested these instructions on Ubuntu 20.04 LTS as well as WSL2 on Windows. We
currently do not plan on supporting MacOS machines though that is possible with
some help from your friend Google.

## Installing prerequisites

1. First, start by updating your distribution:
```shell
$ sudo apt update && sudo apt upgrade
```

2. Second, install the needed dependencies:
```shell
$ sudo apt install -y build-essential gdb-multiarch qemu-system-misc \
    gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu
```

## Getting the source code

Obtain the xv6 modified source code by cloning the following repository. **Make
sure you select the `lab00` branch.**
```shell
git clone -b lab00 https://github.com/rhit-csse332/xv6-riscv-public.git
```

## Booting xv6

Now we are ready to compile xv6 and launch `qemu` to boot into a virtual machine
that is running the xv6 operating system. From the root directory of the cloned
source code, compile and launch `qemu` using
```shell
make qemu
```

On my machine, I had the following output, your output should match mine and you
should drop into a shell in the `qemu` machine.
```shell
$ make qemu
>> Lots of compilation outputs, should be no warnings and no errors <<
xv6 kernel is booting

hart 2 starting
hart 1 starting
init: starting sh
$
```

If you get to this point, you are ready to roll. Try to play around in the xv6
shell and then when you are ready to exit, press <ctrl - a> then x, i.e., press
the control key plus `a` at the same time, then press `x`. Happy hacking!

## Troubleshooting

If the compilation fails, make sure to record any error messages your received.
Before contacting your instructor, make sure you record the versions of all of
the tools that are need to compile xv6. You can get those version by running the
following commands:
1. Your kernel version:
```shell
$ uname -r
```
2. You `qemu` system version:
```shell
$ qemu-system-riscv64 --version
QEMU emulator version 4.2.1 (Debian 1:4.2-3ubuntu6.18)
Copyright (c) 2003-2019 Fabrice Bellard and the QEMU Project developers
```
3. Your RISC-V cross compiler version:
```shell
$ riscv64-linux-gnu-gcc --version
riscv64-linux-gnu-gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
Copyright (C) 2019 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

Once you have the above information, make a public post on [CampusWire]({{
site.campuswire_url }}) and include all the information above plus any error
messages that came up during compilation. One of the instructors or TAs will
get back to you as soon as possible. 

# Exercises

There are four sets of exercises in this lab, that range in difficulty from
straightforward to medium difficulty. Since this is a review lab, there will be
no really challenging tasks. Expect those to show in the following labs. 

## Warmup

The first set of exercises you will solve are warmup exercises that are fairly
easy and straightforward to solve. Checkout the code in `user/warmup.c` for
instructions on how to solve these problems. The source code in `wamup.c`
contains test cases that will help you test your code and debug in the case of a
failed implementation. 

To run the warmup code, you can use the following instructions:
1. Compile the `xv6` source code using
```shell
$ make qemu
```
2. In the `xv6` shell, issue the `warmup` command using
```shell
$ warmup
```

- Note that running `warmup` without any arguments will run **all** of the test
cases. To run a specific test case, you can use `warmup <num>` when `<num>` is
an integer that corresponds to the test case you want to run. Check out the
`main` function `warmup.c` for the mapping between numbers and test cases. 
- Note that assertions in the test cases written will cause your program to
fail. 

### Running the grading script

To double check your work, you can run the grading script (assuming you have
`python3` installed) from your Linux shell (not the xv6 shell):
```shell
$ ./grade-lab-0.py warmup
```

## ArrayList

The next set of exercises will have you implement an array list data structure
in C in the xv6 operating system. Recall that an array list doubles in size
every time it runs out of space. We have already defined a structure that
contains the elements needed to implement an array list in the `arraylist`
structure as follows:
```c
struct arraylist {
  int size;
  int capacity;
  int *list;
};
```
where `size` is the current size of the list (i.e., the total number of elements
currently in the array list), `capacity` is the maximum size of the array list,
and `list` is the actual list in memory that contains the elements. 

1. The first function you are to implement is the `al_new` function, which takes
   no arguments and returns a new array list structure initialized to a `size`
   of 0, a `capacity` of `DEF_ARRAY_LIST_CAPACITY`, and an array of size
   `capacity` allocated in memory (It is okay if the allocated memory area
   contains garbage data, you do not need to initialize it). 

2. The second function you are to implement is the `al_free` function, which
   takes a pointer to an array list structure and frees that array list. Note
   that you must also free the inner list in case it exists. 

3. The third function to implement is the `al_insert_at` function. It takes as
   input a pointer to an array list structure, a position `pos` to insert at,
   and a value `val` to insert, and then replaces the element at `pos` (if it
   exists), with the value `val`. 

   **WARNING** This function does not increase the size of the array list,
   rather it only replaces an existing element. If an element at the input
   position does not exists, the function simply does nothing and returns. 

4. The next function to implement is the `al_get_at` function. It takes as input
   a pointer to an array list structure and a position `pos`. It returns the
   element in the list at the position `pos`. If such an element does not
   exists, then the function returns the hex constant `0xffffffff`. 

5. Next, implement the `al_resize` function. It accepts as input a pointer to an
   array list structure and resizes the inner array to double its size. Note
   that you do no have access to the `realloc` routine, you must implement that
   yourself. 

6. Finally, implement the `al_append` function that accepts a pointer to an
   array list structure and a value `val`. It then inserts `val` at the end of
   the list. If the list is full, it then calls `al_resize` to make room for
   `val` and then inserts it into the list. 

7. For your convenience, we have implemented `al_print` to print an array list.
   Use that to print your list for debugging purposes. 

### Testing

To test your code, after each step, launch the xv6 machine and then issue the
`arraylist` command:
```shell
$ arraylist
```

Note that the numbers between the parenthesis correspond to the line numbers in
your code. So if an assertion fails, you can go to that specific line number and
check which test has failed. 

As you implement more functions, more tests will pass. At the end, you should
see something that looks like the following:
```shell
$ arraylist
main(174): OK.
main(175): OK.
main(176): OK.
main(181): OK.
main(182): OK.
main(185): OK.
main(186): OK.
main(187): OK.
main(192): OK.
main(193): OK.
main(197): OK.
main(198): OK.
main(201): OK.
main(202): OK.
main(203): OK.
main(204): OK.
main(205): OK.
```

### Running the grading script

To double check your work, run the grading script from your Linux terminal (not
your xv6 terminal), using:
```shell
$ ./grade-lab-0.py arraylist
make: 'kernel/kernel' is up to date.
== Test arraylist, all == arraylist, all: OK (1.5s)
```

## Implementing `sleep`

Before getting started, quickly flip through Chapter 1 of the [xv6 book]({{
site.xv6_book_url }}); it serves as a good introduction to the operating
system and its interface.

The goal of this exercise is to implement the UNIX utility `sleep`, which causes
the current terminal to pause for a user-specified number of ticks. In xv6, a
tick is defined as the time between two interrupts from the `qemu` timer chip.
For example, if I issue the command `sleep 10` from the terminal, the whole
terminal would pause until 10 ticks have passed. You can examine the behavior of
`sleep` in your current Linux terminal by typing `man sleep`.

All user utilities in xv6 will go into the `user/` directory. In there you will
find examples of other utilities, such as `ls`, `grep`, `mkdir`, `rm`, etc.
Examining the code behind some of those utilities will prove to be of immense
help in this project.

### Implementation plan

Here's a list of steps and requirements that will help you implement the `sleep`
utility in xv6:
- We have created a file named `sleep.c` for you in the `user/` directory.
  - The `sleep.c` includes the necessary header files and has an empty `main`
  function for you to fill out.
- In `main(int argc, char **argv)`, `argc` is the argument count and `argv` is
	the argument vector. Recall that `argv[0]` is always the name of the program
	being run, so always `argc >= 1`.
- If the user forgets to pass an argument, your program **should** output an
	error message and instruct the user on proper usage of the utility.
- Command line arguments are passed as strings, you can use `atoi` to convert a
	string into an integer. Checkout `user/lib.c` for more information about the
	available `libc` routines.
- Use the system call `sleep`.
	- Take a look at `user/user.h` to see the definition of `sleep` that is
		callable from user-space.
	- If you want to take a look at the actual system call, then examine
		`kernel/sysproc.c` and look for `sys_sleep`.
- Make sure that your program calls `exit()` in order to exit your program.

### Building and testing

We have already added the sleep binary to the list of targets to compile by the
`Makefile` so you do not need to change anything there. 

Compile xv6 and launch into the `qemu` emulator using
```shell
$ make qemu
```
then run your `sleep` program from the xv6 shell:
```shell
$ sleep 10
### Nothing happens for 10 ticks ###
$
```

### Running the grading script

To double check your work, you can run the python grading script (assuming you
have `python3` installed):
```shell
$ ./grade-lab-0.py sleep
```

## Implementing `find`

The goal of this exercise is to implement the UNIX `find` program, which, given
a file name, finds all the files in a certain directory (and its
sub-directories) that match that file name.

For example, consider a directory `a/` that contains a file called `b` and then
another directory called `c/` that contains another file called `b` as well.
Then launching the `find` command from the `a` directory will return both `./b`
and `./c/b` as results.
```shell
$ cd a/
$ find . b
./b
./c/b
```

### Implementation plan

Since we are exploring directories and sub-directories, expect your solution to
be **recursive**. Also, recall that we are dealing with C strings, so `==` does
not compare strings, rather it compares their pointers. To compare strings in C,
use the routine `strcmp`. For more information, check out its man page `man
strcmp`.

For inspiration to get started, take a look at `user/ls.c` to see how to read
files and directories.

Here are a couple of hints and observations:
- You might find the system call `fstat` and the utility routine `stat` useful.
- Running `fstat` on a file will fill out a `struct stat` structure, which
	contains a lot of useful information about the file/directory. You can find
	the definition of the `struct stat` below and in `kernel/stat.h`
	```c
	#define T_DIR     1   // Directory
	#define T_FILE    2   // File
	#define T_DEVICE  3   // Device

	struct stat {
		int dev;     // File system's disk device
		uint ino;    // Inode number
		short type;  // Type of file
		short nlink; // Number of links to file
		uint64 size; // Size of file in bytes
	};
	```
- Use recursion to descend into sub-directories, but **do not** recurse into `.`
	and `..`
- A directory is nothing but a file that contains a bunch of directory entries,
  or dentries. In xv6, they are represented as `struct dirent` structures.
- Remember to `\0` terminate your strings before using the `strlen` routine.

### Building and testing

Compile xv6 using `make qemu` from your terminal, and then, from the xv6 shell:
```shell
$ echo > b
$ mkdir a
$ echo > a/b
$ find . b
./b
./a/b
$
```

### Running the grading script

To double check your work, you can run the python grading script using:
```shell
$ ./grade-lab-0.py find
```

## Running the full grading script

Once you are done implementing all the above programs, run the grading script
using
```shell
$ make grade
```
from your Linux terminal (not your xv6 terminal window). 

## Submitting your code

From the Linux terminal, issue the command:
```shell
make submit
```
Two files will be generated, `submit-lab0.patch` and `submit-lab0.tar`. **Please
submit both of these files to the Gradescope** at the appropriate lab link.
