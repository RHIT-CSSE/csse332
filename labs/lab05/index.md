---
layout: post
title: Lab 05 -- Copy on Write Fork
readtime: true
date: Friday Jan 6 2023
---

# Introduction 

The `fork()` system call in xv6, and as we have seen it so far in class, copies
all of the parent's address space into the child's address space. This is okay
if the process's address space is small, but if the parent is large, then this
becomes very slow. Also, most often, a lot of the pages that are shared between
the parent and the child are never written to. So keeping two copies of the same
page that is only being read is very wasteful.  Furthermore, copies of the full
address space might wasteful since calls to `fork()` are often followed by calls
to `exec()` in the child, which completely replaces the address space of the
child. In that case, the copy that we did on `fork()` was very wasteful.

In this lab, we will implement an optimization to `fork()` referred to as
__copy-on-write__ (COW). Note that this is a challenge assignment and will count
towards your challenges part of the grade.

## Learning Objectives

At the end of this lab, you should be able to:
- Implement cow forking in the xv6 operating system.
- Implement reference counting as a way to track shared pages before freeing
  them.
- Manipulate process page tables in the xv6 operating system.

# Overview

Unlike our regular `fork()`, a COW `fork()` will defer the allocation and
copying of physical memory frame for the child until the copies are really
needed, if ever.

COW `fork()` creates just a page table for the child, which PTEs point to the
parent's physical frames, which are now shared between the parent and the child.
However, COW `fork()` marks all PTEs, in both the parent and the child, as
read-only. In other words, neither the parent nor the child can write to their
pages after a `fork()` system call.

When either the parent or the child attempt to write to a page, a page fault
occurs and the kernel is called upon to handle the exception. The kernel will
detect a COW protected page and will then allocate a frame of physical memory
for the faulting process, copy the content of the original page to the new one,
maps the new frame in the faulty process's page table, and unmaps the old page
appropriately. However, when mapped in the page table, the kernel marks the
corresponding PTE as writable now, and thus the faulty process can continue
execution normally after that.

The COW `fork()` makes freeing physical frame trickier. A frame of memory may be
mapped in multiple processes's page tables, and thus cannot be really free'd
back to the kernel until the last mapping is remove. We will explore this tricky
step in this lab.

# Getting the Source Code

To obtain the updated source code, make sure to fetch the `lab05` branch of the
xv6 source code repository. To do so, from your xv6 top level directory, issue
the following commands:

```shell
$ git fetch
$ git checkout lab05
$ git pull
```

You should be on a new branch that is called `lab05`. You can verify that using:

```shell
$ git branch
```

# Step 0: Making Sure Tests Fail

In this first step, we just want to make sure that your code fails the first
simple test case of our tests. We have provided you with a set of test cases
that tests various `fork()` behavior. The first of those tests, called `simple`,
allocates more than half of the available physical memory for one process, and
then calls `fork()`. This fork will fail because there is not enough space in
the system to create a copy of the parent process, with all of its address
space.

To run this, first compile xv6 using `make qemu`, and then run the `cowtest`
executable using:

```shell
$ cowtest
simple: fork() failed
```

Your output should show exactly the output shown above. Once that is done, you
are ready to move on to Step 1.

# Step 0.5: Identifying Segmentation Faults

The default behavior of xv6 when it encounters a segmentation fault is that it
will kill the faulty process. What really happens behind the scenes is that a
user trap is encountered when accessing an unmapped page in the page table, and
the code in `usertrap` in `kernel/trap.c` is triggered.

Specifically, when a page fault happens, the hardware will trigger an exception.
In RISC-V, the hardware will the cause of the exception in the `scause`
register, save the faulty PC in `sepc`, and the value of the pointer that caused
the page fault in the `stval` register. Then, the hardware will trap into the
kernel to handle the page fault, which will then call on `usertrap` to decipher
what happens, since this is an exception that happened from user space.

Take a look at the code in `kernel/trap.c`, specifically at `usertrap()` for a
sample of what happens. You can see for example that if the trap is caused by a
system call issued from the user, the value of the `scause` register will be 8
as shown in the following snippet of code:
```c
if(r_scause() == 8) {
  // system call
  ...
}
```

As it currently stands, xv6 does not have a handler for segmentation faults.
Instead, the kernel will kill the process if it is caused by an unhandled trap.
In order to figure out what happens when a page fault occurs, let's create one.
In the user directory, we have provided you with a file called `user/fault.c`
that forces a page fault as follows:
```c
int
main(int argc, char **argv)
{
  char *p = (char *)0xdeadbeef;

  /* Try to access an invalid pointer and check what happens. */
  *p = 0xff;
  printf("The ptr %p points to %c", p, *p);

  exit(0);
}
```

To run this program, compile the xv6 kernel using `make qemu` and run the
`fault` executable as follows:
```shell
$ fault
usertrap(): unexpected scause 0x000000000000000f pid=3
            sepc=0x0000000000000010 stval=0x00000000deadbeef
```

You can see that for a page fault, the `scause` register contains the value 15
(0xf in hex), and the `stval` register contains the address that caused the
fault (`0xdeadbeef` in our case). We can therefore modify the source code for
`usertrap()` to print a segmentation fault as follows:
```c
else if((which_dev == devintr()) != 0) {
  // ok
} else if(r_scause() == 15) {
  printf("Segmentation fault from process %d at address %p\n", p->pid, r_stval());
  p->killed = 1
} else {
  ...
}
```

Now compiling this code using `make qemu` and running the `fault` executable
results in:
```shell
$ fault
Segmentation fault from process 3 at address 0x00000000deadbeef
$
```

We will return to this handler later on in Step 2.

# Step 1: Modifying `fork` Behavior

## Understanding the Copying `fork` Behavior

First, take a look at the implementation of the `fork()` system call. You can
find the source code in `kernel/proc.c`. The most important snippet of code we
care about is the following:
```c
// Copy user memory from parent to child.
if(uvmcopy(p->pagetable, np->pagetable, p->sz) < 0){
  freeproc(np);
  release(&np->lock);
  return -1;
}
np->sz = p->sz;
```

This code will copy the parent process (called `p`)'s address space and create a
copy of each page in the child process's address space. `uvmcopy` copies all of
the mapped pages in the parent's address space and creates corresponding pages
in the child's address space, and maps them in the child's page table.  You can
find the source code for `uvmcopy` in `kernel/vm.c`.

Specifically, we care about the following loop in `uvmcopy`:
```c
for(i = 0; i < sz; i += PGSIZE){
  if((pte = walk(old, i, 0)) == 0)
    panic("uvmcopy: pte should exist");
  if((*pte & PTE_V) == 0)
    panic("uvmcopy: page not present");
  pa = PTE2PA(*pte);
  flags = PTE_FLAGS(*pte);
  if((mem = kalloc()) == 0)
    goto err;
  memmove(mem, (char*)pa, PGSIZE);
  if(mappages(new, i, PGSIZE, (uint64)mem, flags) != 0){
    kfree(mem);
    goto err;
  }
}
```

This code will first walk the parent's page table for each virtual page number
in the parent's address space. It does some error checking and then allocates a
page in the child's address space using:
```c
if((mem = kalloc()) == 0)
  goto err;
```
and then copies the content of the old page into the new page using:
```c
memmove(mem, (char*)pa, PGSIZE);
```

Finally, we create a mapping for the new page in the child's page table using:
```c
if(mappages(new, i, PGSIZE, (uint64)mem, flags) != 0){
  kfree(mem);
  goto err;
}
```

## Implementation: Mapping the Parent's Pages Without Copy

Your first task in this lab is to modify the `uvmcopy` function to create
non-writable pages that are shared between the parent and the child. In other
words, you will __NOT__ be creating new pages for the child, instead all the
mappings in the parent's page table will be the same in the child's page table,
and all the pages in the shared address space will be marked as non-writable. 

The goal of making those pages non-writable is to cause segmentation faults to
occur when either the parent or the child try to write to these pages. Then, it
is only at that point that we create new pages, copy the content of the old
pages, and remap those pages in the corresponding process's page table.

However, this creates a small conundrum. What if a page was originally marked as
non-writable? At that point, we do not want the kernel to copy and remap that
page in the page table, we would like to keep that page non-writable. Therefore,
to achieve a distinction between pages that are non-writable because of
the copy-on-write mechanism, and those that are non-writable by default, we will
make use of an additional bit each page table entry (PTE). This bit is called
the RSW bit (where RSW stands for Reserved for SoftWare). Therefore, we will set
that bit to 1 whenever a page is marked non-writable because of the cow
mechanism. We have already defined that bit for you in the `kernel/riscv.h`
definitions as follows:
```c
#define PTE_RSW (1L << 5) // reserved for software
```

Here is a reasonable plan of attack:
1. Remove the code that creates a new page and copies the content of the old
   page into it from `uvmcopy` in `kernel/vm.c`.
2. For each mapped page in the parent's page table, unset the Write flag. To
   unset a flag from a given PTE, you can use:
   ```c
   *pte &= ~PTE_W;
   ```
3. For each mapped page in the parent's page table, set the RSW bit. To do so,
   you can use something like the following:
   ```c
   *pte |= PTE_RSW;
   ```
4. Map the page as is, with the same flags, in the child's page table.

At this point, your xv6 kernel will break. Recall that `fork()` is an essential
system call in everything the operating system does. And as of now, when we fork
a process, all of the pages for both the parent and child are marked as
non-writable, which will cause our operating system to break. 

When I tried to run my kernel using `make qemu`, I get something like the
following:
```shell
xv6 kernel is booting

hart 1 starting
hart 2 starting
init: starting sh
usertrap(): unexpected scause 0x000000000000000c pid=1
            sepc=0x0000000000000394 stval=0x0000000000000394
panic: init exiting
```

Your output might look slightly differently, but basically the `init` process
should fail since we cannot `fork()` properly. We will take care of that in the
next step.

# Step 2: Handling Segmentation Faults

It is now time to go back to our page fault handler to make sure that we create
new pages and copy then when we encounter a page fault caused by a cow mapping.
What we would like to do in this step is to modify the page fault handler that
we created in Step 0.5 to handle cow mapping. 

Here is a reasonable plan of attack:
1. For the fault address, obtain the current mapping in the faulty process's
   page table. In other words, for the faulty page, obtain the page table entry
   that corresponds to it.
   > Hint: To obtain the page address from a given virtual address, use the
     `PGROUNDDOWN` macro as follows:
     ```c
     va = PGROUNDDOWN(r_stval());
     ```
   > Hint: You might find the `walk` function call useful in this step.
   > Hint: For the `walk` function to become accessible from `trap.c`, at the
     top of the `trap.c` file, right after the `extern char tramploine[] ...`,
     add the following:
     ```c
     extern pte_t * walk(pagetable_t pagetable, uint64 va, int alloc);
     ```

2. For the found PTE, check that it is a cow protected page. If it is not, then
   simply mark the process as killed and continue.
   > Hint: A page is cow protected if it is (1) valid, (2) user accessible, (3)
     non-writable, and (4) has the RSW bit set.

3. If the PTE is cow protected, then we need to allocate a new page for it, copy
   the old page into the new page, and update the mapping of the page into the
   process's page table. Here's a reasonable breakdown of this step:

   1. Allocate a new physical page in memory using `kalloc()`.
   2. Copy the old physical page (the one already mapped in the page table) into
      the new physical page using `memmove`.
   3. Unmap the old page from the process's page table using `uvmunmap`.
      > Hint: Here is an example of using `uvmunmap` to unmap a page:
        ```c
        uvmunmap(p->pagetable, va, 1, 0);
        ```
   4. Map the new page into the process's page table using `mappages`. For this
      to work, you will need to copy the same PTE flags form the old pte, except
      with the `PTE_W` bit set and the `PTE_RSW` bit cleared.

## Testing

Implementing this step does completely solve all of our problems, but let's try
to test a few things out so far. The main problem that we have not handled yet
is what happens when a process dies, so by definition our tests will fail. We
will take care of that in the next step.

However, we have provided you with a partial test that would tell you if your
implementation is far is correct. Checkout the code in `user/simplefork.c`. This
is very similar to the `simple` test in `cowtest.c`, however it does not call
the `wait` system call since we haven't handled yet what happens when a process
dies.

To test things so far, compile the xv6 kernel using `make qemu` and run the
`simplefork` executable using:
```shell
$ simplefork
simple: ok
usertrap(): unexpected scause 0x000000000000000c pid=1
            sepc=0x0000000000000fa4 stval=0x0000000000001000
panic: init exiting
```

Note that the test passes but then the kernel panics. This is due to the fact
that we do not clean up after ourselves when doing COW. Let's take care of that
in the next step.

# Step 3: Adding Reference Counting

A big problem arises when we share pages between processes, and that is that of
cleaning up allocated memory. Under normal circumstances, when a process dies,
it will unmapp and free all of its allocated pages. However, under COW, that
page that we are freeing might still be shared with a child process, and that is
problematic if the child process still needs access to it.

This is a very similar problem to one that might arise when dealing with garbage
collection in many high level languages. For each physical frame in our system,
we must keep track of a __reference count__ that indicates how many processes
are still actively trying to access that frame. As long as the reference count
is greater than 0, we must not free that frame. It is only when the reference
count reaches 0 (i.e., no further processes need that frame) that we can
completely free the frame.

Your job in this step is to implement reference counting for each physical frame
in the xv6 system. You will need to modify the code in `kernel/kalloc.c` to keep
a reference count for each allocate frame in the system. The design of xv6 makes
things easy for us since we already know before hand the maximum number of
physical frames that can be allocated in our system. Check out the code in
`kernel/memlayout.h`, specifically,
```c
// the kernel expects there to be RAM
// for use by the kernel and user pages
// from physical address 0x80000000 to PHYSTOP.
#define KERNBASE 0x80000000L
#define PHYSTOP (KERNBASE + 128*1024*1024)
```
The macro `PHYSTOP` indicates that largest physical address that we can reach.
The macro `KERNBASE` is the address where the kernel's memory is allocated.
Therefore the total number of frames that can be allocated for the users is
`(PHYSTOP - KERNBASE) / PGSIZE`.

Here is a reasonable plan of attack:
1. In `kernel/kalloc.c`, add an array that can be use to keep track of the
   reference count of each physical frame as follows:
   ```c
   struct {
     struct spinlock lock;
     struct run *freelist;
     // ADDED CODE HERE
     int refcnt[(PHYSTOP - KERNBASE) / PGSIZE];
   } kmem;
   ```

2. For ease of use, add a macro that can allow you to get the reference count
   index from a physical frame address. Something like:
   ```c
   #define  FRINDEX(pa) ((uint64)pa - KERNBASE) / PGSIZE
   ```

3. Modify `kfree` in `kernel/kalloc.c` to do the following:
   1. For the given physical address to free, first decrement that frame's
      reference count. To avoid any parallelism errors, make sure to acquire the
      memory lock before accessing the reference count array. Something that
      looks like
      ```c
      acquire(&kmem.lock);
      // TODO: decrement the frame's reference count here
      release(&kmem.lock);
      ```
   2. If the frame's reference count becomes 0 or less, then actually free the
      frame by adding it back to the freelist (essentially execute the code that
      was already there in `kfree`). Make sure to reset the frame's reference
      count to 0.
   3. If the frame's reference count is still > 0, we are done, nothing else to
      add.
  
4. Modify the `freerange` function to initialize the reference count for each
   frame to 0 (you can also initialize it to 1 if you like, `kfree` will take
   care of resetting it anyway).
   > Note: You do not need to hold the lock over kmem at this point since the
   initialization code runs on a single processor of the available ones.

5. Modify the `kalloc` function to initialize the reference count for the
   allocated frame to 1.

6. In `uvmcopy`, anytime you share a page between the parent and the child
   processes, increase the reference count of that page's corresponding frame.
   To do so, I added a function to `kalloc.c` that increments the reference
   count for a frame, something like:
   ```c
   void increfcnt(uint64 pa) {
     // TODO: Add your code here.
   }
   ```
   and then extern the function in `vm.c` as follows:
   ```c
   extern void increfcnt(uint64);
   ```
   and then you can use it in `uvmcopy` whenever applicable.

7. In `usertrap`, every time a frame in unmapped, call `kfree` on that frame.
   Note that `kfree` will not actually free the frame until its reference count
   hits 0.

## Testing

Now we can test things out a bit more. To do so, uncomment the lines in
`user/simplefork.c`  that are marked with `UNCOMMENT THIS FOR STEP 3` (there is
one in the `simpletest()` function and one in `main`). Then, compile the kernel
using `make qemu` and run the `simplefork` executable as follows:

```shell
$ simplefork
simple: ok
simple: ok
$
```

Both simple tests should pass at this point. We can also make more extensive
tests using `cowtest`, however not all tests will pass. Specifically, `simple`
and `three` will pass, but `file` will fail, something that looks like the
following:
```shell
$ cowtest
simple: ok
simple: ok
three: ok
three: ok
three: ok
file: eerrorerrorr:r orrea: readd :  rfaeafdi laedifalielde
d

$
```

We will fix the `filetest` in the next and final step.

# Step 4: Modifying `copyout`

The final case we would like to take care of is the case of `copyout` in
`kernel/vm.c`. `copyout` is a function that copies bytes of data from the kernel
address space to the user's address space. When a process calls `copyout` on a
shared cow page, it must first create allocate a new page and copy it before it
can write to it.  However, it will not go through the same page fault mechanism
that we implemented in the previous steps since `copyout` is usually called in
kernel space, and thus has a different trap handler. To simplify things, we will
directly modify `copyout`'s source code to handle cow pages.

To complete this lab, modify the source code for `copyout` to handle cow pages
before writing to them. Your code will look very much similar to the code you
used in step 3. In a nutshell, what the source code of `copyout` is copying date
from the kernel space to the user space page by page. For each page to copy, the
code will first find `pa0`, which the address of the physical frame to write to,
and then calls `memmove` to copy the data from the kernel frames to the user's
frame. Your code must reside after the line `va0 = PGROUNDDOWN(dstva);` and it
should set the appropriate value for `pa0`.

> Hint: In the case where the virtual page to copy to is not a cow page, you
  simply need to set `pa0 = walkaddr(pagetable, va0);`.

> Hint: You will need to fail `copyout` if the user runs out of space. In other
  words, if `va0` is greater than `MAXVA`, then you need to return -1.

## Testing

Now, all `cowtest` tests should pass:
```shell
$ cowtest
simple: ok
simple: ok
three: ok
three: ok
three: ok
file: ok
ALL COW TESTS PASSED
```

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
Two files will be generated, `submit-lab5.patch` and `submit-lab5.tar`. **Please
submit both of these files to Gradescope** at the appropriate lab link.

## Submission Checklist

- [ ]  My code compiles and generates the right executables.
- [ ]  I ran `make grade` to double check the test cases for all of my code.
- [ ]  I ran `make submit` to generate the `.patch` and `.tar` files.
- [ ]  I submitted both `.patch` and `.tar` files to [Gradescope]({{
    site.gradescope_url }}).

