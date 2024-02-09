---
layout: post
title: xv6 Tips and Tricks
readtime: true
date: Mon Feb 13 2023 
---

<!-- vim-markdown-toc GFM -->

* [Adding a userspace test code to xv6](#adding-a-userspace-test-code-to-xv6)
* [Adding a system call to xv6](#adding-a-system-call-to-xv6)
  * [Warning: Accessing user space data from kernel space](#warning-accessing-user-space-data-from-kernel-space)
* [Mixing RISC-V assembly with C](#mixing-risc-v-assembly-with-c)
  * [Compiling: The lazy way](#compiling-the-lazy-way)
  * [Compiling: The efficient way](#compiling-the-efficient-way)
* [Running on a single CPU](#running-on-a-single-cpu)

<!-- vim-markdown-toc -->

# Adding a userspace test code to xv6

Say you would like to add a new test source code to your xv6 system, let's call
it `xv6test.c`, to test a new system call or a new feature that you have added.
Here are the steps involved in doing that:

1. Create your test source code file under `user/xv6test.c`.

2. Write your test code in `xv6test.c`, something like
   ```c
   #include "kernel/types.h"
   #include "kernel/stat.h"
   #include "user/user.h"

   int main(int argc, char *argv[]) {
     printf("Hello from my test case in xv6\n");

     exit(0);
   }
   ```

3. Add your code file to the `Makefile` in the repository's top level directory.
   Specifically, look for the definition of the `UPROGS` variable and then add
   the name of your test file to the end of that list definition, and prefix it
   with an `_` character. For example, in my case, my `UPROGS` variable looks
   like:
   ```makefile
   UPROGS=\
          $U/_cat\
          $U/_echo\
          $U/_forktest\
          $U/_grep\
          $U/_init\
          $U/_kill\
          $U/_ln\
          $U/_ls\
          $U/_mkdir\
          $U/_rm\
          $U/_sh\
          $U/_stressfs\
          $U/_usertests\
          $U/_grind\
          $U/_wc\
          $U/_zombie\
          $U/_xv6test\
   ```
   **Do not copy and paste the above snippet, it will mess up the tabs in the
   Makefile**. Instead, add that last line yourself.

4. Compile and start xv6 using `make qemu`.

5. When xv6 drops into the shell, make sure that `xv6test` exists using `ls`.

6. Then run the `xv6test` program as follows:
   ```shell
   $ xv6test
   Hello from my test case in xv6
   $
   ```

# Adding a system call to xv6

For some of your projects, you might need to add a new system call and provide
support for the kernel to do things on behalf of the user (think of `fork`,
`wait`, `clone`, etc.). Let's say I would like to add a system call, let's call
it `spoon`, that accepts a void pointer as an argument, and then just prints
that address from kernel space. Here are the steps involved in setting this
system call up:

1. Add a prototype for `spoon` in `user/user.h` as follows:
   ```c
   int spoon(void*);
   ```

2. Add a stub for the system call in `user/usys.pl` as follows:
   ```c
   entry(spoon);
   ```

3. Add a system call number to `kernel/syscall.h` as follows:
   ```c
   #define SYS_spoon 22
   ```
   Note that `SYS` must be capitalized.

4. Add your system call to the list of system calls in `kernel/syscall.c` as
   follows:
   ```c
   static uint64 (*syscalls[])(void) = {
     /* ... other stuff here */
     [SYS_spoon]  sys_spoon,
   };
   ```

5. Also in `kernel/syscall.c`, add the signature of your system call to the list
   of externs:
   ```c
   extern uint64 sys_spoon(void);
   ```

6. In `kernel/def.h`, add the function call signature under the `proc.c` section
   of definitions (right around the `fork`, `exec`, etc. definitions):
   ```c
   uint64 spoon(void*);
   ```

7. In `kernel/sysproc.c`, add the function call to `sys_spoon` as follows:
   ```c
   uint64 sys_spoon(void)
   {
     // obtain the argument from the stack, we need some special handling
     uint64 addr;
     argaddr(0, &addr);
     return spoon((void*)addr);
   }
   ```

8. Finally, in `kernel/proc.c`, add the your actual system call implementation:
   ```c
   uint64 spoon(void *arg)
   {
     // Add your code here...
     printf("In spoon system call with argument %p\n", arg);
     return 0;
   }
   ```

9. Write a small test case for your system call, call it `spoon_test.c`, and add
   it to xv6 in the same way we did in the first section above. Mine looked
   something like this:
   ```c
   #include "kernel/types.h"
   #include "user/defs.h"

   int main(int argc, char *argv[]) {
     uint64 p = 0xdeadbeef;

     spoon((void*)p);

     exit(0);
   }
   ```

10. Run your test program and verify that it executes the system call:
   ```shell
   $ spoon_test
   In spoon system call with argument 0x00000000deadbeef
   $
   ```

## Warning: Accessing user space data from kernel space

Note that once you are in kernel space, it is very dangerous to access user
space data. So for example, if you would like your system call to fill in some
buffer for ther user, or fill in the content of a structure, or pretty much
doing anything to userspace data, you must be very careful.

To get around that, you can use `copyin` to copy data from user space to kernel
space, and `copyout` to copy data from kernel space to user space. I doubt you
will need those functions in your project, but in case you do, the code in
`filestate` in `kernel/file.c` serves as a great example of how to use `copyout`
to copy a structure from kernel space to user space.

### Example

You will have plenty of example of reading system call arguments and copying
them into kernel space in the xv6 code. Specifically, look at the different
system call implementation functions in `kernel/sysproc.c` and `kernel/pipe.c`.

Here's a more concrete example, say we want to read an integer from userspace
into the kernel space, then the user must provide us with a pointer to that
integer (it doesn't matter which region of memory that pointer resides in). To
read it into the kernel, we'd do something like the following:

```sh
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

# Mixing RISC-V assembly with C

In some cases, you might need to write some RISC-V assmebly code that you will
need to call from your C code. For example, let's consider that I want to write
a function in RISC-V assembly that adds two numbers and returns their results.
In other words, I would like to implement the below C function in assembly:
```c
int addfn(int x, int y) {
  return x + y;
}
```

To do this, a few things that we need to note. First, in RISC-V, function
arguments are passed through the `a0-7` and the return values should be put into
the registers `a0` and `a1`. Therefore, we can write the above function in
assembly as (you can add this to `user/addfn.S`):
```asm
    .text

    .globl addfn
addfn:
    /* add a0 to a1, those contain x and y, and store the result in a0
     * as a return value.
     */
    add a0, a0, a1
    ret
```

Note that since this is a very simply function that makes no function calls, we
don't need to worry about storing things on the stack and then loading them. I
don't think you will need to write anything more complicated in this project,
but if you need to, check out the [RISC-V calling
conventions](https://inst.eecs.berkeley.edu/~cs61c/resources/RISCV_Calling_Convention.pdf).

Now, we need a way to compile this function and then write some code to test it.
First, let's try to write the testing code. The way to do that is that we will
have to define the function as an `extern` to our file. So, create a test code
file, call it `user/addtest.c`, and test the function as follows:

```c
#include "kernel/types.h"
#include "user/user.h"

extern int addfn(int, int);

int main(int argc, char *argv[])
{
  int x = 1, y = 2, z;

  z = addfn(x, y);

  printf("Calling add(%d, %d) results in %d\n", x, y, z);

  exit(0);
}
```

Now off to compiling it. There are two ways of doing it, one is lazy and a bit
wasteful while the other is more efficient but requires more edits to
`Makefile`.

## Compiling: The lazy way

The first approach compiles the add function and includes it in **all** binaries
that are generated for the user. This wasterful because not all user programs
need this function, so it ends up occupying space that is unnecessary. Also, the
name `addfn` is now global, so no other program can define a function of the
same name, even if they are effectively independent. But sometimes, since we're
prototyping, lazy compilation is ok. Here are the steps:

1. In the `Makefile`, add the `_addtest` to the `UPROGS` variables:
   ```make
    UPROGS=\
      $U/_cat\
      $U/_echo\
      $U/_forktest\
      $U/_grep\
      $U/_init\
      $U/_kill\
      $U/_ln\
      $U/_ls\
      $U/_mkdir\
      $U/_rm\
      $U/_sh\
      $U/_stressfs\
      $U/_usertests\
      $U/_grind\
      $U/_wc\
      $U/_zombie\
      $U/_addtest\
   ```

2. Next, add the assembly file to the list of user libraries:
   ```make
   ULIB = $U/ulib.o $U/usys.o $U/printf.o $U/umalloc.o $U/addfn.o
   ```

3. Then, add a rule to compile the assembly file:
   ```
   $U/addfn.o: $U/addfn.S
       $(CC) $(CFLAGS) -c -o $U/addfn.o $U/addfn.S
   ```
   Again, please do not copy and paste the above line, you need to write it
   yourself with a \<tab\> character at the start of the second line, otherwise
   `make` will complain.

4. Finally, compile the kernel using `make qemu` and run the test code:
   ```shell
   xv6 kernel is booting

   hart 1 starting
   hart 2 starting
   init: starting sh
   $ addtest
   Calling add(1, 2) results in 3
   ```

## Compiling: The efficient way

To compile things in an efficient way, we need to link `addfn.o` only when
compiling `addtest` and nowhere else. To do so,

1. In the `Makefile`, add the `_addtest` to the `UPROGS` variables:
   ```make
    UPROGS=\
      $U/_cat\
      $U/_echo\
      $U/_forktest\
      $U/_grep\
      $U/_init\
      $U/_kill\
      $U/_ln\
      $U/_ls\
      $U/_mkdir\
      $U/_rm\
      $U/_sh\
      $U/_stressfs\
      $U/_usertests\
      $U/_grind\
      $U/_wc\
      $U/_zombie\
      $U/_addtest\
   ```

2. Add the following rules to first generate `addfn.o` and then link it with
   `addtest` as follows:
   ```
    $U/addfn.o : $U/addfn.S
      $(CC) $(CFLAGS) -c -o $U/addfn.o $U/addfn.S

    $U/_addtest: $U/addtest.o $U/addfn.o $(ULIB)
      $(LD) $(LDFLAGS) -N -e main -Ttext 0 -o $U/_addtest $U/addtest.o $U/addfn.o $(ULIB)
      $(OBJDUMP) -S $U/_addtest > $U/addtest.asm
   ```
   Again, please do not copy and paste the above line, you need to write it
   yourself with a \<tab\> character at the start of the second line, otherwise
   `make` will complain.

3. Continue with step 4 from above.

# Running on a single CPU

Sometimes, to debug things, it would be helpful if we can avoid concurrency
problems. To do so, we can ask `qemu` to run a single virtual CPU instead of the
3 that xv6 uses by default.

To do so, open the `Makefile`, find the line that says:
```make
ifndef CPUS
CPUS := 3
endif
```
and change that to
```make
ifndef CPUS
CPUS := 1
endif
```

Alternatively, you can pass the `CPUS` parameter to `make` when compiling the
code. First clean up everything using `make clean`, and then compile while
specifying the number of CPUs to use as follows:

```shell
$ make CPUS=1 qemu
```

When xv6 runs, you should see something like:

```shell
xv6 kernel is booting

init: starting sh
$
```

Note the missing lines compared to our regular execution:
```
hart 1 starting
hart 2 starting
```

