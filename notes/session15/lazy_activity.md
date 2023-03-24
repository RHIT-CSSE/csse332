---
layout: post
title: Lazy Paging Activity
readtime: true
date: Thu Mar 23 2023 
---

# Introduction

In this acitivit, we will ask our operating system to take on a lazy approach to
memory allocation. So when a process asks for more memory, using the `sbrk`
system call, the operation system does not really allocate any memory for that
process. Why? because programmers are bad, they often overallocate memory and
end up not using. So why give them all that memory if they don't really need it?

What we are going to do is to give a process a page of memory only when it tries
to access it, and not before. How are we going to do that? We are going to make
use of our good old friend, the segmentation fault.

## Getting the Source Code

For this acitivity, we will need to mess around with the code for the xv6
kernel. So before you start this activity, please make sure that all of your
code for the next lab is pushed to your own private repo, otherwise, we might
risk mixing things up and losing your progress.

### Getting on the right branch

Once you have everything saved, checkout the `paging` branch from the xv6
repository as follows:

```shell
[xv6-riscv-public] $ git fetch
[xv6-riscv-public] $ git checkout lazy
[xv6-riscv-public] $ git pull
```

# The Lazy Allocation Activity

So the idea that we'd like to implement is the following. When a process
requests an increase in its allocated size, we will increase it but not really
allocate the phyisical frames for that process. Instead, we will wait for that
process to attempt to access the pages that it neededs, and only then we
allocate the frames for it and map them into its page table.

## `sbrk`

The first thing to look at is the code for the `sbrk` system call. Under
`kernel/sysproc.c`, look at the function `sys_sbrk(void)` and notice the changes
we introduced. The function should look something like:

```c
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  // Activity: We will not actually grow the process's memory unless we really
  // want to. More trickery for the OS point of view.
  // if(growproc(n) < 0)
  //   return -1;
  // Instead, we will just increase the process's size without actually growing
  // it now.
  myproc()->sz += n;
  return addr;
}
```

Note that we have removed the code the actually grows the process' address
space, `growproc(n)`. Instead, we only increase the size variable for the
process, as shown in the line `myproc()->sz += n;`.

### Testing

Now compile and build xv6 using `make qemu`, and try to run any command, for
example `echo hi`, you should see something like this:

```shell
[xv6 shell] $ echo hi
usertrap(): unexpected scause 0x000000000000000f pid=3
            sepc=0x00000000000012c0 stval=0x0000000000005008
```

❓ What does the error message indicate? What are the `scause`, `sepc`, and
`stval` values?

Any time a process attempts to access an unallocated page, the hardware will
generate a page fault, and trigger a trap handler in the kernel. Checkout the
code in `kernel/trap.c` for more information. Specifically, look at
`usertrap(void)`.

## Handling Page Faults

Your task in this activity is to modify the xv6 trap handling so that when a
page fault happens for a page that is not mapped, the kernel will allocate a
physical frame for that page, and then map the page into the process's page
table.

### Some Hints

Here are some hints to help you on your task:

1. You can read the `scause` and `stval` registers using the `r_scause()` and
   `r_stval()` funcitons.

2. It would be helpful for you to better understand things to print a message
   that says something like this:
   ```c
   printf("Segmentation fault occurred at address %p, from process %s (%d)\n",
       fault_addr, p->name, p->pid);
   ```

3. If you cannot allocate a page for a process, make sure to set that process as
   killed using `setkilled(p);`.

4. Note that the faulty address might be an address within a page. How can we
   extract the page address from that faulty address?

5. To allocate a physical frame, you can ask the kernel to do that for you using
   the `kalloc()` function. Notice how `kalloc` does not accept any arguments,
   it only allocate a single page (of size 4096 bytes).

6. You will find the functions in `kernel/vm.c` very useful. Specifically,
   `uvmalloc` might be very inspirational.

7. If you need permission bits, you can use `PTE_R|PTE_W|PTE_U` to mark the page
   as readable, writable, and user accessible.

## Testing

Once you have your implementation ready, look into the `user/lazy.c` program and
run it while experimenting with its values. I recommend that you first try to
run it as follows:

```shell
[xv6 shell] $ lazy 1024 10
```

❓ How many segementation faults do you see? Why is there this many?

Now, try to use different arguments, something like

```shell
[xv6 shell] $ lazy 4096 10
```

❓ How many segementation faults do you see? Why is there this many?

Experiment with the arguments and to make sure you understand how things
operate. Ask you instructor questions as you doing so.

