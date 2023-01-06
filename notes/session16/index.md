---
layout: post
title: Paging
readtime: true
date: Wed Jan  4 13:52:45 2023
---

# Learning Objectives

At the end of this lecture, you should be able to:

- Recap the paging approach and its shortcomings.
- Define how caching can help improve the performance of paging using the
  notions of temporal locality and spatial locality.
- Define multi-level paging and draw the access pattern of a multi-level page
  table.

---

# Topics

In this lecture, we will cover the following topics:

- Shortcomings of paging.
- The Translation Lookaside Buffer (TLB).
- Multi-level page tables.

---

# Notes


<!-- vim-markdown-toc GFM -->

* [Problems with Paging](#problems-with-paging)
* [Caching to Speedup Access](#caching-to-speedup-access)
* [Multi-level Page Tables](#multi-level-page-tables)
* [The xv6 Paging Mechanism](#the-xv6-paging-mechanism)

<!-- vim-markdown-toc -->

# Problems with Paging

- Last lecture, we looked at the paging scheme and realized there are two main
  problems with the way things are organized as pages in the operating system.
  - Well actually three problems but we said we are not going to care about the
    internal fragmentation problem.

- The first problem stems from the fact that we need two memory accesses for
  each valid memory access to fetch things from main memory.
  - We must first go to the page table, (which is stored in main memory itself),
    get the address translation and find out the physical address from it.
  - We then build the full physical address (by concatenating the frame number
    with the page offset) and we go with that to main memory again to fetch the
    right set of bytes from memory.
- This is definitely slower than just using direct memory lookup, so we must
  devise a way to deal with it.

- The second problem stems from the fact that the page table is nothing but a
  huge array that contains the one-to-one mappings between the virtual page
  numbers and the physical frame numbers.
  - Recall that in our example, if each PTE is 4 bytes, then the size of the
    page table is 4MB, which is huge for every process.
  - So we must find a way to reduce the size impact of the page table.

# Caching to Speedup Access

- To address the problem of the two memory accesses to fetch a single address,
  we will employ a technique that is often used to speed up memory accesses.
- That is the mechanism of caching!
- Each process will store its most frequently used page translation in a cache,
  called the translation lookaside buffer (TLB).
- For every memory access, say `a[1]`, the operating system will first quickly
  check the TLB for the translation of the virtual address into a physical
  address.
  - If the translation is found, the operating system will go and fetch the
    physical address directly from memory.
    - We have saved a memory access and gained a speedup in execution.
    - This is called a TLB hit.
  - If the translation is not found, then we incur a TLB miss.
    - The operating system must go fetch the page table and find the translation
      in the page table.
    - Then the OS will fetch the actual physical address.
    - The OS will also update the TLB with the mapping that it has found in the
      page table so that it can speed up access to it later on.

- The concept of caching relies on two import properties in user programs:
  1. Temporal locality: If a process accesses a memory location, it is likely to
     access it again soon.
  2. Temporal locality: If a process accesses a memory location, it is likely to
     access a location close to it soon.
- So the TLB will actually fetch a few more page table translations and place
  them in the TLB.
- ❓ What if the TLB is full?
  - If the TLB could fit the entire page table, then we'd be okay. 
  - But practically it cannot, so what should we do?
- You will go into more information about caching Comp Arch II.

# Multi-level Page Tables

- To deal with the fact that page tables are necessarily huge, we must figure
  out a way to reduce the memory footprint of the page tables.
- The main motivator that we are going to be building upon here is the fact that
  a process is not going to be mapping all of its pages.
- Some pages are not used by the process, so they will be invalid.
  - So why would we want to store invalid mappings in our page table?

- So the trick is that we would like to reduce the number of invalid mappings
  stored in the actual page table.
- And the solution that we will come up with is a simple extension of the paging
  mechanism, we will page the page table itself.

- We will subdivide the virtual address into several page directory indices.
- Each index points to a page of the page table, and then we must traverse each
  level to get to the final mapping of the page table.
- In other words, the page table itself is broken down into pages, with each
  page store the physical address of the next page that is part of the page
  table.
  - Until we get to the final level and we can retrieve the actual physical
    frame that we care about the offset into it.
- So the page table itself is sort of organized into a tree that we must
  traverse every time we have to get a physical frame address mapping for a
  virtual page number.

# The xv6 Paging Mechanism

- In xv6, each virtual address is 64 bits long, out of which only the lower 39
  bits are used.
  - The upper 25 bits are always 0 and are saved for future use if we have much
    larger memories.
- The address is broken down into a 3-level page table:
  - The top 9 bits of the address are the index into the second level page
    table.
  - The second 9 bits of the address are the index into the first level page
    table.
  - The next 9 bits of the address are in the index into the zero'th level page
    table, and that is the one that contains the actual physical frame number
    that we care about. 
  - The remaining 12 bits are the page offset since we are dealing with 4KB
    pages.
- We will walk through the code that translates a virtual address into a
  physical address in class.

