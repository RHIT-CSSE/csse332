---
layout: post
title: Project Memory Hint
readtime: true
date: Fri Aug 02 2024
---

# Accessing user space data from kernel space

Note that once you are in kernel space, it is very dangerous to access user
space data. So for example, if you would like your system call to fill in some
buffer for ther user, or fill in the content of a structure, or pretty much
doing anything to userspace data, you must be very careful.

To get around that, you can use `copyin` to copy data from user space to kernel
space, and `copyout` to copy data from kernel space to user space. For
reference, the code in `filestate` in `kernel/file.c` serves as a great example
of how to use `copyout` to copy a structure from kernel space to user space.

## Example

You have plenty of examples of reading system call arguments and copying them
into kernel space in the `xv6` code. Specifically, look at the different system
call implementation functions in `kernel/sysproc.c` and `kernel/pipe.c`.

Here's a more concrete example. Say we want to read an integer from userspace
into the kernel space, then the user must provide us with a pointer to that
integer (it doesn't matter which region of memory that pointer resides in). To
read it into the kernel, we'd do something like the following:

```c
int
my_kernel_function(uint64 *user_addr) {
  /* grab the process that caused the context switch to kernel space. */
  struct proc *p = myproc(); 
  int kernel_value, err;

  err = copyin(p->pagetable, &kernel_value, user_addr, sizeof(int));
  if(err == -1) {
    // error, something went really wrong, return an error or panic.
  }

  // do stuff with kernel_value and now copy it back to the user.
  err = copyout(p->pagetable, user_addr, &kernel_value, sizeof(int));
  if(err == -1) {
    // error, something went wrong, return an error or panic
  }
}
```

You can do very similar things to read in any type, including structures and so
on. But be careful that `copyin` and `copyout` do shallow copies, so if you
have pointers deep into a structure, you'll need to do multiple copy
operations. Try to avoid that as much as possible.

