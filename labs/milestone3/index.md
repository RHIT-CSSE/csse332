---
layout: post
title: Project Milestone 2
readtime: true
date: Mon Aug 12 2024
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

# Milestone 2: Creating threads that share memory

In this second milestone, our goal is to implement the system call that allows
for the creation of a thread. Recall that each process initially starts with
one thread, so you wouldn't have to worry about the main thread. What we are
after in this milestone is to create an additional thread that will share the
same address space as the thread that created it.

- Read and understand the source code for the `fork` system call in xv6.

- Implement a system call that creates a thread living within the same address
  space as the process that creates it.

- Design a test case to test the creation and running of the thread.

- Design a test case to test the sharing of the memory address space between
  threads.

## Step 1: Read the code for `fork`

To get start, please take a moment to read the source code for the `fork`
system call in `xv6`. It will prove very useful for you as it will help you
understand how `xv6` creates processes, and the API calls needed to set things
up. You will reuse a lot of the structure and API calls of `fork` in your
project, so spending some time to understand it will be of immense help on
this project. 

> _Hint_: To make things easier on you, I do strongly recommend that you reuse
  the `struct proc` process control block to represent both threads and
  processes. The implications of that are that you wouldn't have to worry about
  scheduling, as the scheduler wouldn't care much if it is scheduling a thread
  or a process, as long as it is represented by a `struct proc`.
  Recall that one of our requirements is that our threads are schedulable
  entities. Therefore, using the `struct proc` to represent threads as well
  will win you this requirement for free.

> _Hint_: You will need to add some fields to the `struct proc` to determine if
  this is a thread of a process, and if it is a thread, who are its parent(s),
  siblings, etc. Feel free to add any fields to the `struct proc` that you see
  fit. You have complete freedom over that.

## Step 2: Implement the thread creation system call

Once you have a good grasp over the `fork` system call, you can start planning
how to implement your thread creation system call.

Here are a few things you will need to consider:

1. Threads need to share the same address space. What does that mean when it
   comes to their page tables? What can you say about the mappings in each
   thread's page table?

   _Hint_: **Do not simply share the page table** between threads, that will
   not work as each thread will need separate mappings for a special page to
   perform context switching. _Think back to the copy-on-write lab_ and adopt a
   similar strategy to what we did there.

2. Each thread needs to have its own stack. However, be careful that when we
   say that stacks are separate, we do not mean that they are in different
   address spaces. All we mean is that the threads' stack start **at different
   locations in the same address space**. Here are a few hints to consider:

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

4. For this milestone, you do not need to worry about what happens if a thread
   creates a new page and maps it in its page table. We will get to that in the
   last milestone.

## Step 3: Test your thread creation code

After you have designed and implemented your thread creation system call,
please write a test case to make sure that it is behaving as expected. You will
need to consider the following things:

1. Test that threads are actually being created, i.e., the system call is
   effective and successful.

2. Test that threads start from any code location that the user specifies when
   they make the system call.

3. If you are supporting arguments, test that threads can support function
   arguments. If not, you can leave this one until milestone 3.

4. Test that the threads execute concurrently, meaning we do not block a thread
   because of the existence of another one. If both can run, they will run.

## Step 4: Test your shared address space

Finally, you will need a way to test that your threads actually share the same
address space. The easiest way to test that is by creating global variables,
and then making sure if one thread changes the value of a global variable, then
all other threads will see that change.

Of course, the above test case has race conditions, **you do not have to worry
about those in this project**. Feel free to use `sleep` statements in each
thread to create some form of synchronization.

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

Memory sharing
===============

Describe how you have implemented the sharing of the address space in this
milestone, including answers to any questions in the description above.

Test cases
===========

- Describe the test case you used for the creation of the threads.

- Describe the test case you used for testing shared memory between threads.

Addition comments
==================

Add any addition comments, questions, or design decisions that you are
considering for the final submission.

```

Finally, recall that the goal of these milestone is not completion, but
progress. So don't panic if you have not completed everything yet, as long as
you are making progress, your time closer to the end of the term will be much
more enjoyable.

_Please make sure to submit only once per group_.

