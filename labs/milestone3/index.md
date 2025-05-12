---
layout: post
title: Project Milestone 3
readtime: true
date: Mon May 12 13:12:02 EDT 2025
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

# Milestone 3: Joining threads and Sharing Memory

In this third milestone, we would like to update our half-threads
implementation to support joining threads that have been created. Given our
design constraints, joining a thread resembles waiting for a process to a large
degree. The main difference would be the fact that we would like to join for a
specific thread that belongs to the same process from which the join is called.

After wrapping up the join operation, you would start working on sharing the
address space (**do not share pointers to the same page table**) between all of
your threads that belong to the same process. We would need to do that in a way
that is very similar to the CoW lab (including implementing reference
counting). However, you do not need to worry about updates to the page table in
this milestone.

In this milestone, you should perform the following tasks:

- Implement thread joining based off of the implementation of `wait` in xv6.
  Make sure to add the ability to wait for a specific thread.

- Augment your thread creation code to share memory instead of isolating memory
  between the threads.

  Please note that while the threads will have their stacks starting at
  different locations, they _can_ access each other's threads if they have
  pointers within those. In other words, if a thread passes another thread a
  pointer to variable in its own stack, that other thread should be able to
  read and write it. It is the job of the user to make sure that such a
  behavior would not cause any issues.

- Design a test case to verify the sharing of the memory address space between
  threads.

- Augment your thread joining (or waiting) to make sure that memory will only
  be deleted once all threads have terminated.

- Design a test case to verify that threads gracefully terminate without
  causing memory to either leak or be incorrectly accessed.

## Step 1: Implement join

Start by closely reading the source code for the `wait` system call in `xv6`.
Reading and understanding that implementation will be crucial for this task.
Make sure you understand how does a process find its children in the current
implementation of `wait` in `xv6`.

Next, set up the stub for your thread joining system call and adjust it to
accept at least one parameter that identified the thread that one wishes to
join.

Finally, follow closely the implementation of `wait` and adjust it where you
see fit so that it blocks waiting for a specific thread instead of waiting for
any child to finish.

## Step 2: Share the memory

At first, modify your thread creation function to have the newly created
process share all of its user memory pages with its creator. **Please note that
you should not simply assign the new thread the same pointer to its creator's
page table**, that would cause `xv6` to break. Your threads should have
separate page tables, but the mappings in those page tables should always be
synced. This is very similar to what we have done in the CoW lab, except that
we do not want to turn off writing to these pages, nor do we want to make use
of an RSW bit (unless you want to!).

## Step 3: Test your shared memory implementation

Once you have your shared memory implementation in place, write a test case to
verify that the threads are indeed sharing their memory address space. Your
code should show that if one thread modifies a variable, then all other threads
should see the change reflected. **However**, as we are not dealing with
concurrency in this project, feel free to use `sleep` statements and `while`
loops to inject artificial concurrency solutions.

At this stage, if you have not yet adjusted your `joining` code, it is likely
that your threads will crash when they terminate. You can just use infinite
loops (`while(1);`) to make sure that your threads will not exit and thus be
able to verify that you implementation works so far.

## Step 4: Adjust thread joining and graceful exit

At this stage, your threads should function correctly while they are alive, but
will cause issues when they terminate. Adjust your thread exit and thread
joining code so that memory pages are only deleted from the address space once
all of the threads have terminated. We have done something similar to that in
the _copy on write_ lab, feel free to grab your solution from there or
implement a fresh one.

## Step 5: Test your thread create and join

Once thread creation and thread joining are in place, it is time to have a
holistic test. Remove any infinite loops you injected into your previous test
cases and make sure that your threads terminate gracefully and that your memory
is cleaned up once all of your threads have terminated.

<!--
## Step 5: Dynamic memory changes

Finally, you should test that dynamic changes to the memory address space are
propagated to all threads sharing that address space. In other words, if thread
$\mathtt{T_1}$ and thread $\mathtt{T_2}$ share an address space and
$\mathtt{T_1}$ decides to create and map a new page using `sbrk` (or any other
way), then the newly created page should also be mapped and accessible for
$\mathtt{T_2}$. Here's an example way to test this feature (this is just a
pseudocode, you will need to adjust a few things to make it work):

```c
// global variable, start invalid
int *p = 0xdeadbeef;

void *thread_t1(void *arg)
{
  // allocate a new page using sbrk
  p = sbrk(4096);

  // p is the start of this new page
  p[0] = 3;
  p[1] = 2;
}

void *thread_t2(void *arg)
{
  // sleep a bit while t1 allocates p
  sleep(10);

  // check that the address in p have changed
  if(p == 0xdeadbeef) {
    // FAIL
  }

  if(p[0] == 3 && p[2] == 2) {
    // SUCCESS
  } else {
    // FAIL
  }
}

// create threads and join them....
```

**You will also need to adjust the above test code to make sure if more than
one page has been allocated and mapped, then all those pages are updated within
the other threads**.

## Step 6 (Optional): `exit` vs `join`

As you might recall from `pthreads`, there is a difference between a thread
exiting and a thread returning and then being joined. `exit` is a process-wide
system call that kills all threads living within the same address space while
thread return and join are thread-local calls, i.e., when a thread returns, only
that thread will terminate without impacting other threads.

Adjust your implementation to have two separate ways for threads to terminate:

1. Using `exit`, at this stage all of the calling thread's siblings will have
   to also terminate. The process address space will have to be cleaned up. It
   does not matter which thread called `exit`, all of them will have to be
   terminated.

2. Using `return` or your own custom system call, then only the calling thread
   will terminate and your regular operations (from previous steps) are
   executed.

# Project Presentation

On the last day of classes, we will ask you to present your project during
class time. You will have 5 minutes to present your work leaving 2 minutes for
questions and answers. **All members of your group** should participate in the
project presentation.

Essentially, your presentation is your elevator pitch to get our class to adopt
your threads framework as part of the teaching load. Your presentation should
address the following:

1. All of the design decisions that you had to make to get you threads in a
   working condition. For example, how are your thread's stacks managed, who
   allocates those, how are they shared, etc.

2. A high level overview of your user-level API and how it can be used. If
   there are any specific considerations that your users must be aware of,
   please include those in the presentation.

3. How lineage is tracked between your threads and whether there are any
   limitations on the number of the threads to be created.

4. How memory is shared and maintained between the threads, with any
   limitations spelled out.

5. What are the challenges your faced during your design and implementation and
   how you overcame (or planned to overcome) them.

6. In hindsight, what would you have liked to know earlier and what would you
   have done differently?
-->

# Submission

For your submission, please first **clean** your submission directory using
`make clean` before your submit. This will remove all intermediate files and
binaries. Then create a zip file of your directory and upload it to Gradescope.

In addition to your modified files, please submit a short description of the
decision you have made for this milestone. They do not have to be your final
decision, you can still adapt them as you make further progress.

> Please note the changes from milestone 2 in the template below.

**Please do not submit MS Word file**, I will not open any MS product to read
your design document. Please use pdf, markdown, or just plain text for your
submission.

Here is a sample `design.txt` file you can fill out and submit. **It is in your
own best interest to fill this out BEFORE you write a single line of code**,
then implement your code based on your design. It is never a good idea to write
a design description based on code that you have already implemented.

**Please place your `design.txt` as well as your presentation under a `doc/`
directory in your top level source code directory**.

```txt
---
title: Project Milestone 3
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

Additional comments
==================

Add any addition comments, questions, or design decisions that you are
considering for the final submission.

Grade
======

The grade you believe should be assigned to your group, with some argument as
to why you chose that grade.

```

_Please make sure to submit only once per group_.

