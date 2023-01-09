---
layout: post
title: Paging
readtime: true
date: Sun Jan  8 2023 
---

# Learning Objectives

At the end of this lecture, you should be able to:

- Traverse a page table for a given process in the xv6 kernel.
- Define when a page table entry is a page directory or is a leaf page.
- Implement a function called `vmprint` to print a given process's page table.

---

# Topics

In this lecture, we will cover the following topics:

- Page Tables.
- Page Table Traversal in xv6.

---

# Notes


<!-- vim-markdown-toc GFM -->

* [Some Definitions](#some-definitions)
* [Page Table Manipulation](#page-table-manipulation)
  * [The `walkaddr` Function](#the-walkaddr-function)
  * [The `walk` Function](#the-walk-function)
* [Other Relevant Functions](#other-relevant-functions)
* [Implementing `vmprint`](#implementing-vmprint)
  * [Obtaining the Source Code](#obtaining-the-source-code)
  * [Implementing `vmprint`](#implementing-vmprint-1)
    * [Possible Implementation Plan](#possible-implementation-plan)

<!-- vim-markdown-toc -->

# Some Definitions

- We will first explore the xv6 code that is used to manipulate a process's page
  table.
- For the RISC-V architecture used in xv6, all addresses are 64 bits. However,
  only 39 bits of those 64 are actually used.
  - The top 25 bits are unused, and are assumed to be 0.
  - The next set of 9 bits are a first level page directory index.
  - The next set of 9 bits are a second level page directory index.
  - The next set of 9 bits are a third level page directory index.
  - Finally, the last set of 12 bits are the page offset in the final physical
    frame.
- Moreover, each page table entry (PTE) is 64 bits wide.
  - The top 10 bits are unused.
  - The next 44 bits are the physical frame number.
    - They can represent the address of a physical data page, or
    - they can represent the address of another level page table.
  - The last set of 10 bits are reserved for protection bits.
- This means that each page table contains 512 PTEs.

- The file `kernel/riscv.h` contains a bunch of useful macros for manipulating a
  virtual address, a physical address, and a PTE.

- In xv6, a page table is represented by 64 bit address as follows:
  ```c
  typedef uint64 *pagetable_t;
  ```

- Similarly, a PTE is also represented as a 64 bit address:
  ```c
  typedef uint64 pte_t;
  ```

# Page Table Manipulation

## The `walkaddr` Function

- In xv6, the function `walkaddr` in `kernel/vm.c` takes a page table and a
  virtual address `va`, and returns a corresponding physical address `pa`.
  - The function first walks the page table looking for the virtual address's
    PTE using `pte = walk(pagetable, va, 0);`. We will look at the `walk`
    function shortly.
  - After obtaining the corresponding PTE, the function checks for some
    conditions:
    - If the PTE is 0, then the page is not mapped in the page table.
    - If the PTE is not valid (i.e., if `*pte & PTE_V == 0`, then the page is
      not mapped in the page table.
    - If the PTE is not user accessible (i.e., if `*pte & PTE_U == 0`, then the
      page is not accessible by the user and an invalid physical address is
      returned.
  - From the corresponding PTE, you can use the `PTE2PA` macro to obtain the
    corresponding physical frame address from the PTE.
    ```c
    pa = PTE2PA(*pte);
    ```
  - Checkout the `kernel/riscv.h` file for the definition of the `PTE2PA` macro.

## The `walk` Function

- The `walk` function in `kernel/vm.c` is the building block of `walkaddr` and
  any traversal of a give page table.
- It accepts a page table and a virtual page address as arguments (let's ignore
  the `alloc` argument for now), and returns the corresponding leaf page table
  entry for the virtual page address. Actually, it returns the address of that
  PTE.
- `walk` starts from the level 2 page directory index and traverses the page
  table level by level until it hits a valid leaf PTE.
- To get a PTE from a page table and a virtual address `va`, `walk` uses the
  `PX(level, va)` macro that extracts the page directory index from `va` at the
  corresponding `level`.
  - It then indexes into the corresponding page table and obtains the PTE.
    ```c
    pte_t *pte = &pagetable[PX(level, va)];
    ```
- If the PTE is valid, then it maps into a lower level page table (or a leaf
  physical frame or it's the last level).
- The statement `pagetable = (pagetable_t)PTE2PA(*pte);` first extracts a
  physical frame number from a given PTE using the `PTE2PA` macro defined in
  `kernel/riscv.h`, and then casts that address into another page table.
- Once it reaches level 0, `walk` will return the address of the PTE in the last
  level page table as follows:
  ```c
  return &pagetable[PX(0, va)];
  ```

# Other Relevant Functions

- Here is a list of some other relevant functions in `kernel/vm.c`:
  
  1. `mappages` maps a memory region of size `size` that starts at virtual
     address `va` into a page table `pagetable`. It creates PTEs in each level
     of the page table by setting the `alloc` argument of `walk` to 1, and then
     marks each mapped PTE as valid.

  2. `uvmunmap` unmaps `npages` pages starting a virtual address `va` from the
     input page table. If the `do_free` argument is set, then the function will
     also free allocated frames for each mapped page.

  3. `uvmcreate` creates an empty page table.

  4. `freewalk` recursively frees all the pages that have been allocated for a
     page table. Note that this function should never encounter leaf frames.

  5. `uvmcopy` copies a parent's page table into a child's page table by copying
     all of the mapped pages from the parent to the child. This function is at
     the core of the implementation of the `fork()` system call, and is one that
     we will extensively revisit in lab 5.

# Implementing `vmprint`

- In this activity, we would like to implement the function `vmprint` that you
  find at the end of the `kernel/vm.c` file.
- This function simply accepts a page table as an argument and prints that page
  level by level.
- We will use the recursive `printpte` function defined right before it as a
  helper in print out all levels of a given page table.

## Obtaining the Source Code

- From the xv6 source code, checkout the `vmprint` branch as follows:
  ```shell
  $ git fetch
  $ git checkout vmprint
  $ git pull
  ```
- You can check the branch that you are on using `git branch`.
- Make sure that you can see the empty `vmprint` and `printpte` functions at the
  end of the `kernel/vm.c` file.

- Currently, the code will call the `vmprint` function every time the process
  with `pid = 1` is `exec`'ed. You can check out the code for that in
  `kernel/exec.c` in the `exec()` function:
  ```c
  if(p->pid == 1) vmprint(p->pagetable);
  ```

## Implementing `vmprint`

- Your task for the rest of this activity is to implement the code in `vmprint`
  and `printpte`.
- You will find a lot of inspiration in the `freewalk` function.
- When running xv6 with `vmprint` implemented, your output should look like the
  following:

```shell
$ make qemu
...
xv6 kernel is booting

hart 1 starting
hart 2 starting
page table 0x0000000087f6c000
..0: pte 0x0000000021fda001 pa 0x0000000087f68000
.. ..0: pte 0x0000000021fd9c01 pa 0x0000000087f67000
.. .. ..0: pte 0x0000000021fda41b pa 0x0000000087f69000
.. .. ..1: pte 0x0000000021fd9817 pa 0x0000000087f66000
.. .. ..2: pte 0x0000000021fd9407 pa 0x0000000087f65000
.. .. ..3: pte 0x0000000021fd9017 pa 0x0000000087f64000
..255: pte 0x0000000021fdac01 pa 0x0000000087f6b000
.. ..511: pte 0x0000000021fda801 pa 0x0000000087f6a000
.. .. ..510: pte 0x0000000021fdd007 pa 0x0000000087f74000
.. .. ..511: pte 0x0000000020001c0b pa 0x0000000080007000
init: starting sh
$
```

- Note that the physical mapped addresses that you see above might be different
  in your case, but your output should look similar.

### Possible Implementation Plan

- Here's a possible plan of attack:
  - Look at `freewalk` for inspiration, the code there will prove to be very
    helpful.
  - For each level, traverse all 512 PTE of the corresponding page table.
    - If the PTE is valid, but is neither readable, write, nor executable, then
      the PTE points to an inner level page table. This is when you need to make
      the recursive call to `printpte`.
    - If the PTE is valid and is either readable, writable, or executable, then
      the PTE points to a leaf data frame. In that case, you simple need to
      print the mapping.
  - The `prefix` argument of the `printpte` function might prove to be useful in
    printing the `..` that you can see at each level of the page table in the
    output above.

