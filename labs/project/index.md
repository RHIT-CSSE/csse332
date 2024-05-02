---
layout: post
title: Project Description
readtime: true
date: Sun Jan 28 22:30:29 2024
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
3. Each thread is a separate schedulable entity. In other words, even if there
   are two processors available, then both threads can be actively running at
   the same time.

To support creating and manipulating threads, we would also like to
provide an API for our users to create and join threads, in a way
similar to what the `pthreads` library provides. You do not need to
worry about synchronization primitives like condition variables.


## The implementation

In this section, you must present a plan to implement your design. We ask you
to adopt an incremental build and test process. In other words, do not attempt
to implement everything all at once. Start with smaller pieces, test those out,
make sure they are working, and then start adding more features.

Be specific about the data structures that you will be using and the functions
that you will be adding/editing to achieve your goals. The more specific you
can be here, the more feedback you can get from your instructor prior to
starting your implementation.

# Required features

Here's what we require:

1. Support for an API to create a thread that can start at an given
   function pointer and has its own stack.

2. Support for joining threads.

3. Support for an API to create a thread that can take one argument.

5. The created threads share the address space when reading &
   writing. In other words, the same address in all threads will read
   the same value. If one thread modifies a memory location, all other
   threads will see the impacts of that change. No need to worry about
   synchronization here.

7. If one thread allocated and maps a new page, all other threads must be able
   to access that page.

   This one is challenging, you be judged by the creativity of your solution as
   well as its efficiency and impact on performance.

10. If a thread finishes, its state is cleared up without causing any memory leaks
    and/or memory corruption.

11. If the main process dies, then all threads associated with it will die,
    i.e., we do not do any reparenting.

Each of these should be documented by a small test case or sample
program.  Please have a separate one that highlights each feature.

You do not need to handle the intersection of threads and complicated
process stuff (e.g. what happens if you fork a process with threads,
what happens if you exec a process with threads).

You do not need to handle concurrency primitives.

# Deliverables

At the end of the project, please submit the following:

1. Your modified `xv6` source code.

2. A short design doc that explains how your threads memory works, the
   implementation of trapframes in your system etc.  Highlight
   differences from the milestone 1 doc.

3. A brief (5 minutes) video presentation of the features of your design along
   with a demo of your threads in action. Think of this as an elevator pitch
   for your project, highlighting where it stands strong and showcasing those
   in practice using the demo.
   
4. Instructions on how to run and see your test cases per feature.

# Milestone 1: The very basics

For milestone 1 we'd like to submit 2 things: a design doc describing
how your threads memory will work and a demo that showcases two
threads of code running in parallel.

### Threads in memory

To get started, you **must** read chapters 3 and 4 of the [xv6
book](https://pdos.csail.mit.edu/6.828/2023/xv6/book-riscv-rev3.pdf); those
chapters are essential for you to understand how to design and implement
lightweight threads in the xv6 kernel. Please do read those chapters carefully
before answering the below questions.

In particular, there are three special mappings, the kernel stack, the
trapframe and the trampoline.  You must understand all of them to
build your thread system correctly.

Of particular interest is the trapframe.  You must determine a
mechanism that allows multiple threads within the same program to trap
at the same time.  This in particular should be documented in your
design doc.

Be as specific as possible in this doc.  Include the particular C
structs you'll use, showing which parts are thread specific and which
parts are process specific.
   
### A demo

This demo can be as hacky as you need it to be but it should run and
evidence that two threads within the same process are running in
parallel.  It should not use process level paralellism like fork.
