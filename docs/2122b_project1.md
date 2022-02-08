---
layout: post
title: xv6 Project Introduction
readtime: true
date:
gh-repo: rhit-csse332/csse332
gh-badge: [star,watch,follow]
---

# Introduction

xv6 is a simple operating system, created by MIT faculty and students, to
serve as a teaching tool for operating systems classes. xv6 is based on Dennis
Ritchie's and Ken Thompson's Unix Version 6 (v6), and is implemented in C to run
on a multi-core RISCV (virtual) machine.

One of the main features of xv6 is that its code is very readable for an
operating system. The accompanying book is a great resource to understand how a
basic operating system functions, handles system calls, interrupts, memory
management, etc.

In this class, we will use xv6 as a toy operating system to implement several
tools and features that we discussed in class. Specifically, we will implement
new system calls, new user utilities, modify the kernel's representation of a
process, and introduce a more sophisticated scheduler.

# Table of contents


* [Building xv6](#building-xv6)
  * [Installing prerequisites](#installing-prerequisites)
  * [Getting the source code](#getting-the-source-code)
  * [Booting xv6](#booting-xv6)
  * [Troubleshooting](#troubleshooting)
* [Exercises](#exercises)
  * [Implementing `sleep`](#implementing-sleep)
    * [Implementation plan](#implementation-plan)
    * [Building and testing](#building-and-testing)
  * [Implementing `pingpong`](#implementing-pingpong)
    * [Implementation plan](#implementation-plan-1)
    * [Building and testing](#building-and-testing-1)
  * [System call tracing using `trace`](#system-call-tracing-using-trace)
    * [Implementation plan](#implementation-plan-3)
  * [Implementing the `ps` utility](#implementing-the-ps-utility)
    * [Implementation plan](#implementation-plan-4)
    * [Building and testing](#building-and-testing-3)
* [Acknowledgements](#acknowledgements)



# Building xv6

Before we get started, let's install the needed software requirements. xv6 runs
in a virtual environment that uses the `qemu` RISCV emulator.

** Note that we created the instructions below using Ubuntu 20.04 LTS, and they
work equally well on WSL2 on Windows machines. For MacOS installation, please
contact your instructor.**

## Installing prerequisites
1. First, start by updating your distribution:
	```shell
	$ sudo apt update && sudo apt upgrade
	```
2. Second, install the needed dependencies:
	```shell
	$ sudo apt install -y it build-essential gdb-multiarch qemu-system-misc gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu
	```

## Getting the source code

Obtain the xv6 modified source code by cloning the following repository
```shell
$ git clone git@github.com:rhit-csse332/xv6-riscv-public.git
```

## Booting xv6

Now we are ready to compile xv6 and launch `qemu` to boot into a system that is
running the operating system.

From the top directory of the cloned source code, compile and launch `qemu`
using
```shell
$ make qemu
```
On my machine, I had the following output, your output should match mine and you
should drop into a shell in the `qemu` machine.
```shell
$ make qemu
# Lots of compilation outputs, should be no warnings and no errors
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

If the compilation fails, make sure to record any error messages you received.
Before contacting your instructor, make sure you record the versions of all the
tools that are needed. On my machine, the versions are (along with the way to
get them):
```shell
$ qemu-system-riscv64 --version
QEMU emulator version 4.2.1 (Debian 1:4.2-3ubuntu6.18)
Copyright (c) 2003-2019 Fabrice Bellard and the QEMU Project developers
```

```shell
$ riscv64-linux-gnu-gcc --version
riscv64-linux-gnu-gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
Copyright (C) 2019 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

Once you have this information, contact your instructor with the output of the
above commands as well as any error messages you received when compiling. Or
even better, post a question in CampusWire.

# Exercises

## Implementing `sleep`

Before getting started, reach Chapter 1 of the [xv6
book](https://pdos.csail.mit.edu/6.828/2021/xv6/book-riscv-rev2.pdf); it serves
as a good introduction to the operating system and its interface.

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
- In `main(int argc, char *argv[])`, `argc` is the argument count and `argv` is
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

To add your utility to the list of available xv6 utilities, add your `sleep`
program to `UPROGS` in the `Makefile` under the top xv6 directory. The
content of your `Makefile` should look something like:
```
UPROGS=\
	$U/_cat\
	$U/_echo\
	$U/_forktest\
	$U/_grep\
	$U/_init\
	$U/_kill\
	$U/_ln\
	$U/_ls\
	$U/_mkdir\
	$U/_rm\
	$U/_sh\
	$U/_stressfs\
	$U/_usertests\
	$U/_grind\
	$U/_wc\
	$U/_zombie\
	$U/_sleep\
```

Then compile xv6 and launch into the `qemu` emulator using
```shell
$ make qemu
```
then run your `sleep` program from the xv6 shell:
```shell
$ sleep 10
----> Nothing happens for 10 ticks <----
$
```

## Implementing `pingpong`

The goal of this exercise is to create a program, called `pingpong`, that uses a
bunch of UNIX system calls to send a byte of data back and forth between two
processes, a parent and a child, over a pair of pipes.

The parent should start by writing a byte to the child. Once received, the child
should print out `<pid>: received ping`, where `<pid>` is the process ID of the
child. Then, the child should write the same byte to the parent and exit
successfully. On the other end, the parent should receive the byte and then
print `<pid>: received pong` where `<pid>` is the parent's process ID, and exit
successfully.

Your solution should placed in the file `user/pingpong.c`.

### Implementation plan

First, checkout `user/user.h` to find the list of system calls and utility
routines that you have access to. System calls are implemented in the kernel
while the utilities are implemented in `user/ulib.c`, `user/printf.c`, and
`user/umalloc.c`. Feel free to explore the code based to understand a few
examples.

Second, try to create a child process using the `fork` system call. Make sure to
terminate both the parent and the child appropriately in order to avoid creating
orphan or zombie processes.

Third, to find the process ID of a certain process, you can use the `getpid`
system call.

Finally, use the `pipe` system to call create pipes between the two processes
to exchange the bytes. **Recall that UNIX pipes are unidirectional**.

### Building and testing

Remember to add your program to `UPROGS` in the top level `Makefile`. Compile
xv6 using `make qemu` from your terminal, and then, from the xv6 shell:
```shell
$ pingpong
4: received ping
3: received pong
$
```

<!--
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
	```C
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
- Recall that a directory is nothing but a file that contains a bunch of
	directory entries, or dentries. In xv6, they are represented as `struct
	dirent` structures.
- Remember to NULL terminate your strings before using the `strlen` routine.

### Building and testing

Remember to add your program to `UPROGS` in the top level `Makefile`. Compile
xv6 using `make qemu` from your terminal, and then, from the xv6 shell:
```shell
$ echo > b
$ mkdir a
$ echo > a/b
$ find . b
./b
./a/b
$
```
-->

## System call tracing using `trace`

In the previous three exercises, we have been using provided system calls to
implement UNIX utilities. In this exercise and the following, we will add a new
system call and implement an UNIX utility to interface with it.

As a head start, read Chapter 2 of the [xv6
book](https://pdos.csail.mit.edu/6.828/2021/xv6/book-riscv-rev2.pdf) as well as
Sections 4.3 and 4.4 of Chapter 4. For your information, the user-space code for
system calls is in `user/user.h` and `user/usys.pl`. The kernel-space code is in
`kernel/syscall.h` and `kernel/syscall.c`, while the process-related code is in
`kernel/proc.h` and `kernel/proc.c`.

The goal of this exercise is to implement the `trace` system call that controls
system call tracing in the xv6 kernel, and then using that system call in an
UNIX utility to trace system calls for a specific process or set of related
processes (i.e., a process and its children and so on). System call tracing
allows you to print the sequence of system calls that a process makes throughout
its lifetime. For an example of system call tracing, check out the `strace`
utility. For example, running `strace` on my machine (not xv6) gives you the
following:
```shell
$ strace head -c 1 /dev/urandom
execve("/usr/bin/head", ["head", "-c1", "/dev/urandom"], 0x7fffb4936400 /* 59 vars */) = 0
brk(NULL)                               = 0x55859615d000
arch_prctl(0x3001 /* ARCH_??? */, 0x7ffe2337c2b0) = -1 EINVAL (Invalid argument)
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=85359, ...}) = 0
mmap(NULL, 85359, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7fa542d48000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\360q\2\0\0\0\0\0"..., 832) = 832
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
pread64(3, "\4\0\0\0\20\0\0\0\5\0\0\0GNU\0\2\0\0\300\4\0\0\0\3\0\0\0\0\0\0\0", 32, 848) = 32
pread64(3, "\4\0\0\0\24\0\0\0\3\0\0\0GNU\0\t\233\222%\274\260\320\31\331\326\10\204\276X>\263"..., 68, 880) = 68
...
...
openat(AT_FDCWD, "/dev/urandom", O_RDONLY) = 3
read(3, "\255", 1)                      = 1
fstat(1, {st_mode=S_IFCHR|0620, st_rdev=makedev(0x88, 0x1), ...}) = 0
close(3)                                = 0
write(1, "\255", 1)                     = 1
close(1)                                = 0
close(2)                                = 0
exit_group(0)                           = ?
+++ exited with 0 +++
```

In our implementation, the `trace` system call accepts a single argument, an
integer `mask`, that determines which system calls the user wishes to trace. For
example, if the `mask` is `1 << SYS_fork`, then only the `fork` system call is
being traced (all other system calls are ignored by tracing). `SYS_fork` is the
system call number (syscall number) for `fork` as defined in `kernel/syscall.h`.

Your job is to modify the xv6 **kernel** to print out a line at the end of each
traced system call, i.e., print those system calls that are executed and that
are enabled by the `mask` argument. Each line you print should contain the
process ID, the name of system call being traced, and the return value of the
system call.

**The trace system call should enable tracing and set the mask for the calling
process and all of its subsequent children that it forks, but should not affect
any other processes in the system**.

We have provided you with a `trace` UNIX utility that you can use to test your
code. Once you are done with this exercise,  your output should look like the
following (from the xv6 shell):
```shell
$ trace 32 grep hello README
3: syscall read -> 1023
3: syscall read -> 966
3: syscall read -> 70
3: syscall read -> 0
$
```
where 32 is nothing but (1 << 5) as from `kernel/syscall.h` we know that
`SYS_read` is 5. In other words, we are only tracing the read system calls in
this case.

To trace all system calls, that `mask` should be `2147483647` to be a 32 bit
integer of all 1's. The output of the `trace` utility in that case would be:
```shell
$ trace 2147483647 grep hello README
4: syscall trace -> 0
4: syscall exec -> 3
4: syscall open -> 3
4: syscall read -> 1023
4: syscall read -> 966
4: syscall read -> 70
4: syscall read -> 0
4: syscall close -> 0
$
```

If a process is not called from the `trace` utility, you should not see any
lines being printed:
```shell
$ grep hello README
$
```

Finally, `trace` controls should be propagated to child processes created using
the `fork` system call. To test this, you can use the `usertests` program that
we provide you with in the `user/` directory. You can run the tests as follows:
```shell
$ trace 2 usertests forkforkfork
usertests starting
test forkforkfork: 407: syscall fork -> 408
408: syscall fork -> 409
409: syscall fork -> 410
410: syscall fork -> 411
409: syscall fork -> 412
410: syscall fork -> 413
409: syscall fork -> 414
411: syscall fork -> 415
...
$
```
Note that you might see different process IDs than the one we show here.

### Implementation plan

The first step you must do is to add the `trace` UNIX utility to `UPROGS` in the
`Makefile` at the top-level directory. If you try to compile the code now, it
will fail with errors related to the `trace` utility, most importantly, we have
not defined nor implemented the `trace` system call yet.

For adding the stubs for the system call, proceed as follows:
1. Add a prototype for `trace` in `user/user.h` as follows:
	```c
	int trace(int);
	```
2. Add a stub for the system call in `user/usys.pl` as follows:
	```perl
	entry("trace");
	```
3. Add a system call number to `kernel/syscall.h` as follows:
	```c
    #define SYS_trace 22
	```
	Once you fix these compilation issues, your code should compile successfully now
	and you should be able to boot into the xv6 kernel. If you run the `trace`
	utility at this point, it will fail with an error message, but the kernel will
	not crash.

Since we need to remember the mask for each process in the system, we need to
add the mask to each process' process control block (PCB). You can find the
definition of the PCB in `kernel/proc.h`. Therefore, add an integer `trace_mask`
to the `struct proc` definition in `kernel/proc.h`.

Next, add a function `sys_trace` to `kernel/sysproc.c`. This function will
implement the system call that will set the mask for the process that calls the
system call.
```c
uint64
sys_trace(void)
{
	/* your code goes here */
}
```

Here are a few hints to help you implement this function:
- You can use the routine `myproc` to get the currently running process (i.e.,
  the process that called the system call).
- You might have noticed that `sys_trace` does not accept any arguments, so
  where do our arguments come from? Hopefully, you've read the chapter on system
	calls in the book and have realized that they are passed in the processor
	registers. To fetch the data from the registers, you can use the functions
	defined in `kernel/syscall.c`. Specifically, you might find the routine
	`argint` useful.

Next, we need to make sure that the masks are transferred from parent to child.
For that purpose, modify the `fork()` in `kernel/proc.c` to copy the trace mask
from the parent to the child.

Finally, modify the `syscall()` function in `kernel/syscall.c` to print the
tracing output. You might find it useful to map each syscall number of an
equivalent name to that you can print easily.

## Implementing the `ps` utility

The goal of this exercise is to implement the UNIX utility `ps`, which collects
and formats information about the current processes in the system. You can try
it out for yourself in any UNIX-based distribution, for example, try running `ps
-a` to see the list of processes that are controlled by a terminal.

To that end, you will add a `pinfo` system call that collects a bunch of
information about each process in the system, and stores them in a `struct
proc_info` data structure that we provide. You can find the definition of that
structure below and in `kernel/ps.h`.
```c
#define MAX_PS_PROC 64
struct proc_info {
  int active[MAX_PS_PROC];            // active/inactive process
  int pid[MAX_PS_PROC];               // the pid of each active process
  int states[MAX_PS_PROC];            // the state of each active process
  int num_mapped_pages[MAX_PS_PROC];  // the number of mapped pages per process
  char name[MAX_PS_PROC][16];         // the name of each process if any
};
```
Here is the breakdown of the fields in this structure:
- `active` is a boolean flag that is set to 1 if the process is allocated and
	active. Since there is a static number of processes in xv6, we need a flag to
	tell us whether to read that process entry or not.
- `pid` is the process ID of each active process.
- `states` represents the state of each active process.
- `num_mapped_pages` represents the number of mapped pages for each active
	process.
- `name` represents the name of each active process.

Check out the user-space code for `ps` under `user/ps.c` to understand how those
fields are used to format and print out the fields of a certain process.

### Implementation plan

Start by adding a new system call, namely `pinfo`, that always returns -1. Then
add `_ps` to the `UPROGS` in the top-level `Makefile`. At this point, your
kernel should compile but `ps` will fail to execute since `pinfo` is not
implemented yet.

It is better to implement your system call under `sys_pinfo` in
`kernel/sysproc.c` so that you have easy access to the process information and
the process list.

Here are a few hints to help you implement the system call:
- Read the source code for `allocproc` as an example of how to manipulate the
  list of processes in the system.
- `sys_pinfo` accepts as an argument the address of `struct proc_info`, checkout
	`argaddr` for a way to read the argument from the processor registers.
- When accessing a process's PCB, it is important that you hold the lock over
	that process. The code in `allocproc` should serve as a good example, but also
	check out `acquire` and `release` for locking/unlocking a spinlock.
- **Important note**: Accessing user-space memory from kernel-space is a big NO
  NO in kernel code. Instead, you must first allocate space in kernel memory for
	the `struct proc_info` that you want to fill out, then use `copyout` to
	transfer the data from kernel-space to user-space.
	- see `sys_fstat()` (`kernel/sysfile.c`) and `filestat()` (`kernel/file.c`)
		for examples of how to do that.
- To count the number of mapped data pages, you can find inspiration in the
	implementation of `freewalk` in `kernel/vm.c`. Note that all data pages are
	leaf pages, i.e., we are not counting the memory pages used to store different
	levels of the page table, we only care about the page table entries that are
	actually used to contain text/data for each process.
	- I would recommend that you add a function called `countmapped(pagetable_t)`
		to `kernel/vm.c` to count the number of mapped pages, and then use that
		function from your system call. Don't forget to add the definition of
		`countmapped` to `kernel/defs.h` under the `vm.c` section.

### Building and testing

We have provided you with the user-space UNIX utility called `ps` to allow you
to test your implementation of the `pinfo` system call. To test your code, try
running the ps utility, you should see something that looks as follows (from the
xv6 shell):
```shell
$ ps
PID 	 STATE 	 USED PAGES 	 NAME
1 	 SLEEPING 	 5					 init
2 	 SLEEPING 	 6					 sh
3 	 RUNNING	   5					 ps
```

We have also provided you with a set of test cases (found in `user/pstest.c`)
that will test different functionalities of your code. You should pass all the
tests before you submit your project. To run the `ps` tests, use
```shell
$ pstest
pstest: starting...
pstest: testname():  OK.
pstest: teststates(): OK.
pstest: testcount(): OK.
pstest: testmem(): OK.
pstest: testexec():
PID 	 STATE 	 USED PAGES 	 NAME
1 	 SLEEPING 	 5 	 init
2 	 SLEEPING 	 6 	 sh
3 	 SLEEPING 	 6 	 pstest
14 	 RUNNING 	 5 	 ps
pstest: OK...
```

# Acknowledgements

The original version of this lab is based on different assignments provided by
the instructors of MIT's 6.S081 and modified by the Rose-Hulman instructors.


