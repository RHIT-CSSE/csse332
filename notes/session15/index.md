---
layout: post
title: Paging
readtime: true
date: Thu Mar 23 18:24:18 2023 
---

# Learning Objectives

At the end of this lecture, you should be able to:

- Draw a paging mechanism that involves the steps needed to translate a virtual
  page address into a physical frame address.

---

# Topics

In this lecture, we will cover the following topics:

- Paging.
- Shortcomings of paging.

---

# Notes

<!-- vim-markdown-toc GFM -->

* [Activity](#activity)
* [The Page Table](#the-page-table)
  * [Page Table Content](#page-table-content)
  * [Exercise](#exercise)
* [Problems with Paging](#problems-with-paging)
  * [Slow Memory Lookup](#slow-memory-lookup)
    * [The Translation Lookaside Buffer](#the-translation-lookaside-buffer)
  * [Large Page Tables](#large-page-tables)
    * [Multi-level Page Tables](#multi-level-page-tables)

<!-- vim-markdown-toc -->

# Activity

In this lecture, we will do two small activities in xv6 to see paging in
practice.  Please checkout the [first](activity/) and [second](lazy_activity/)
activity pages for more information.  

# The Page Table

- Each process will have its own page table that is maintained by the operation
  system.
- Every time a new page of memory is created, the operating system creates a
  mapping for that page in the page table and allocates a corresponding physical
  page for it.
- Every time a user wants to access an address in memory, the following happens:
  1. The operating system reads the page number from the address.
  2. The operating system indexes into the page table with that index and
     fetches the corresponding physical frame number.
  3. The operating system builds the physical address by concatenating the
     physical frame number with the frame offset bits from the user-provided
     address.
  4. The operating system sends the built address on the bus and obtains and the
     corresponding word.

- The address of each process's page table is maintained int the process's
  process control block (PCB).
- In xv6, this looks like the following (located in `kernel/proc.h`):

```c
// Per-process state
struct proc {
  struct spinlock lock;

  // p->lock must be held when using these:
  enum procstate state;        // Process state
  void *chan;                  // If non-zero, sleeping on chan
  int killed;                  // If non-zero, have been killed
  int xstate;                  // Exit status to be returned to parent's wait
  int pid;                     // Process ID

  // wait_lock must be held when using this:
  struct proc *parent;         // Parent process

  // these are private to the process, so p->lock need not be held.
  uint64 kstack;               // Virtual address of kernel stack
  uint64 sz;                   // Size of process memory (bytes)
  pagetable_t pagetable;       // User page table
  struct trapframe *trapframe; // data page for trampoline.S
  struct context context;      // swtch() here to run process
  struct file *ofile[NOFILE];  // Open files
  struct inode *cwd;           // Current directory
  char name[16];               // Process name (debugging)
};

```

- Anytime a process is created (in `allocproc()` in `kernel/proc.c`), the kernel
  will create an empty (kind of) page table for that process as follows:

```c
pagetable_t
proc_pagetable(struct proc *p)
{
  pagetable_t pagetable;

  // An empty page table.
  pagetable = uvmcreate();
  if(pagetable == 0)
    return 0;

  // map the trampoline code (for system call return)
  // at the highest user virtual address.
  // only the supervisor uses it, on the way
  // to/from user space, so not PTE_U.
  if(mappages(pagetable, TRAMPOLINE, PGSIZE,
              (uint64)trampoline, PTE_R | PTE_X) < 0){
    uvmfree(pagetable, 0);
    return 0;
  }

  // map the trapframe page just below the trampoline page, for
  // trampoline.S.
  if(mappages(pagetable, TRAPFRAME, PGSIZE,
              (uint64)(p->trapframe), PTE_R | PTE_W) < 0){
    uvmunmap(pagetable, TRAMPOLINE, 1, 0);
    uvmfree(pagetable, 0);
    return 0;
  }

  return pagetable;
}
```

## Page Table Content

- Each page table is composed of multiple page table entries (PTEs).
- Each PTE contains the translation between a page address and a frame address.
- In addition to that, each PTE will also contain some metadata bits that
  represent the following information:
  - A valid bit (V) that indicates whether the translation is valid or not.
  - A write bit (W) that indicates whether the page can be written or not.
  - A read bit (R) that indicates whether the page can be read or not.
  - An execute bit (X) that indicates whether the page can be executed or not.
  - A user bit (U) that indicates whether the page can be accessed by the user
    or not (for example, kernel pages should not be accessed by the user at
    all).
- Note that above bits can change between architectures, and the ones we list
  above are those specific for RISC-V.
- You can check the definition of those bits in `kernel/riscv.h` as follows:

```c
#define PTE_V (1L << 0) // valid
#define PTE_R (1L << 1)
#define PTE_W (1L << 2)
#define PTE_X (1L << 3)
#define PTE_U (1L << 4) // user can access
```

## Exercise

- 🏃 Let's suppose that we have a 32-bit machine with 4KB pages.
  - ❓ How many pages can a process have?
    - `2 ** 32 / 2 ** (2 + 10) = 2 ** 20` pages.
  - ❓ How many bits do we need for the page number in the address?
    - 20 bits.
  - ❓ How many bits do we need for the frame offset in the address?
    - 12 bits.
  - ❓ How large is our page table? You can assume that each PTE is 4
    bytes.
    - `4 * (2 ** 20) = 2 ** 22 = 4MB`.

# Problems with Paging

- ❓ What can you think might be some problems with the current setup of
  paging?

- First, let's think about accessing memory, say at the address `a[1]`, what
  does that involve?
  - The operating system will first need to fetch the page table from memory.
  - Then it index the page table with the page index from the virtual address
    and obtain and frame index. 
  - Then it will go back to memory to fetch the frame and offset into it using
    the page offset from the provided address.
- Therefore, to make a single memory access for `a[1]`, we must do __two__
  memory accesses:
  1. One to fetch the page table and get the frame index from it.
  2. One to fetch the actual frame from memory and offset into it using the page
     offset.
- ❓ What do you think we could do to speed up the process of accessing
  the page table?

- Second, let's go back to our previous example, for a 32-bit machine with 4KB
  pages, we need __4MB__ of memory space just to store the page table.
- So there is a large memory footprint for storing the page table, which is bad
  because we have _one page table for each process_, plus the page table for the
  operating system.
- Even worse, often processes don't end up using all of their memory address
  space.
  - So we store a lot of invalid mappings in the page table that we do not end
    up using at all.
- ❓ What do you think we can to reduce the size of the page table?
  -  Think about what we did when we had a large segment of memory in the
    segmentation case?


## Slow Memory Lookup

### The Translation Lookaside Buffer

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

## Large Page Tables

### Multi-level Page Tables

- To address the issue of large page tables, we will treat the page table just
  like any other large memory region.
- So we will actually page the page table and create a multi-level page table. 
- We will discuss this more in depth tomorrow!


