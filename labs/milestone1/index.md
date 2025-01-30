---
layout: post
title: Project Milestone 1
readtime: true
date: Fri Aug 02 2024
---

# Introduction

In this project, we would like to add a new feature to the xv6 operating
system. Specifically, the current implementation of xv6 provides support only
for processes, each with its own separate address space. That is enough to
implement basic features like a simple shell, however, to make use of the
available processors, we'd definitely need to share memory between lightweight
threads. Therefore, we will make it our task in this project to add support for
`pthread`-like threads in xv6.

In our context, two threads will live in the same process address space but
each will have the following:

1. Its separate set of registers, including its own program counter. This means
   that each thread can be executing in a different spot in the program's
   source code.
2. Its separate stack (the stack will live in the same address space, they
   will just be at separate locations).
3. Each thread is a separate schedulable entity. In other words, if there
   are two processors available, then both threads can be actively running at
   the same time.

To support creating and manipulating threads, we would also like to provide an
API for our users to create and join threads, in a way similar to what the
`pthreads` library provides. For the base project, you do not need to worry
about synchronization.

# Milestone 1: The theory, system calls, user tests, and an API

In the first milestone, we will do some admin work to construct the base of our
project. You will do the following:

- Read some theory about processes and memory management in the `xv6` operating
  system.

- Implement a sample system call and a sample user test.

- Implement stub system calls for creating a thread and joining a thread.

- Design an API that users of your library will use. _You don't have to
  implement the API, just what the functions would look like_.

## The theory

To get started, you **must** read chapters 3 and 4 of the [xv6
book](https://pdos.csail.mit.edu/6.828/2023/xv6/book-riscv-rev3.pdf); those
chapters are essential for you to understand how to design and implement
lightweight threads in the xv6 kernel. Please do read those chapters carefully
before considering answers to the below questions.

While you are reading those two chapters, please consider the following
questions. Be ready to provide answers for them during the project meetings
time.

1. Describe in detail the content of the page table of a process in xv6.

2. Looking back at our interrupts lecture, and with insights from chapter 4,
   describe in-depth the process of context switching between one process and
   another in xv6. Make sure to mark references to the kernel code, or code
   snippets to support your answer.

3. Describe how the `fork` system call works in `xv6`. Understanding `fork`
   well is **key** to a smooth flight through this project.

On a separate level, please consider the following questions when reading those
chapters. You might need to go back to your CSSE132 notes (or the equivalent
class).

> _Hint_: Referring back to our interrupts and exceptions lecture would be of
  great help here.

1. In RISC-V, how are arguments passed to a function? You can safely assume we
   have a limited number of arguments. You do not need to support a variable
   number of arguments.

2. As we saw with `pthreads`, each thread will start execution from a different
   place in the code. What registers would you need to manipulate to impact
   where your thread is going to start and where your thread will return to (if
   it does return) once it has completed its execution.

## System calls

To get started with your project, you would need to need the vanilla `xv6`
source code provided to you in the **`main` branch** of the class labs
repository. **Please make sure you are on the `main` branch to avoid having any
of the other labs' code** as those can interfere with your work.

The first thing we'd like to do is to add system calls to `xv6`. We will first
walk you through how to add a new system call (we call it `spoon`) along with a
user test file that you can use to test it.

Then, you will add empty stub system calls for the thread creating and thread
joining operations in your project. You do not need to implement those calls
yet, simply add them to `xv6` and have them print something like `This call is
not supported yet!`.

### Adding a user test file

To add a new test file to `xv6` (this pretty much means writing a user program
that runs on the `xv6` operating system), proceed as follows.

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

   > __Note__: If you use `vscode` and have C Intellisense configured, it will
   tend to reorder the `#include` above. **Please do not let it do so** as
   `xv6` is very sensitive to the order of these included files.

   > __Note__: For now, this simply prints out a line, we will add more code to
   test a new system call soon.

3. Add your code file to the `Makefile` in the repository's top level directory.
   Specifically, look for the definition of the `UPROGS` variable and then add
   the name of your test file to the end of that list definition, and prefix it
   with an `_` character.

   For example, in my case, my `UPROGS` variable looks like:
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

### Adding the `spoon` system call

Now that we have added a user program, let's add a new system call to `xv6`.

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
     /* ... Other stuff here */
     [SYS_spoon]  sys_spoon,
   };
   ```

5. Also in `kernel/syscall.c`, add the signature of your system call to the list
   of externs:
   ```c
   extern uint64 sys_spoon(void);
   ```

6. In `kernel/def.h`, add the function call signature under the `proc.c` section
   of definitions (right around the `fork`, `exec`, etc., definitions):
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

9. Modify `xv6_test.c` from the previous section to call this new system call
   with any value you'd like.
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
   $ xv6_test 
   In spoon system call with argument 0x00000000deadbeef
   $
   ```

### Hints and Warnings

A few things you need to be mindful about when dealing with `xv6`:

1. By design, the address `0x00000000` in `xv6` is **a valid address**. It
   points to the start of the code section of your current program (in virtual
   space of course).

   Therefore, if you are debugging and you see a pointer with a value `0`, that
   does _not necessarily_ mean it is an invalid address (this becomes more
   relevant when dealing with function pointers).

2. Accessing user space data from the kernel space is very dangerous. For
   possible engagement points, think about why that is. In other words, an
   address coming from the user should **never** be dereferenced in the kernel.

   Think about why that is the case and post about it in our class forum for
   engagement points opportunity.

   To get around this problem, please see [this hint page](copyin_out/) for
   further details. You will not have to worry about it in this milestone, but
   you do need to do so later on in this project.

3. Remember from our debugging session, you can turn down parallelism to help
   debug your code. You can do so when compiling `xv6` using `make CPUS=1
   qemu`.

## Task 1: Add your stub system calls

In this milestone, all you have to do is use the above procedure to add stub
system calls for your project. As this is your own design, it is up to you to
design the naming convention and the arguments of your system call.

At the base of things, you will need at least two system calls: (1) to create a
thread and (2) another to join a thread. It will be up to you to add more
should you need them in your design.

Make sure to give an initial through to what those systems calls are going to
need as arguments. Thinking back to `pthread_create` and `pthread_join` is a
good starting point, though you might need to adjust them later on.

In the implementation of each system call, you need to do the following:

1. Parse the arguments and print them.

2. Print the message `This call has not been implemented yet!`.

## Task 2: Design your user API functions

After you have defined your system calls, you will need to provide your users
with a user-level API they can easily use to create threads. Normally, system
calls are complicated and require us to set up some information and variables
before making the call. Such complications are abstracted from the user by
providing them with an easy to user API.

For example, `pthread_create` is not really a system call. It makes use of the
`clone` system call (also wrappers around it). You can check out how
complicated the code for `pthread_create`
[here](https://codebrowser.dev/glibc/glibc/nptl/pthread_create.c.html#create_thread).

For this milestone, you only need to define your API. Again, you don't have to
implement it, just have an idea of how your users will be using your threads.
Remember that this is your design, so you make the rules. Don't want your users
to worry about thread ids? Maybe maintain those for them. Don't want them to
worry about the stack? Maybe allocate it for them. It is up to you to define
what each API function will do and perform.

For this step as well, you'd need to provide your threading library a name. For
example, if you name your library `cool threads`, then create your API
functions in a file under `user/cool_threads.h`.

# Submission and grading

Submit all of your modified `.c` and `.h` files to Gradescope. Please don't
compress the entire directory and submit it, that makes looking at it very
hard. Simply drag and drop the files you have modified onto the submission box.

## Grading

Here's a breakdown of how your project will be graded per milestone:

| Milestone    | Grade |
|--------------|-------|
| Milestone 1  | 15    |
| Milestone 2  | 35    |
| Milestone 3  | 40    |
| Presentation | 10    |

<!--
You can see that the first two milestones are only worth 15 points of your
final project grade. This means three things:

1. The goal of these milestones is to break down your project into reasonable
   steps so that the project would be manageable. At the same time, they are
   not worth much which means that you could still crank a good project by
   staying up for 36 hours before the deadline (highly not recommended).

2. Should you decide to skip these two milestones, your project grade is capped
   at a B+.

3. These milestones are malleable, meaning you don't have to stick with what
   you submit in your design. You can, and probably will, change those along
   the way. They are here to drive you to work and think about potential
   issues.
-->
