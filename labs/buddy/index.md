---
layout: post
title: (Challenge) Buddy Allocator Heap Manager
readtime: true
date: Mon Mar 20 2023
---

# Introduction

In the previous lab, we have built a simple, somewhat inefficient, memory
manager for a processs's heap space. We would like to improve on our approach by
implementing a more interesting and faster heap manager. It will be one that
utilizes the __buddy system__ and enforces that all memory regions are powers of
2 in size (and we do love things to be powers of 2, because bitwise
operations!).

# Learning Objectives

At the end of this lab, you should be able to:

- Implement an improved memory manager that uses the buddy allocation scheme.

- Identify difference between traditional memory management and buddy-based
  memory management.

# Overview and Some Reading

Similar to our previous lab, our implemetation consists of providing support for
two functions: `rhmalloc` and `rhfree`, however, the internals of managing
the free and used slabs of memory are going to be very different.

To fully understand what a buddy allocation scheme does, you __must__ read the
"Segregated Fits and Buddy Systems" (pages 837 - 838) from the following [book
chapter](./dynamicmem.pdf). Make sure you fully understand the notions behind
the buddy system since you are in full control over who you are going to
implement it.

## Getting the Source Code

To obtain the starter code, make sure to fetch the `buddy` branch of the xv6
source code repository. To do so, __from the top level directory of your xv6
code base__, run the following:

```shell
$ git fetch
$ git checkout buddy
$ git pull
```

You should now be on a different branch that is called `buddy`. You can verify
that by checking your current branch using:
```shell
$ git branch
```

Note: for `git fetch` to work, you will need to make sure all of your changes
are either stashed or committed and pushed to your private copy of the repo.

# Starter Code

The starter code for this lab looks very much similar to the starter code for
the first heap manager lab. However, note the following minor differences.

## Metadata

We have slightly adjust the `metadata_t` structure to beautify its usage a bit.
The metadata structure is now defined as follows:

```c
struct __attribute__((aligned(8))) metadata_t {
  unsigned size;
  unsigned in_use:1;
  struct metadata_t *next;
  struct metadata_t *prev;
};
```

and we rely on the compiler to aligned it to 8 bytes in all cases.

Now, to use the fields of a metadata structure, we can directly access them
as follows:

```c
// p is some void pointer to some metadata structure
metadata_t *ctx = (metadata_t*)p;

// can now directly access size in ctx using ctx->size without the extra step
printf("metadata size = %d\n", ctx->size);
```

# The Approach

In this lab, we are going to give you a lot more freedom with your approach than
we did in the previous lab. You are free to add your own globals as long as your
implementation fulfills the requirements of a buddy system allocator. We only
ask you that you do not change any of the lines of code that are clearly
labelled as `do not change` in the comments. You will need to those to do some
magic to get enough memory at the right spot initially, specifically in
`rhmalloc_init` and `rhfree_all`.

## The Requirements

Your particular implementation must satisfy the following requirements:

1. This being a buddy system, all memory chunks (allocated or not) should be
   powers of 2 in size.

2. Allocating a chunk of an appropriate size should be fast (so no linear search
   like what we did in the previous lab!).
      - It should not be necessary to search through linked lists of memory
      regions. Instead, it should be constant time to allocate a chunk if the
      nearest power of 2 is available.

      -  If splitting is required, the number of splies should be no more than
         O(log(MAX_HEAP_SIZE)).

3. Our tests assume a minimum chunk size of 32 bytes. In other words, your
   smallest chunk of allocated memory should be no less than 32 bytes including
   the metadata.

4. Your system should use an existing chunk of the right size before splitting
   a larger block, e.g., if a recent operation left an unused chunk of size 32
   bytes, and an allocation requests a chunk of size 4 bytes, then we should use
   that existing block rather than splitting some larger chunk.

   When splitting is necessary, the system should always split the smallest
   chunk possible.

# Implementation Plan

With the exception of the our first test case, `test_get_buddy`, our tests do
not really walk you through a particular solution or implemetation approach.
However, it makes sense to get the basic allocation and freeing tests working
before going into more complicated test cases.

## Buddy Chunk Calculation

For a buddy system to work, we must be able to quickly find a chunk's buddy
without doing too much calculation. Your first task is to implement the
`get_buddy` function in `user/rhmalloc.c`.

Given a pointer to a memory chunk and a given size, `get_buddy` returns a
pointer to this chunk's buddy region, i.e., the adjacent region with only 1 bit
of difference. However, note the following:

- The size is expressed as an exponent of 2, i.e., `exponent = 6` means a
  pointer to a chunk of memory of size 64 bytes.

- A buddy region might be higher in memory or lower in memory, but __it is
    always adjacent__.

- You can always assume in this function that the pointer is always valid, i.e.,
  it points to the starting byte in a region of the given size.

- This should be very easy to do using simple binary arithemtic. We used
  bitshifts (`<<`) and binary xor (`^`) in our case. 

### Testing

To test your implementation of `get_buddy`, we have provided you with a set of
test cases that you can go through. Feel free to use those test cases as
examples to guide your implementation. You can find those test cases in
`user/buddy_test.c` in the `test_get_buddy` function.  

To test your code, build your xv6 kernel and then run:
```shell
[xv6 shell] $ buddy_test 0
test_get_buddy(48): OK.
test_get_buddy(49): OK.
test_get_buddy(51): OK.
test_get_buddy(52): OK.
test_get_buddy(53): OK.
test_get_buddy(54): OK.
test_get_buddy(55): OK.
test_get_buddy(56): OK.
```

## Basic Example

Once you have a basic implementation going (don't worry about coalescing yet),
you can use the `user/buddy_basic.c` file to test and debug your
implementation. Check out the code in there for more information. To run
this file, build xv6 using `make qemu` and then run it using `buddy_basic`.
```shell
[Linux shell] $ make qemu
...

[xv6 shell] $ buddy_basic
**************** test_basic_allocate *************************
p1 is 0x0000000000400018
p2 is 0x0000000000400038
p2 - p1 = 32 (should be 32 bytes)
**************** test_basic_allocate *************************
**************** test_basic_free *************************
p1 = 0x0000000000400018
p2 = 0x0000000000400018 (should be the same as p1)
**************** test_basic_free *************************
```

## Test Basic Alloc

This test tests only `rhmalloc`. It does require you to split blocks as memory
requests come in. Recall that initially we start with one large block of size
`2^22` then split start splitting from there.

To run this test, build xv6 using `make qemu` and then run the test:
```shell
[xv6 shell] $ buddy_test 1
test_basic_alloc(74): OK.
test_basic_alloc(79): OK.
test_basic_alloc(83): OK.
test_basic_alloc(87): OK.
```

## Test Basic Free

This tests the `rhfree` operation. Naturally, it will also include coalescing
tests.

To run this test, build xv6 using `make qemu` and then run the test:
```shell
[xv6 shell] $ buddy_test 2
test_basic_free(104): OK.
test_basic_free(108): OK.
test_basic_free(113): OK.
test_basic_free(120): OK.
```

## Test Wrong Size Coalesce

This test checks for an easy to make error where you do not check for matching
region sizes when coalescing.

To run this test, build xv6 using `make qemu` and then run the test:
```shell
[xv6 shell] $ buddy_test 3
test_wrong_size_coalesce(140): OK.
```

## Test Gracefully Run Out of Memory

Running out of memory should not be a problem for a memory manager. Functions
that call `rhmalloc` are supposed to check to see if the result of the returned
pointer is 0, which indicates an allocation failure.

Therefore, your memory manager should behave correctly (i.e., return 0) rather
than crashing or inifinite looping when a call to `rhmalloc` cannot be fulfilled
because all memory is occupied.

To run this test, build xv6 using `make qemu` and then run the test:
```shell
[xv6 shell] $ buddy_test 4
test_gracefully_run_out_of_memory(152): OK.
```

## Test Stress Big Heap

This test allocates a bunch of very big memory regions and then frees them in a
random order. If you've implemented the previous parts correctly, this should
work out of the box. One thing that can sometimes cause an issue is incorrect
coalescing. After the big chunks have all been freed, the code allocates a
really big chunk. This will fail unless all the previous regions have correctly
coalesced.

To run this test, build xv6 using `make qemu` and then run the test:
```shell
[xv6 shell] $ buddy_test 5
test_stress_big_heap_chunks(172): OK.
test_stress_big_heap_chunks(172): OK.
test_stress_big_heap_chunks(172): OK.
test_stress_big_heap_chunks(172): OK.
test_stress_big_heap_chunks(172): OK.
test_stress_big_heap_chunks(172): OK.
test_stress_big_heap_chunks(172): OK.
test_stress_big_heap_chunks(172): OK.
test_stress_big_heap_chunks(185): OK.
```

## Test Stress Overlapping

This test randomly allocates and deallocates regions. While doing allocations,
it checks to ensure that the newly created region does not overlap with an
existing allocated region. If you've implemented the previous parts
correctly, this should work out of the box. Debugging this one can be trickier
because it generates plenty of regions.

To run this test, build xv6 using `make qemu` and then run the test:
```shell
[xv6 shell] $ buddy_test 6
test_stress_overlapping(263): OK.
```

# Running the Full Grading Script

Once you are done implementing all the above programs, run the grading script
using
```shell
[Linux shell] $ make grade
```
from your Linux terminal (not your xv6 terminal window).

# Submitting Your Code

From the Linux terminal, issue the command:
```shell
[Linux shell] $ make submit
```
Two files will be generated, `submit-lab-buddy.patch` and
`submit-lab-buddy.tar`. **Please submit both of these files to Gradescope** at
the appropriate lab link.

## Submission Checklist

- [ ]  My code compiles and generates the right executables.
- [ ]  I ran `make grade` to double check the test cases for all of my code.
- [ ]  I ran `make submit` to generate the `.patch` and `.tar` files.
- [ ]  I submitted both `.patch` and `.tar` files to [Gradescope]({{
    site.gradescope_url }}).

## Grading

Check out this assignment's [grading](checklist/) page for more information.

