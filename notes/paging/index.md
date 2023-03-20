---
layout: post
title: Paging
readtime: true
date: Mon Mar 20 07:13:10 2023 
---

# Learning Objectives

At the end of this lecture, you should be able to:

- Define memory virtualization and its different approaches.
- Define the main shortcomings with segmentation.

---

# Topics

In this lecture, we will cover the following topics:

- Segmentation.
- Fragmentation.
- Introduction to Paging.

---

# Notes


<!-- vim-markdown-toc GFM -->

* [Memory Virtualization](#memory-virtualization)
* [Segmentation](#segmentation)
  * [Problem with Segmentation](#problem-with-segmentation)
* [Paging](#paging)
  * [Paging Address Translation](#paging-address-translation)

<!-- vim-markdown-toc -->

# Memory Virtualization

- The operating system plays the role of an illusionist that gives each process
  the illusion that it owns the entirety of memory.
- Each process has its memory organized into four main regions:
  1. The text (or code) region.
  2. The globals region.
  3. The stack starting from the high address and growing down.
  4. The heap starting from the end of the globals and growing up.
- To trick each process into thinking that it owns __all__ of its address space
  (from low memory and high memory), the operating system performs __address
  translation__.
  - All addresses that the process operates on are referred to as __virtual
    addresses__.
  - The operating system will translate those virtual addresses into __physical
    addresses__ that can then be put on the bus and used to index into memory.

# Segmentation

- In order to provide address translation, in the early days, operating systems
  adopted the approach of segmentation.
- Each memory region is mapped into its own segment in memory.
- Each virtual address is divided into:
  - a segment number the indicates in which segment of memory does the address
    reside,
  - and a segment offset that indicates the offset into that segment where to
    locate the corresponding byte of memory.
- Segmentation adopts an approach in which each segment of memory is of
  different size.
- This no longer requires that all of memory resides in a contiguous block in
  physical memory, it can be spread out across the different segments.

## Problem with Segmentation

- By adopting variable-sized segments, the segmentation process leads to the
  problem of __external fragmentation__.
- When segments of different sizes get freed, they created different-sized holes
  in physical memory.
  - Those holes may add up to a large-size memory area, but since it is not
    fully contiguous it cannot contain an actual  segment. 
- This problem is common with any approach that uses variable-sized segment of
  memory, we will revisit it again when we talk about file-systems as well.

# Paging

- ❓ What would be the logical next step to solve the problem of
  external fragmentation?
  - Make the memory fragments be of equal size.
  - Drop the contiguity requirement even within memory regions.

- Memory is now decomposed into __fixed-size__ blocks, referred to as pages in
  virtual space, and frames in physical space. 
- In other words, now, the entire process address space is composed of multiple
  different pages of memory.
  - The distinction between memory regions is now only a conceptual one that is
    handled in software by the operating system.
  - Each abstract memory region, and thus each page, now contains some metadata
    information that keeps track of whether the page can be read, written to,
    executed, and accessed by the user.

- The operating system will maintain the mapping between each virtual page and
  its corresponding physical frame. 
- ❓ So how would we treat the address that is sent to us by the user?

## Paging Address Translation

- Imagine that we are dealing with C code that is accessing an array entry
  `a[1]`, doing something like `a[1] = 3;`.
- In RISC-V assembly, this will translate into something that looks like the
  following, assuming that the address of `a` is contained in the `x6` register.
  ```assembly
  addi x5, x5, 3
  sw   x5, 4(x6)
  ```
- All the addresses that are used in the assembly instructions are virtual
  addresses.
- When received by the operating system, those addresses are interpreted (for
  now) as two parts (assume the address is `m` bits):
  1. The first `l` bits are reserved for the page index.
    - They determine which frame is the page mapped to in physical memory.
  2. The remaining `m - l` bits represent the offset of the address into the
     physical frame, and are used as is when addressing into the physical frame. 

- So this means that we will have `2 ** l` pages.
- Each page will have a size of `2 ** (m-l)` bytes.

- ❓ Where should we keep this mapping between virtual pages and
  physical frames?
  - Memory itself!
