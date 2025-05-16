---
layout: post
title: Project Milestone 4
readtime: true
date: Fri May 16 09:02:15 EDT 2025
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

# Milestone 4: Page table updates

After the third milestone, your threads should now be living in the same
address space, sharing all of their memory pages, all the while maintaining
separate page tables (though page tables should contain the same information). 
The last bit of implementation we need to add is concerned with synchronizing
the page tables once any updates are made.

For example, if thread $T_1$ maps a new page in its page table, then all of the
threads that live in the same address space **must** see that update in their
page tables. Similar, if $T_1$ unmaps a page from its page table, then all of
the threads that live in the same address space **must** see that page removed.

In this milestone, you should perform the following tasks:

- Implement page table propagation in your threading library. There is not one
  way of achieving this, so you should design your approach in the best way
  that fits your current implementation.

- Design a test case to verify the page table updates are propagated to all
  other threads in the same address space.

## Step 1: Dynamic memory changes

In the last step of this project, you should test that dynamic changes to the
memory address space are propagated to all threads sharing that address space.
In other words, if thread $\mathtt{T_1}$ and thread $\mathtt{T_2}$ share an
address space and $\mathtt{T_1}$ decides to create and map a new page using
`sbrk` (or any other way), then the newly created page should also be mapped
and accessible for $\mathtt{T_2}$. Here's an example way to test this feature
(this is just a pseudocode, you will need to adjust a few things to make it
work):

```c
// global variable, start invalid
int *p = 0xdeadbeef;

void *thread_t1(void *arg)
{
  // allocate a new page using sbrk
  p = sbrk(4096);


  // you can also use sbrk(-4069) to remove that page and unmap it from the
  // page table

  // p is the start of this new page
  p[0] = 3;
  p[1] = 2;
}

void *thread_t2(void *arg)
{
  // sleep a bit while t1 allocates p
  sleep(50);

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

### Some hints

All updates to the page table from user space are handled by the `sbrk` system
call. Specifically, `sbrk` calls the `growproc` routine which will either add
new pages to a process's address space, or remove pages from its address space,
depending on its arguments. You should first read and understand the code in
`growproc` before attempting to implement your design. You can find the
`growproc` function in the `kernel/proc.c`.

`growproc` calls two functions that you will find useful, `uvmalloc` and
`uvmdealloc`. You can find both of these functions in `kernel/vm.c`.

**Note** that `xv6` assumes that memory grows or shrinks linearly. In other
words, a process either adds pages on top of its current address space,
increasing its size to `sz + PGSIZE`, or it remove pages from the top of its
address space, decreasing its size to `sz - PGSIZE`.

## Step 2 (Optional): `exit` vs `join`

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
title: Project Milestone 4
Author(s): Your name and teammates names here
Date: Submission date
---

High level description
======================

Add a high level description of your project here. Make sure to summarize all
the features that you have designed.

(M2) Thread creation
====================

Describe the decisions you have made for thread create, including answers to
the questions above.

(M3) Memory sharing
===================

Describe how you have implemented the sharing of the address space in this
milestone, including answers to any questions in the description above.

(M4) Memory udates
===================

Describe the approach you have taken to propagate updates to the page table
from one thread to all of the other ones. Make sure to discuss how threads can
find out about other threads in the same address space, and then how to page
table updates are mapped out.

Test cases
===========

- (M2) Describe the test case you used for the creation of the threads.

- (M3) Describe the test case you used for testing shared memory between threads.

- (M4) Describe the test case(s) you used for testing the propagation of the
  updates on the page table.

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

