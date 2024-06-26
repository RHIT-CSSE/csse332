---
layout: post
title: Challenge Project Step 1
readtime: true
date: Mon Feb  6 2023 
---

# Introduction

The goal of this project is for you to choose your favorite feature of an
Operating System and then add that feature (or some simplified version of it) to
the xv6 kernel. The idea behind this exercise is for you to design, tune, and
implement a simple OS features, based on your own research and understanding of
the material we covered in class.

As part of your implementation plan, you might need to make multiple assumptions
to simplify certain aspects of the problem that cannot be handled by your design
and implementation. An important part of this exercise is to train in
__documenting your assumptions__ so that a user of your feature can know exactly
what to expect when deploying. This is a really important aspect of OS design as
your code will be running at a high privilege level and it must thus be very
clear what your code can and cannot do.

Another important part of this exercise is designing and writing test cases. Say
for example you decide to write a new xv6 process scheduler, then you will need
to write a few test cases that actually showcase or stress the features of your
scheduler. Running all processes to completion is great, but if your scheduler
has support for some form of priorities, then you will need to write, and
document, test cases that exercise those priorities.

# Learning Objectives

At this end of this project, you should be able to:

- Design a basic OS feature and implement it in xv6.
- Document your assumptions about your OS feature.
- Write test cases that showcase and stress your implementation.
- Write a report that highlights the advantages and limitations of your
  approach.

# Project Requirements

There are several requirements that your project must meet at the end of the
term.

## Teams

You should complete this project in teams of 2 or 3 students, you are free
to pick your group-mates based on your preferences and connections. I strongly
discourage lone-wolf projects; sharing ideas, pair programming, and task
planning can be very helpful for you to have a smooth experience.

It is __okay__ to form teams across the class sections as long as all team
members can show to the project meetings at the same time. If a student fails to
show up to the meeting without a valid excuse, then they will automatically fail
the project. 

## Project Meetings

During class time (as determined by the schedule page), I will meet with every
group to discuss their ideas and potential plans. Those meetings will be crucial
in making sure that your ideas are at an appropriate scale. __All team members
should be present at those meetings__.

## Completion Requirements

At the end of the term, your project will be assessed based on the
following requirements:

1. The assumptions that you make throughout your design, and how well they are
   documented.

2. The test cases that you write to stress your design and showcase where it
   excels/fails.

3. A small design document that walks me through your design features, how you
   came up with them, any challenges you faced, and how (or why not) where you
   able to overcome them.

4. Your implementation in the xv6 kernel.

5. The performance of your implementation (runtime, memory footprint, etc.).

Particularly, I am very interested in seeing how you would design your feature
and implement it, given the many limitations of the xv6 kernel. That is not to
say that your project should not be implemented, but more to say that the
process that lead you to make the choices you made (for example which data
structures to use, concurrency issues, trade-offs you had to make, etc.) are
very important.

### Documentation

As per the completion requirements, documenting your approach is of the utmost
importance. Think of it as the means by which you can demonstrate how well you
understand OS concepts by designing and implementing one of them, and
basically what you can use to argue for the grade that you deserve on the
project. When reading your document, I should be able to understand the design
options that have been considered, the design decisions that have been made, and
the rationale for those decisions

At the same time, a tutorial on how to run and use the OS features that you have
implemented would be crucial for us to determine how usable your added
feature(s) is.

Finally, in your design document, you will get to assign yourself (or your group
for that purpose) that grade that you think you deserve for the project, and
make the argument as to why you think that would be an appropriate grade.

# Ideas

In what follows I will list a couple of ideas that I think would be doable
within the time that we have allocated for this project. However, if you have
ideas of your own that you'd like to explore, then I definitely encourage you to
go after them. I am more than happy to meet with you and discuss your ideas and
make sure to scale them down to be done within the time allowed for this
project.

Note that for all these ideas (and your own), you will find that the [xv6
book]({{ site.xv6_book_url }}) to be very useful and very informative. The
source code is also pretty readable and very easy to understand.

<!--
## 1. Memory Management: Buddy Allocator

In its current form, the xv6 kernel allocates physical memory in a
straightforward way, and only one page of memory at a time. For example, take a
look at `kalloc` and `kfree` in `kernel/kalloc.c`, you can see that the
implementation is fairly simple. The kernel maintains a freelist
`kmem.freelist` as a singly linked-list. Allocation simply picks a free page
from the list, while freeing returns an allocated page to the list. This is very
simply and efficient, but clearly not very useful.

In this task, your job is to modify the kernel's physical page allocator to use
a buddy memory allocator scheme. The buddy allocator was originally devised by
Kenneth Knowlton in the paper [A Fast Storage
Allocator](https://dl.acm.org/doi/pdf/10.1145/365628.365655) and has been ported
into the Linux kernel, and has shown several performance improvement over other
schemes of memory allocation.

You can find a great discussion of how the buddy allocator is implemented in the
[Linux
kernel](https://www.kernel.org/doc/gorman/html/understand/understand009.html)
with plenty of nice diagrams that can help you understand how it can be built.
You can make any simplifying assumptions that will help you spin up a working
implementation, but make sure to document all of your assumptions in your design
document.
-->

## 1. Process Management: Kernel Threads

In its current form, the xv6 kernel only supports single-threaded processes with
separate address spaces. That is great for a start, but it has to incur a pricey
context switching overhead every-time we switch from one process to another.
Therefore, we would like to provide support for kernel threads that can share a
process's address space (something like `pthreads`) thus speeding up context
switching and providing parallelism.

In this task, you should provide support for kernel threads in the xv6 kernel.
Essentially, we need to provide support for a new system call called `clone`
that creates a child thread sharing the same address space as that of its
parent, except that it has its own stack. You would also need to create a system
call, most likely called `join`, that will wait for a child thread to exit.

I suggest that you start this task by reading the [man page of the `clone`
system call](https://www.man7.org/linux/man-pages/man2/clone.2.html) and then
writing a few examples that uses `clone` and run them on your Linux machine.
You'd then want to select a small subset of the options that are provided by
`clone` and implement them in xv6. Then you can move on to implementing
`join`. Your implementation must provide support for a user-level API that
allows users to create and manipulate kernel threads (something similar to the
`pthreads` library).

## 2. Process Management: Userspace Threads

The goal of this exercise is to build a user-space threading library for xv6.
What makes this project unique is that it doesn't involve any coding in kernel
space, i.e., all of your files will go into the `user/` directory. However, it
will require you to write some RISC-V assembly.

From a kernel's point of view, nothing changes. The kernel still only sees one
process and is not aware of the user-space threads that you should be creating.
You can definitely use the code from the user-space threads lab as a starting
point, however, what we are looking to implement here are the `makecontext` and
`swapcontext` functions that we used in that lab.

At an initial stage, you do not have to worry about any concurrency issues.
However, your implementation __must__ provide support for at least one means for
concurrency along with its corresponding implementatin and usable API.

## 3. Process Management: MLFQ Scheduling

As we have seen in class, the xv6 scheduler is a fairly straightforward round
robin preemptive scheduler with a fixed quantum. That is great if what we care
about achieving is fairness and good response time. However, the scheduler does
not take into consideration the behavior of the processes and their patterns of
execution. 

Your job is to implement the Multi-Level Feedback Queue scheduling policy that
we discussed in class in the xv6 kernel. This will require substantial changes
to the scheduling algorithm implementation; we can no longer loop entirely over
the full list of processes, we will need different data structures.

One challenging aspect of this task is the design of test cases. If all of your
processes behave in the same way, then pretty much MLFQ will devolve into round
robin, and we are back to square one. So in order to showcase your
implementation, you will have to carefully design your tests cases to trigger
the different aspect of the MLFQ algorithm. 

## 4. Process Management: Signals

In its current form, the xv6 kernel does not support for process signaling. In
this task, we would like to change that. Pick some set of signals that you would
like to provide for and then implement support for those signals in the kernel.
Of course, you should define default behavior for those signals once they are
received by a process.

Naturally, after that, you should allow for user to write their own signal
handlers. This can be a bit tricky and would require you to carefully design,
and possibly put restrictions on, what can be considered as a signal handler.
For example, it might be a good idea to force all signal handlers to have a
certain pattern or to call certain functions once they enter or when they exit.

## 5. Process Management: Lottery Scheduler

In class, we focused on deterministic scheduling policies. However, another
class of CPU scheduling policies are _probabilistic_ policies, where the process
that gets to run next is selected based on a probabilistic approach to provide
certain guarantess. Here's a really good description of _lottery scheduling_
from the
[paper](https://www.usenix.org/legacy/publications/library/proceedings/osdi/full_papers/waldspurger.pdf)
that introduced it to the OS community:

> Lottery scheduling provides efficient, responsive control over the relative
  execution rates of computations. Such control is beyond the capabilities of
  conven- tional schedulers, and is desirable in systems that service requests
  of varying importance, such as databases, media-based applications, and
  networks. Lottery scheduling also supports modular resource management by
  enabling concurrent modules to insulate their resource allocation policies
  from one another. A currency abstraction is introduced to flexibly name,
  share, and protect resource rights. We also show that lottery scheduling can
  be generalized to manage many diverse resources, such as I/O bandwidth, mem-
  ory, and access to locks.

Your job in this task is to implement a lottery-based scheduling policy in xv6
(or any probabilistic scheduling policy). Similar to the case of the MLFQ
scheduling project, a challenging aspect of this project is to design test cases
that highlight the features of your implementation and how it differs from
other policies. You might find it helpeful to look at the original
[paper](https://www.usenix.org/legacy/publications/library/proceedings/osdi/full_papers/waldspurger.pdf)
or
this [book chapter](https://pages.cs.wisc.edu/~remzi/OSTEP/cpu-sched-lottery.pdf) for some evaluation metrics.

<!--
## 6. Defensive Security: Users and Authentication

Currently, xv6 has no notion of users and thus has no mean for authentication.
Once you launch a shell, you are pretty much in change of everything and can
mess around with anything in the machine. That is not idea.

In this task, we would like to add support for users and authentication in the
xv6 kernel. We will start off real simple, we are not seeking isolation between
different users, we just need the concept of a user and a way to create and
authenticate users. In other words, when xv6 starts, we would like to have a
prompt that asks the user for their username and password, and then we only give
them access if those match our database. At this point, we don't really care
about what happens after that (i.e., who accesses what and when), xv6 goes back
after that to its old ways.

However, one thing that we would like to care about is how we store our database
of passwords. Storing it in plain text is definitely a bad idea since anyone can
steal them, so you'd have to think of a way to store passwords and authenticate
users. I suggest you take a look at password hashing, the Unix `passwd` file,
and the Unix `shadow` file. Note that you might need to implement some
cryptography functions manually since we don't have access to outside crypto
libraries.

Of course, doing authentication with proper access control is not very useful.
So eventually, we would like to support some form of access control over files
in the kernel. Given our timeline, that is definitely out of scope, yet it would
be great if you could draw a plan about how you would approach such a design.
-->

## 6. Defensive Security: Kernel Auditing

As we mentioned in class, auditing is an important part of the OS's roles in
providing security guarantees. When something bad happens, we would like to be
able to trace it back to its origins and to also be able to evaluate what that
bad event impacted in our system (think of it as backward analysis and forward
analysis over data).

In this task, we would like to add to the xv6 kernel the ability to keep a log
of important security (or non-security) events that happen in our system. For
example, anytime a process touches a file, we would like our kernel to keep
track of that event in a log stating something like `Process PID opened file
file with permissions PERM at TS` where `<PID>` is the process id, `<file>`
is the file descriptor of the file that we opened, `<PERM>` is the permissions
that the process opened the file with, and `<TS>` is a timestamp of when that
even happens. You will need to think of a way to intercept such events and log
them into an appropriate location (first in-memory, then maybe on disk?).

Similarly, we would also be interested in tracking the system calls that were
executed by any process in our system. You might find it useful to checkout the
[Linux audit framework](https://wiki.archlinux.org/title/Audit_framework) and
[`auditctl`](https://www.man7.org/linux/man-pages/man8/auditctl.8.html).

You might want to start by selecting a fixed subset of events that you would
like to audit in your system, and then add more events once you have the
infrastructure in place. A challenging aspect of this project is to manage
how logs are buffered and when they would be flushed to disk to be saved.
Writing to disk on every monitored even would be very slow and would become a
bottleneck. So it would be your job to determine how to avoid constantly dumping
logs to disk. This also introduces concurrency issues that you must struggle
with.


## 7. Offensive Security: Return-Oriented-Programming (ROP)

Return-Oriented-Programming (ROP) is a dangerous form of attack that chains
several small pieces of benign code (called gadgets) to allow for arbitrary code
execution. There are plenty of ROP examples that work on different popular
architectures like x86-64, i386, ARM, etc. However, there are very few examples
of running those attacks on an RISC-V system.

Since xv6 is built to run on a RISC-V architecture, we would like to develop a
vulnerable user-space program and a corresponding exploit that can allow a
malicious user to run arbitrary code on the xv6 machine by chaining several
RISC-V gadgets. RISC-V has of course different calling conventions than other
architectures, so the patterns and the makeup of our gadgets will change. You
will need to spend some time reading the RISC-V specifications document and
taking some examples and working through them (I can help you set up debugging
for xv6).

Some references you might find useful are the following:

- [ROPGadget](https://github.com/JonathanSalwan/ROPgadget): It is a great tool
  that finds ROP gadgets in a given binary, yet has no support for RISC-V as of
  yet.

- [Return-Oriented Programming without
  Returns](https://hovav.net/ucsd/dist/noret-ccs.pdf).

- [When Good Instructions Go Bad: Generalizing Return-Oriented Programming to
  RISC](https://cseweb.ucsd.edu/~savage/papers/CCS08GoodInstructions.pdf).

- [No RISC No Reward: Return-Oriented Programming on
  RISC-V](https://arxiv.org/pdf/2007.14995.pdf)

## 8. Other Ideas?

If there is anything specific that you are interested or excited about that you
would want to see come to alive, I would be more than happy to sit down and talk
to you about it. We would devise a plan that simplifies the problem as much as
possible to make it doable within the time available.

# Submission

After our in-class discussions, please submit a proposal __IN PDF FORMAT__
listing your group members, your idea that you would like to implement, and a
tentative plan at implementing it. Your proposal __must contain__ a plan for
exactly how you plan to implement your idea, including needed data structures,
places in the xv6 code-base where your code should be added, and so on. The more
details you add the more we can help you identify what is feasible in the
current timeline and what is not.

