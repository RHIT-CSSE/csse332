---
layout: post
title: Lab 01 -- Heap Manager
readtime: true
date: Thu Nov 10 08:48:11 2022 
---

# Introduction

A basic feature of any programming library is a heap manager that handles the
allocation and freeing of memory resources. The memory manager is invoked every
time you allocate room for a class or object, and then when you no longer need
those resources. The heap manager is not a feature of the operating system, it
is rather provided by the maintainers of the programming language you are using.
Specifically, for C, one implementation of a heap manager comes from the
standard `libc` library, and handles the calls to `malloc` and `free`. The heap
manager presents the users with a library that wraps around many system calls
that the operating system provides. 

In this lab assignment, we will implement a basic heap manager for the C library
in the xv6 operating system. However, note that the heap manager we will be
designing is not built-into the operating system. Rather it lives entirely in
user-space and is only accessible through a user-level API.

## Learning Objectives

At the end of this lab assignment, you should be able to:
- Implement a basic memory manager for users of the xv6 operating system. 
- Use the `sbrk` system call to reserve memory from the operating system.
- Manage free memory and provide users with chunks of memory based on API
  requests. 

# Overview and Some Reading

We will implement the Rose-Hulman memory manager in the xv6 operating system by
providing support for two main function calls: `rhmalloc` and `rhfree` that
allocate memory on the heap, and free allocated memory, respectively. First, it
is a good idea to gain some familiarity with the standard `libc` memory API,
specifically, `malloc` and `free`, by reading [this
section](https://pages.cs.wisc.edu/~remzi/OSTEP/vm-api.pdf) from our textbook. 

Furthermore, managing memory requires us to maintain a free list that tracks the
free chunks of memory that can be allocated based on user requests.  [This
section](https://pages.cs.wisc.edu/~remzi/OSTEP/vm-freespace.pdf) of our
textbook provides a great deal of information about strategies to maintain a
free list in a heap management system. 

Briefly, our heap manager first allocates a large chunk (or slab) of memory from
the operating system by asking for it using the `sbrk` system call. That chunk
of memory is now owned by the heap manager and can then be distributed to users
based on incoming requests. When a user makes a request for memory using the
`rhmalloc` function, the memory manager will try to accommodate the request by
splitting the list of available memory and giving the user ownership over a
chunk of the requested size. Once the user is done with that chunk of memory,
they will use the `rhfree` function call to give back ownership of the memory
area to the memory manager that merges it with its list of free memory chunks. 
Note that throughout this lab, we will make several simplifying assumptions that
will make our lives easier and makes the process of implementing a memory
manager more enjoyable.

# Getting the Source Code

<!-- TODO: Write this next! -->

# Structures and Definitions

To get you started, we have provided you with some definitions and structures
that are going to be helpful for you in implementing our Rose-Hulman heap
manager. Let's go through them one by one so we can understand what is going on. 
We will start with the definitions in `user/rhmalloc.h` and then move to the
helper functions in `user/rhmalloc.c`.

## Alignment

First, we have defined the maximum heap size to be 1MB of memory. In other
words, we only have 1MB of memory to allocate to the users' requests. Normally,
the heap grows dynamically as we need more memory, but in this lab, we will make
the simplifying assumption that our heap never exceeds 1MB. If a user requests
an area that is larger than 1MB, our memory manager will simply refuse to
allocate memory and return an invalid pointer. Therefore, the `MAX_HEAP_SIZE`
macro is defined as 1024 x 1024 bytes as follows:
```c
#define MAX_HEAP_SIZE 1024*1024
```

Next, we need to make sure that our chunks are properly aligned in memory. In
other words, we want all of our allocated and free memory chunks to be in
multiples of `ALIGNMENT` bytes (for reasons that will become clearer later on in
the course). Therefore we define the `ALIGNMENT` macro as the size of an
`unsigned long` integer:
```c
#define ALIGNMENT sizeof(unsigned long)
```
In addition, we define the macro `ALIGN` that takes as input a `size` in bytes,
and adjusts it so that it is a multiple of `ALIGNMENT` bytes:
```c
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))
```

For practical purposes, anytime a user wants to allocate `s` bytes, we must
align those bytes to the closest multiple of `ALIGNMENT` bytes by using the
`ALIGN` macro as follows:
```c
unsigned size_to_allocate = ALIGN(s);
```

## The Metadata

Next, in order to keep track of our memory blocks, we must maintain a set of
information about each block (like whether it is allocated or not, its size,
etc.). We therefore define the union metadata as follows:
```c
union metadata {
  struct {
    unsigned size;          /* The size of the block. */
    unsigned in_use:1;      /* Flag to indicate if the block is in use.*/
    union metadata *next;   /* The next pointer in the linked list. */
    union metadata *prev;   /* The prev pointer in the linked list. */
  } s;
  unsigned long alignment;
};
```
and then we define a new type based on this union to make it easier to code
using:
```c
typedef union metadata metadata_t;
```

The relevant information in the metadata union are those contained in the struct
s inside it. The `alignment` variable put in there is to ensure that the size of
the metadata information is always going to be a multiple of `sizeof(unsigned
long)` bytes. In other words, every time we make room for a `metadata_t`
structure, we actually allocate enough bytes to  hold a multiple of `ALIGNMENT`
bytes. Since every block contains its own metadata, this helps us make sure that
each block is always aligned to `ALIGNMENT` bytes. 

Note that each block's metadata contains two pointers, `prev` and `next`. This
indicates that the blocks are maintained in a simple [doubly linked
list](https://en.wikipedia.org/wiki/Doubly_linked_list). For simplicity, we
assume that the blocks are sorted by increasing memory order. In other words,
the `next` block always points to an address that is higher in memory, and the
`prev` block always points to an address that is lower in memory. 

Finally, note that in our Rose-Hulman memory manager, __we keep track of both
free and used blocks__. Unlike more sophisticated memory managers who only keep
track of a free list, we find it simpler to implement for our educational
purposes to keep track of all of our blocks, regardless of whether they are free
or in-use. 

## Globals and Helper Functions

Next, let's take a look at the `user/rhmalloc.c` file. The static global
variable `heap_mem_start` is used by our memory manager to keep track of the
start of the region of memory to allocate. It is initialized to 0 (i.e.,
`NULL`).

Second, the static metadata variable `freelist` refers to the head of the list
of the doubly linked list of blocks to be allocated. Similar to
`heap_mem_start`, it is initialized to 0 and should be re-initialized when the
heap manager is first called upon. Additionally, the function `freelist_head`
simply returns a pointer to the head of the list, i.e., it simply returns
`freelist`.  In a similar fashion, the function `heap_start` returns the
`heap_mem_start` pointer. 


# Implementation and Testing

## Initialization

First thing we would like to implement is the initialization code in
`rhmalloc_init`. This function should only be called once at the start to
communicate with the operating system kernel and reserve a large piece of memory
of size `MAX_HEAP_SIZE`. It is also responsible for initializing
`heap_mem_start` and the free list. 

# Running the full grading script

Once you are done implementing all the above programs, run the grading script
using
```shell
$ make grade
```
from your Linux terminal (not your xv6 terminal window). 

# Submitting your code

From the Linux terminal, issue the command:
```shell
make submit
```
Two files will be generated, `submit-lab1.patch` and `submit-lab1.tar`. **Please
submit both of these files to Gradescope** at the appropriate lab link.

## Submission Checklist

- [ ]  My code compiles and generates the right executables.
- [ ]  I ran `make grade` to double check the test cases for all of my code.
- [ ]  I ran `make submit` to generate the `.patch` and `.tar` files.
- [ ]  I submitted both `.patch` and `.tar` files to [Gradescope]({{
    site.gradescope_url }}).

## Grading

Check out this assignment's [grading](checklist/) page for more information.
