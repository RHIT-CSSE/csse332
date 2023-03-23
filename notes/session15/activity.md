---
layout: post
title: Paging Activity
readtime: true
date: Thu Mar 23 2023 
---

# Introduction

In this activity, we will explore some of the early stages of implementing
paging in xv6. Unfortunately, paging is dictated by the architecture (RISC-V) in
this case, so we cannot really change how it is structured, but nothing is
stopping us from playing around with it.

## Getting the Source Code

For this acitivity, we will need to mess around with the code for the xv6
kernel. So before you start this activity, please make sure that all of your
code for the next lab is pushed to your own private repo, otherwise, we might
risk mixing things up and losing your progress.

### Backing up your lab code 

If you have made a private xv6 repo, first commit and push all of your changes
to your repo. You can do so using:

```shell
[xv6-riscv-public] $ git commit -am "progress on xv6 lab"
[xv6-riscv-public] $ git push
```

If you have not made a private xv6 repo, we strongly recommend you do so! For
now, in order to not loose your progress, clone the xv6 repository again in a
different location, something like:

```shell
[~] $ mkdir activities
[~] $ cd activities
[~/activities] $ git clone https://github.com/rhit-csse332/xv6-riscv-public.git
```

### Getting on the right branch

Once you have everything saved, checkout the `paging` branch from the xv6
repository as follows:

```shell
[xv6-riscv-public] $ git fetch
[xv6-riscv-public] $ git checkout paging
[xv6-riscv-public] $ git pull
```

# The Activity

What we would like to do in this activity is to examine the mappings from
virtual addresses to physical addresses in xv6. Normally, a process cannot
access its phyical address (think about why?) but we will expose those in this
activity so we could learn more.

The goal of this activity is to implement a system call that, given a virtual
address, returns the address of the physical frame of memory that corresponds to
the page that the address belongs to. The system call we are implementing is
called `getmaping` and it has the following signature:

```c
int getmaping(uint64 va, uint64 pa_addr);
```

where `va` is the virtual address we are looking up, and `pa_addr` is a pointer
to a pointer where we would like to store the physical frame address.

❓ Why do we have to do this level of indirection to write a value back to the
user?

To use it, the system, you could do something like:

```c
// the physical address we map to
uint64 pa;

// allocate some virtual address
char *p = malloc(4);

// make the system call, pa will contain the physical frame address
getmaping((uint64)p, (uint64)&pa);
```

To help you out, we have provided you with a userspace program that makes
several calls to the `getmaping` system call and prints out some valid and
invalid mappings. You can find the source code for the program under
`user/map.c`, you can run from the xv6 shell using

```shell
[xv6 shell] $ map 1024
############### CHILD (4) ###############
 Virtual Address                  Physical Address
------------------               -------------------
0x0000000000013C00               0x0000000000000000

Checking for invalid address 0x0000000000014C00 -->  0x0000000000000000
############### CHILD (4) ###############

############### PARENT (3) ###############
 Virtual Address                  Physical Address
------------------               -------------------
0x0000000000013C00               0x0000000000000000

Checking for invalid address 0x0000000000014C00 -->  0x0000000000000000
############### PARENT (3) ###############
```

Currently, all addresses will show up as zeros, since we have not implemented
the system call completely. Your job in this activity is to implement the system
call.

## Implementing `getmaping`

Systems calls are called from userspace, but they cause a trap into the kernel
so that the kernel can execute some service on behalf of the user process. We
have set everything up for you to add the `getmaping` system call. Your job is
to implement the functionality of translating the virtual address into a
physical address.

In `kernel/sysproc.c`, at the end of the file, you will find a function called
`sys_getmaping(void)`. This is the function that the kernel executes when the
user calls the `getmaping` system call.

Your job now is to add your code to line 104 so that you can translate the
virtual address `va` into its corresponding physical frame address `pa`.

_Hint 1_: To get a reference to the process that issues the system call, you can
use the `myproc()` call. It returns `struct proc *` pointer that represents the
PCB of the process that issued the system call.

_Hint 2_: You will find the functions defined in `kernel/vm.c` very helpful for
this activity.

