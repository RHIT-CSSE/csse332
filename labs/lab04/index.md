---
layout: post
title: Lab 04 -- Virtual Memory I
readtime: true
date: Wed Jan  4 10:03:08 2023 
---

# Introduction

In this assignment, we will be augmenting a [language virtual
machine](https://en.wikipedia.org/wiki/Virtual_machine#Process_virtual_machines).
In essence, a virtual machine is a program that hosts programs in another
programming language, providing a device independent layer for cross platform
execution. For example, the Java virtual machine executes Java programs. A
virtual machine is actually similar to an operating system in a lot of ways,
especially when the hosted language is actually compiled at runtime into
assembly language and run directly on the CPU. This is know as Just-In-Time
(JIT) compilation and our virtual machine will do something pretty similar.

# Learning Objectives

At the end of the this assignment, you should be able to:
- Catch segmentation faults and implement custom handlers for them. 
- Manage memory for forth instances (basically processes) by allocating pages
  for them only upon need.
- Map memory pages onto disk and swap pages back and forth between physical
  memory and disk.

# Getting the Source Code

For this lab, you will be using the native Linux virtual machine (or baremetal
machine if you have one) and not the xv6 operation system. __Please note that
this assignment will NOT run on MacOSX__, you should be running a Linux machine
using the x86 architecture.

To obtain the starting code for this lab, navigate to the top level directory of
your __csse332 class repository__ and `cd` into the `labs/lab04` directory as
follows:
```shell
$ cd /path/to/csse332/git/repository/
$ git pull
$ cd labs/lab04
```

# Starter Code

## Forth

The language our virtual machine will be hosting is known as
[Forth](https://en.wikipedia.org/wiki/Forth_(programming_language)).
It is not that important for you to fully understand this language, but you
might find it handy to know a bit. The Wikipedia page is a good introduction,
and if you'd like to try some code you can type `make interactive` with the
provided `makefile`. This will build an interactive forth that you can issue
commands to and see output:

```shell
$ make interative
$ ./jonesforth.bin forth/jonesforth.f
Welcome to forth! Press ^D to quit. 
3 2 + .
5
```

## Paged Forth

Using the interactive version of forth is a great way to get introduced to the
language, however, when in this lab, we would like to provide input to forth as
a C string and read outputs from it as a C string. For that, take a look at the
`paged_forth.c` file. To run the code, use the following steps:

```shell
$ make pagedforth.bin
gcc -no-pie -ggdb -Wall -c paged_forth.c -o paged_forth.o
gcc -no-pie -ggdb -Wall -c forth/forth_embed.c -o forth_embed.o
gcc -no-pie -ggdb -Wall -c forth/myjf.S -o jonesforth.o
gcc -no-pie -ggdb -Wall -o pagedforth.bin forth_embed.o jonesforth.o paged_forth.o

$ ./pagedforth.bin
finished loading starter forth
OUTPUT: 4999 5000 finished successfully
done
```

The comments in `paged_forth.c` should be mostly self-explanatory, but a few
details here might be helpful:

1. `mmap` is mostly a more powerful version of `malloc`. We will be using a lot
   of its features in this lab, so you might want to familiarize yourself with
   its man page.
2. The `forth_data` struct is a structure that represents the total state of a
   forth system (similar to the process control block for processes in our
   operating system discussions). However, it does not allocate space for the
   forth return stack, stack, or heap. This memory must be allocated by the user
   and then be passed to the `initialize_forth_data` function, which sets the
   `forth_data` struct up. The existing code does all of that for you.
3. The second parameter passed to `f_run` is the forth code we would like to
   run.
4. The third parameter passed to `f_run` is the buffer where forth's output
   should go.

# Step 1

## Goals

Like most programs, forth expects memory to be a contiguous space with the stack
at the top (high memory) and the heap at the bottom (low memory). Forth uses
both parts of the memory. It uses the stack to store its data stack, and uses
the heap to store its function definitions and some globals. These two memory
locations grow towards each other but hopefully never get so large they overlap.

Like an operating system does, we would like to start the stack and the heap
very fat apart to accommodate the largest possible program. However, similar to
modern operating systems, we would prefer if we did not have to allocate all
that memory upfront because most programs won't use all of it. In the
`paged_forth.c` right now, we allocate the entire (20 pages) memory block
upfront. Our first step is to change that so that we only allocate a page of
memory when it is needed.

Here's a possible plan to achieve that:

1. We want to initially pass the forth instance pointers to a memory region that
   is actually invalid.
2. Then we will set up a signal handler that detects segmentation faults.
3. We will determine the address that is failing and determine which page that
   bad address corresponds to.
4. We will use `mmap` to allocate memory for that exact page, at the particular
   address we need it to be. Note that `mmap` must be placed at a page boundary
   (i.e., allocations must not overlap).
5. We will then return from the signal handler and allow the erroring code to
   run again, which should succeed because the requested address is now valid. 
6. Code will continue until another invalid page is referenced.
7. Eventually, we will have loaded only the parts of the stack/heap we actually
   use, while leaving unreferenced memory unallocated.

## Implementation

Take a look at `segfault_catch_example.c`, this shows you how to correctly
install a signal handler to catch a segmentation fault, and then resume
operations after the segmentation fault is resolved. Use that code as your
starting point to make changed to `paged_forth.c`.

The given code always maps the same page, but you will need to edit it. Your
code will need to use some pointer math to determine in which page (of the
possible 20 pages) the address that causes the segmentation fault resides. Then,
you will compute the beginning of that page, and then `mmap` it.

You will want to modify the line in `main` so that the stackheap is not actually
allocated. So the line:

```c
void* stackheap = mmap(NULL, stackheap_size, PROT_READ | PROT_WRITE | PROT_EXEC,
                       MAP_ANON | MAP_PRIVATE, -1, 0);

```

becomes

```c
void *stackheap = (void*) STACKHEAP_MEM_START;
```

## Testing

When completing the above steps, your output should look as follows:

```shell
in handler with invalid address 0xf9f9fff8
mapping page 19
in handler with invalid address 0xf9f8c000
mapping page 0
in handler with invalid address 0xf9f8d000
mapping page 1
in handler with invalid address 0xf9f9eff8
mapping page 18
in handler with invalid address 0xf9f9dff8
mapping page 17
in handler with invalid address 0xf9f9cff8
mapping page 16
in handler with invalid address 0xf9f9bff8
mapping page 15
in handler with invalid address 0xf9f9aff8
mapping page 14
in handler with invalid address 0xf9f99ff8
mapping page 13
in handler with invalid address 0xf9f98ff8
mapping page 12
in handler with invalid address 0xf9f97ff8
mapping page 11
in handler with invalid address 0xf9f96ff8
mapping page 10
OUTPUT: 4999 5000 finished successfully
done
```

Make sure your output matches the above before going on to step 2.

# Step 2

## Goals

In this step, we would like to further improve the memory footprint of our forth
instances by writing infrequently used memory pages to files on disk.

Here's a possible plan of action:

1. Our forth instance will have a maximum number of pages we want to keep in
   memory at one time, that is defined by the `MAX_PAGES` constant. This can be
   set to anything that is 3 or higher.
2. When we catch a segmentation fault indicating we need to load a new page, we
   will see if we're already at our max pages.
3. If we are, we will first write our oldest page onto disk, then `unmap` it
   from memory.
4. Then we will `mmap` the newly required page into memory. If it's a page we
   will loaded before, we will load that page from the file instead.
5. We will probably have to update some metadata to keep track of what pages
   have been in memory the longest.

## `mmap`

This problem is made much easier by the fact that `mmap` has a feature that
allows to map a file into a particular region of memory. If we do this, the
memory region will be populated with the file's content when it is created.
Also, any modifications to the memory page will automatically be transmitted
back to the file. So `mmap` will handle most of the work of file operations.

Take a look at `mmap_file_example.c` for details.  Note that this example remove
the `MAP_ANONYMOUS` option from the `mmap` parameters. This is important because
anonymous means _not mapping to a disk or file_, which was true in the first
part but is no longer true for this step.

## Tracking the Oldest

It is important that we select the right page to `unmap` when a new page needs
to be swapped in. Otherwise, we can generate an infinite loop. Imagine for
example that we use an algorithm that always swaps out the smallest numbered
page. Imagine our code is attempting to execute an assembly language
instruction stored in page 1 to an address stored in page 2.

If pages 1 and 3 and 4 are loaded, this will cause a segmentation fault and we
will replace page 1 with page 2. Now pages 2, 3, and 4 are loaded. This will
cause a segmentation fault when accessing page 1, so we will replace page 2 with
page 1.

Hopefully, you can see the loop taking place. Alternatively, if we always swap
the oldest page, we can avoid this problem. Even better might be to track the
most recently used pages, but unfortunately, our system can only detect when
blocks are loaded not when an already loaded block is accessed.

You can use any mechanism you like to detect the oldest page (and they should
all be equivalent), but here's my approach:

1. Store a `priority` for every page in the system. When a page enter (i.e., is
   loaded), give it the best priority, which is the max allowed number of pages.
2. When another page is loaded, loop through all existing pages and drop their
   priority by 1. If any page drops to 0 priority (only one should be at a time,
   at most), then unmap it from memory.

## Page States

A page in this system can be in 3 states:
1. Active.
2. Never mapped before.
3. Swapped out.

If you need to swap in a page for the first time, you need to open a file
descriptor and create a file of the appropriate size.

When a page is unmapped, you don't have to lost the file descriptor. Instead,
keep it saved but unmap the memory, mark the page as swapped out.

When a page is swapped in, reuse the file descriptor but remap the file.

## Testing

The file `finaloutput.txt` contains my solution's output when `MAX_PAGES = 3`.
To see if yours matches exactly, run your code and pipe the output to a file,
and then use the `diff` command as follows:

```shell
$ ./pagedforth > testout.txt
$ diff finaloutput.txt testout.txt
```

There should not be anything printed out on the command line.

# Submitting your code

There are no special submission requirements for this lab, you only have to
submit the `paged_forth.c` file. __Please do not submit any other files than
`paged_forth.c`__.

## Submission Checklist

- [ ]  My code compiles and generates the right executables.
- [ ]  I submitted `paged_forth.c` to [Gradescope]({{ site.gradescope_url }}).

## Rubric

| Part                                                                      | Points |
|---------------------------------------------------------------------------|--------|
| Step 1: manages to catch segfaults                                        | 20     |
| Step 1: works correctly and matches output                                | 40     |
| Step 2: produces some files                                               | 15     |
| Step 2: seems to be swapping pages in and out but not with FIFO algorithm | 15     |
| Step 2: works correctly and matches output                                | 50     |

