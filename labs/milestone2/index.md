---
layout: post
title: Project Milestone 2
readtime: true
date: : Sat Feb  8 23:48:38 EST 2025
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

# Milestone 2: Creating threads with no shared memory

In this second milestone, our goal is to implement the system call that allows
for the creation of a thread. Recall that each process initially starts with
one thread, so you wouldn't have to worry about the main thread. When user,
through your provided API, make a call to your system call, it will create a
new thread of execute. That new thread will need to meet the following
requirements:

1. The thread must start execution from a user-provided function. That function
   is to be passed to the system call at the time of the thread's creation.

2. The thread's function must be able to accept _at least_ one argument. Though
   in RISC-V, it is very easy to allow up to 8 arguments at once.

3. You must define a way for your threads to exit correctly, either implicitely
   by _forcing_ a thread to exit, or explicitely by asking your user to make a
   certain follow up system call.

4. At this point, we want our thread to have __isolated memory address
   spaces__. In other words, we will not share the address space yet, we will
   just make a copy of the address space for each newly created thread, just
   like we would do for a _forked_ process.

5. Since each thread is to execute a different function, it must then create a
   new stack for that function call. You must make sure that when the thread
   starts execution, it's stack pointer `sp` should point to a new location
   that you decide upon (either you create or passed through the user, etc.).
   For this milestone, this new stack will be in the thread's isolated address
   space and thus you don't have to worry about sharing anything.

5. You do not have to worry about ways to `join` a thread yet. However,
   reading the source code for `exit` and `wait` in `xv6` should give a good
   way to start approaching this subject.

6. Lastly, you must design a user test case that illustrates the correct
   execution of your system call.


## Step 1: Read the code for `fork`

To get start, please take a moment to read the source code for the `fork`
system call in `xv6`. It will prove very useful for you as it will help you
understand how `xv6` creates processes, and the API calls needed to set things
up. You will reuse a lot of the structure and API calls of `fork` in your
project, so spending some time to understand it will be of immense help on this
project. 

> _NOTE_: To make things easier on you, you __must__ reuse the `struct proc`
  process control block to represent both threads and processes. The
  implications of that are that you wouldn't have to worry about scheduling, as
  the scheduler wouldn't care much if it is scheduling a thread or a process, as
  long as it is represented by a `struct proc`. Recall that one of our
  requirements is that our threads are schedulable entities. Therefore, using the
  `struct proc` to represent threads as well will win you this requirement for
  free.

> _Hint_: You will need to add some fields to the `struct proc` to determine if
  this is a thread of a process, and if it is a thread, who are its parent(s),
  siblings, etc. Feel free to add any fields to the `struct proc` that you see
  fit. You have complete freedom over that.

## Step 2: Implement the thread creation system call

Once you have a good grasp over the `fork` system call, you can start planning
how to implement your thread creation system call.

Here are a few things you will need to consider:

1. Recall that for this milestone, we want our threads to have separate address
   spaces, so please do not attempt to optimize early and share the address
   space.

   Specifically, what this means is that we are still copying the address space
   for the newly created thread and create a new, separate, page table for it.
   Therefore, you should not mess with the code that copies page tables for
   this milestone.

2. Each thread needs to have its own stack. What this means is that each
   thread's stack should start **at a different virtual location**.
   Here are a few hints to consider:

   - Which register from the thread's `trapframe` is used to determine where
     its stack starts?

   - _Design decision_: You will need to decide who creates the stack for the
     thread, is it the user that creates it (meaning the user must allocate room
     for each thread's stack)? Or is the kernel's job to create new stacks for
     each process (i.e., the kernel will allocate memory frames for each thread
     and map them in the page tables).

     The answer to this question is for you to consider. You should be able to
     argue for your answer to justify your choice.

3. Newly created threads need to start execution from any point in the code.
   Specifically, the user must specify where each created thread should start
   execution from (think back to how we used `pthread_create`).

   - You should definitely ask your user to provide you a function pointer to
     the starting point for the thread being created, you don't have to figure
     that out yourself.

     Be careful though, recall that address `0x00` is a valid address in `xv6`,
     so don't worry if you see that the function the user passed to you is
     located at address `0x00`, that is normal in `xv6`.

   - Which register in the thread's trapframe will influence where the thread
     will start executing after it is created? Once you figure that out, it
     should be easy to implement this step.

4. When designing your solution, make sure to think of a way to track lineage
   relationships between threads of the same process. In other words, each
   thread must have a way to reach other threads that are in the same process,
   and thus share the same address space.

## Step 3: Test your thread creation code

After you have designed and implemented your thread creation system call,
please write a test case to make sure that it is behaving as expected. You will
need to consider the following things:

1. Test that threads are actually being created, i.e., the system call is
   effective and successful.

2. Test that threads start from any code location that the user specifies when
   they make the system call.

3. If you are supporting arguments, test that threads can support function
   arguments.

4. Test that the threads execute concurrently, meaning we do not block a thread
   because of the existence of another one. If both can run, they will run.

## Thread joining

You do not have to worry about joining threads after they have been created in
this milestone. **It is perfectly okay if your code crashes or xv6 crashes**
after one or all of your threads die out. It is also okay if you leak memory at
this stage, that is okay for now. We will look into thread joining in the last
milestone.

# Submission

Submit all of your modified `.c` and `.h` files to Gradescope. Please don't
compress the entire directory and submit it, that makes looking at it very
hard. Simply drag and drop your modified files onto the submission box.

In addition to your modified files, please submit a short description of the
decision you have made for this milestone. They do not have to be your final
decision, you can still adapt them as you make further progress.

**Please do not submit MS Word file**, I will not open any MS product to read
your design document. Please use pdf, markdown, or just plain text for your
submission.

Here is a sample `design.txt` file you can fill out and submit. **It is in your
own best interest to fill this out BEFORE you write a single line of code**,
then implement your code based on your design. It is never a good idea to write
a design description based on code that you have already implemented.

```txt
---
title: Project Milestone 2
Author(s): Your name and teammates names here
Date: Submission date
---

High level description
======================

Add a high level description of your project here. Make sure to summarize all
the features that you have designed.

Thread creation
================

Describe the decisions you have made for thread create, including answers to
the questions above.

Stack separation
================

Describe how your threads gets assigned different stacks (even though they are
still in separate address spaces for now).

Test cases
===========

- Describe the test case you used for the creation of the threads.

Addition comments
==================

Add any addition comments, questions, or design decisions that you are
considering for the final submission.

```

_Please make sure to submit only once per group_.

