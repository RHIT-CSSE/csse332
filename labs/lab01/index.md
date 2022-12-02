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

To obtain the updated source code, make sure to fetch the `lab01` branch of the
xv6 source code repository. To do so, from your xv6 top level directory, issue
the following commands:
```shell
$ git fetch
$ git checkout lab01
$ git pull
```
You should be on a new branch that is called `lab01`. You can verify that using
```shell
$ git branch
```

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

We have provided you with the starting code for this function. Here is how it
goes. First, we call `heap_mem_start = sbrk(0);` to ask the operating system to
tell if it were to allocate any memory for us, where would the starting address
of that piece of memory be. 

Next, we actually ask the OS to reserve a large slab of memory of size
`MAX_HEAP_SIZE` for us using:
```c
p = sbrk(MAX_HEAP_SIZE);
if(p == (char *)-1) {
  fprintf(2, "sbrk failed:exiting....\n");
  exit(1);
}
```
At that point in time, if the `sbrk` system call succeeds, we will know for sure
that `p == heap_mem_start` and that they both point to an area of memory that is
of size `MAX_HEAP_SIZE`.

Your job in `rhmalloc_init` is the implement the code that properly initializes 
`freelist`. Note that initially, we only have a single free block, so our doubly
linked list contains a single node only (in other words, the `next` and `prev`
pointers are invalid). Here are a few hints to help you implement this function:
1. Note that each block contains its own metadata at the top of the block. In
   other words, the first `sizeof(metadata_t)` bytes of each block are reserved
   for that block's metadata. This should help you obtain `freelist` from `p`.
2. Note that the `size` variable in each block refers to the __usable__ size of
   that block, i.e., its size __without__ its metadata. 
3. All blocks start off not being in use. 

## Freeing everything

Next, let's take a look at the `rhfree_all` function. This function is tasked
with freeing everything and returning the previously allocated slab of memory
back to the operating system. We have implemented that function for you. 

In short, calling the system call `sbrk` with a negative number `x` __shrinks__
a process's memory region by `x` bytes. That is why we make the call
`sbrk(-MAX_HEAP_SIZE)`.

The next two lines simply reset `freelist` and `heap_mem_start` to 0. 

## Implementing `rhmalloc`

Now let's start with implementing `rhmalloc`. We have provided you with the
first two lines that call `rhmalloc_init` in case `freelist` has not been
initialized yet. 

When a user requests `x` bytes, the memory manager will search for a free block
that can fit those `x` bytes __plus the metadata__ required to maintain the
information about that block. Then, if the found block is much larger that `x`
bytes, the memory manager will split that block into two contiguous blocks, one
that is return to the user and one that contains the free bytes remaining from
the split operation. For example, ignoring the metadata bytes, if the memory
area initially consists of a single block of size 4 bytes, and the user requests
1 bytes, then the memory manager will split the 4 bytes block into two blocks:
one of size 1 byte that is return to the user and another of size 3 bytes that
is still marked as free and can be used to allocate memory for other users. 

Now you should implement the process by which our memory manager will allocate a
block of `size` bytes to whoever called `rhmalloc`. Here are a few hints:

1. Note that we always allocate things in multiples of `ALIGNMENT` bytes.
   Therefore, as discussed earlier, you should use the `ALIGN` macro to figure
   out how many bytes to allocate for that user. 
2. Next, your code must search the `freelist` for the __first__ block that can
   fit the user's requested size. Once you find such a block, two cases might
   arise:
   1. The found block can only fit the user's request, then your search is done
      and you should update the block's metadata and return to the user a
      pointer __to the start of the usable section__ of the block (In other
      words, the user should not be aware of the presence of the `metadata_t`
      struct in the block). 

       For example, if the free block you found is of size 24 bytes and the user
       requests 16 bytes, then it doesn't make sense to split the block since it
       cannot fit any other request from the user. So we can bite the bullet and
       allocate to the full 24 bytes to the user even though they only needed 16
       bytes.

   2. The found block is larger than the user's request, then we must split that
      block into two blocks, one that is returned to the user and another that
      is free and contains the remaining bytes. Then, if another request comes
      in, the memory manager will use the second block to service that user's
      request. 

### Testing

Once you complete your implementation of `rhmalloc`, you can use the
`test_basic_alloc` test in `user/lab01test.c` to verify that your code works
correctly. To do so, first compile your code using
```shell
$ make qemu
```
and then run the test with the parameter 1 to only run the `test_basic_alloc`
test case using
```shell
$ lab01test 1
test_basic_alloc(57): OK.
test_basic_alloc(61): OK.
test_basic_alloc(62): OK.
test_basic_alloc(63): OK.
test_basic_alloc(64): OK.
test_basic_alloc(68): OK.
test_basic_alloc(69): OK.
test_basic_alloc(70): OK.
test_basic_alloc(71): OK.
test_basic_alloc(72): OK.
test_basic_alloc(77): OK.
test_basic_alloc(81): OK.
test_basic_alloc(82): OK.
test_basic_alloc(83): OK.
test_basic_alloc(84): OK.
test_basic_alloc(87): OK.
test_basic_alloc(88): OK.
test_basic_alloc(90): OK.
test_basic_alloc(91): OK.
```

Again, just like in the previous lab, the number between parenthesis tells you
which test case passed or failed. 

## Implementing `rhfree`

Next, let us implement `rhfree`. Given a pointer `ptr`, `rhfree` will find the
memory block that `ptr` belongs to and then mark that pointer as free by
updating the metadata of that specific block. And that is the basic operating of
the `rhfree` call. 

### Testing basic `rhfree`

Once you have implemented the basic idea behind `rhfree`, you can double check
the correctness of your code by running the `test_basic_free` test case from
`user/lab01test.c` as follows:
```shell
$ lab01test 2
test_basic_free(114): OK.
test_basic_free(115): OK.
test_basic_free(116): OK.
test_basic_free(117): OK.
test_basic_free(120): OK.
test_basic_free(128): OK.
test_basic_free(129): OK.
test_basic_free(132): OK.
test_basic_free(133): OK.
test_basic_free(134): OK.
test_basic_free(139): OK.
test_basic_free(140): OK.
test_basic_free(142): OK.
test_basic_free(143): OK.
test_basic_free(145): OK.
test_basic_free(150): OK.
test_basic_free(154): OK.
test_basic_free(155): OK.
```

## Implementing coalescing

One disadvantage of implementing `rhfree` in the basic way discussed above is
that if the users request to allocate a lot of tiny sized blocks and then free
them. The memory manager's free list will end of a lot of tiny free blocks. And
if a user comes in with a request for a large piece of memory, then the memory
manager would not be able to service that request since it does not have a
contiguous large block of free memory. 

Here is an example to make it clearer. Ignoring metadata, let's assume initially
we have a single free block of size 4 bytes. Then a user makes 4 requests for 4
blocks of size 1 byte each, and then frees those blocks. Using our basic
`rhfree` implementation, the memory manager will now contain a list of four free
block, each of size 1 byte. 

If a user now comes in and asks for a block of 2 bytes of memory, the memory
manger would not be able to service that request since it only has 4 blocks of
size 1 byte each, but no blocks of size 2 bytes or more. This is bad because
even though we actually have 4 bytes of free memory, we cannot service the
user's request because those 4 bytes are not in a single block. 

To solve this problem, every time a user frees a pointer, we must check whether
we can merge, or coalesce, any free blocks in our free list. Since we do this
every time we free a pointer, we only need to check for adjacent blocks (i.e.,
we do not need to scan the entire free list, only the block adjacent to the
block we have just freed).

### Implementation and testing

To implement coalescing, when a user calls `rhfree`, you must check if the freed
block is adjacent to another free block, and then merge those two blocks
together (while preserving the ordering of the pointers in the list). Note that
there are two cases we must take care of:
1. The freed block is the first in a pair of blocks that must be merged, then
   the resulting block will have the same address as that of the freed block,
   and the second block in the pair will no longer be addressable. 

   For example, if we have two blocks (`a -> b`) that are both free and each of
   size 1 byte, then we must merge those two into a single block with address
   `a` of size 2 bytes. 
2. The freed block is the second in a pair of blocks that must be merged, then
   the results block will have the address of the first block in the pair and
   the free block will no longer be addressable. 

   For example, if we free a block `a` and then find a block `c` that is also
   free, and such that `c` directly precedes `a` in the free list, then we must
   merge those two blocks into a single block `c` that is of size `size(c) +
   size(a)`.

To check the implementation of case 1 above, run the `test_coalesce_basic1` test
from the `user/lab01test.c` as follows:
```shell
$ lab01test 3
test_coalesce_basic1(176): OK.
test_coalesce_basic1(177): OK.
test_coalesce_basic1(178): OK.
test_coalesce_basic1(181): OK.
test_coalesce_basic1(182): OK.
test_coalesce_basic1(183): OK.
```

To check the implementation of case 2 above, run the `test_coalesce_basic2` test
from the `user/lab01test.c` as follows:
```shell
$ lab01test 4
test_coalesce_basic2(204): OK.
test_coalesce_basic2(205): OK.
test_coalesce_basic2(206): OK.
test_coalesce_basic2(209): OK.
test_coalesce_basic2(210): OK.
test_coalesce_basic2(211): OK.
```

To check the combination of both cases, run the `test_coalesce3` test from the
`user/lab01test.c` as follows:
```shell
$ lab01test 5
test_coalesce3(233): OK.
test_coalesce3(234): OK.
test_coalesce3(235): OK.
test_coalesce3(238): OK.
test_coalesce3(239): OK.
test_coalesce3(240): OK.
test_coalesce3(244): OK.
test_coalesce3(245): OK.
test_coalesce3(246): OK.
test_coalesce3(247): OK.
test_coalesce3(248): OK.
```

## Running the test cases

If you were careful when implementing `rhmalloc` and `rhfree` above, the
remainder of the test cases should pass automatically. Here is a description of
the remaining ones.

### Test force alignment

This tests that your code aligns correctly. By aligned here we mean evenly
divisible by the size of the archiecture - because in this system pointers are
8-bytes, our struct must start at an address that is divisible by 8. Similarly,
because we have no idea what our user might want to store in their allocated
space, we must also always give them an address that is divisible by 8.

To run this test case, run `lab01test` with the argument 6 as follows:
```shell
$ lab01test 6
test_force_alignment(261): OK.
test_force_alignment(268): OK.
test_force_alignment(271): OK.
test_force_alignment(273): OK.
test_force_alignment(275): OK.
test_force_alignment(276): OK.
```

### Test too small blocks

This test ensures that you never have an entry in the free list that is too
small to actually contain data. This can happen when a new `rhmalloc` requests a
size that is very slightly smaller than an available block in the free list. The
normal thing to do would be to split the block, but if it is just slightly
smaller there isn't enough space to actually add a new entry. In this case the
block should not be split, and instead the slightly larger block should be fully
allocated to the request (and, as with alignment the stored size should reflect
the true allocation not the requested allocation).

In this system, the minimum size needed to allocate a viable block is the size
of a aligned metadata block + 8 bytes.

To run this test case, run `lab01test` with the argument 7 as follows:
```shell
$ lab01test 7
test_too_small_blocks(295): OK.
test_too_small_blocks(306): OK.
test_too_small_blocks(315): OK.
test_too_small_blocks(316): OK.
test_too_small_blocks(326): OK.
test_too_small_blocks(332): OK.
test_too_small_blocks(333): OK.
test_too_small_blocks(344): OK.
test_too_small_blocks(348): OK.
test_too_small_blocks(350): OK.
test_too_small_blocks(351): OK.
test_too_small_blocks(352): OK.
```

### Test gracefully run out of memory

Running out of memory should not be a problem for a memory manager. Functions
that call `rhmalloc` are supposed to check to see if the result pointer was `0`
(which indicates an allocation failure). So your memory manager should behave
correctly (i.e., return `0`) rather than crashing or infinite looping when a
malloc cannot be fulfilled because the slab is fully in use.

To run this test case, run `lab01test` with the argument 8 as follows:
```shell
$ lab01test 8
test_gracefully_run_out_of_memory(364): OK.
```

### Test stress big heap

This test allocates a bunch of very big memory regions and then frees them in a
random order. If you've coded the previous parts correctly, this should work on
the first try. One thing that can sometimes cause an issue is incorrect
coalescing - after the big chunks have all been freed, the code allocates a
really big chunk. This will fail unless all the previous regions have been
correctly coalesced.

To run this test case, run `lab01test` with the argument 9 as follows:
```shell
$ lab01test 9
test_stress_big_heap_chunks(384): OK.
test_stress_big_heap_chunks(384): OK.
test_stress_big_heap_chunks(384): OK.
test_stress_big_heap_chunks(384): OK.
test_stress_big_heap_chunks(384): OK.
test_stress_big_heap_chunks(384): OK.
test_stress_big_heap_chunks(384): OK.
test_stress_big_heap_chunks(390): OK.
test_stress_big_heap_chunks(401): OK.
```

### Test stress overlapping

This test randomly allocates and deallocates regions. While doing allocations,
it checks to ensure that the newly created region does not overlap with an
existing allocation region. If you've coded the previous parts correctly, this
should work on the first try. 

To run this test case, run `lab01test` with the argument 10 as follows:
```shell
$ lab01test 10
```
There will be no printout in case the test is successful. 


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
