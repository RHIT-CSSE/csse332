---
layout: post
title: Term Project (Milestone 3 - Checkpoint II)
readtime: true
date: Fri May  7 11:49:55 2021
gh-repo: rhit-csse332/csse332-202130
gh-badge: [star,watch,follow]
---

# Table of contents

<!-- vim-markdown-toc GFM -->

* [Introduction and aim](#introduction-and-aim)
  * [Learning objectives](#learning-objectives)
* [Multi-Level Feedback Queue](#multi-level-feedback-queue)
* [Requirements](#requirements)
* [Submission](#submission)
* [Junior: Rules 1 and 2](#junior-rules-1-and-2)
  * [Data structures](#data-structures)
  * [Timers](#timers)
  * [Registration](#registration)
* [Senior: Rules 3 and 5](#senior-rules-3-and-5)
* [Rubric: Rule 4](#rubric-rule-4)

<!-- vim-markdown-toc -->

# Introduction and aim

In the second checkpoint of milestone 3, we will finally put things together and
design our Multi-Level-Feedback-Queue (MLFQ) scheduling algorithm. We will start
from our FIFO round robin scheduler that we design in checkpoint I and use that
to lead us forward into our design and implementation. 

## Learning objectives

In this checkpoint, we will
- __solidify__ our understanding of CPU scheduling algorithms
- __design__ and __implement__ the MLFQ scheduling algorithm
- __complete__ our module with scheduling parameters
- __test__ our scheduler using custom-designed test cases
- __write__ a documentation and user guide report for our module and test cases.


# Multi-Level Feedback Queue

Recall [our
discussion](https://rhit-csse332.github.io/csse332-202130/sessions/14_session14/)
of the MLFQ scheduling algorithm from Session 14. In MLFQ, we will try to
optimize for both the _turnaround_ as well as the _response time_. The MLFQ
scheduling algorithm is composed for five rules:
1. If `Priority(A) > Priority(B)`, then A runs and B doesn't.
2. If `Priority(A) == Priority(B)`, then A and B run in round-robin fashion
   using the quantum length __of the given queue__.
3. When a process enters the system, it is placed at the highest priority level
   possible.
4. Once a job uses up its time allotment at a given level (regardless of how
   many times it has given up the CPU), its priority level is reduced (i.e., it
   moves down one queue).
5. After some time period `S`, move all the jobs in the system to the topmost
   queue.

An MLFQ scheduler is interesting for the following reason: It does not require
any prior knowledge about the nature of the processes that it is scheduling. It
will observe the behavior of these processes and then will adjust and adapt
accordingly. Therefore it can deliver on the performance of algorithms such as
Shortest-Job-First or Shortest-Time-to-Completion-First, as well as make similar
fairness guarantees as a Round Robin scheduler.

# Requirements

Here is a list of requirements that your implementation must satisfy:
- Your system must have __at least__ 3 queues.
- Each queue must have a different round robin quantum.
- Your scheduler must implement the above five rules correctly.
- Your system must allow for processes to register and deregister at any time,
  i.e., it must support processes accessing your data structures concurrently.

__WARNING: This a design-based checkpoint, i.e., I will give you the general
idea and some hints as to how to approach it, but it is up to you to make and
justify your design choices__.

# Submission

Please submit the following to the moodle dropbox by the 5:00 on the last day of
finals:
- Your project source code (so far, we've been using `project.c`)
- Your makefile
- A project report (in PDF format) that contains the following:
  - An overview of your design (one paragraph)
  - A list of your design decisions, along with justification for each design
    decision (one paragraph per design decision)
  - A list of the challenges you faced, along with a description of how you
    overcame (or did not) the challenge.
  - A description of your testing environment along with screenshots showing
    your test cases. I suggested your record a video of your test cases to
    explain them to me, but that is optional, screenshots will do.

If you were not able to complete the project, then please include a detailed
description of why you think that happened, what you think you could have done
differently, and list of the parts of the project that you think are working.
This will be crucial for me to give you partial credit.

---

Here is a suggested plan of implementation, however, feel free to follow
whichever plan that works best for you.

# Junior: Rules 1 and 2

To start putting things together, let's put our focus on Rules 1 and 2 first. We
will first expand our FIFO scheduler to support multiple queues instead of just 1.
Moreover, to test things out, we will add processes to the queues one at a time
(i.e., first process to register gets queue 1, second process gets queue 2,
etc.).

## Data structures

To achieve our goal, let's first start by modifying our data structures. Instead
of a single linked list, we will now a linked list (i.e., a queue) for each of
our run queues (recall, you must have at least three). In my implementation, I
found it easier to declare a structure for each queue, something along the lines
of:
```c
#define MAX_QUEUES 3

struct run_queue {
  /* The priority of this queue */
  int priority;
  /* The round robin quantum for this queue */
  int quantum;

  /* The head of the queue */
  struct list_head queue_head;
};

struct run_queue priority_queues[MAX_QUEUES];
```

Remember to initialize your list heads using `INIT_LIST_HEAD` and set the
default values in the `__init` function, otherwise, nasty things will happen.

__Note:__ You can still reuse your database as one of the priority queues,
however, I would advise against it. Feel free to keep the database or just get
rid of it all together.

## Timers

A big difference between this checkpoint and the first one is the fact that now
we cannot rely on a single timer that always goes off periodically. Instead,
depending on which queue we are using, the timer will need to go off at
different intervals (recall, each queue must have its own quantum).

For example, if the quantum for queue 1 is 1 second, the quantum for queue 2 is
2 seconds, and the quantum for queue 3 is 3 seconds, then our timer will behave
differently depending the queue from which we are scheduling. If we are
scheduling from queue 1, then we need our timer to go off after 1 second. If we
are scheduling from queue 2, then we need our timer to go off after 2 seconds,
and so on.

In my implementation, instead of keeping a single global timer, I found it
easier to create a timer for each process that is being scheduled. Once I know
the process I need to schedule and the queue from which it was scheduler, I can
use `mod_timer` to appropriately set when the next timer will go on.

## Registration

To implement and test rules 1 and 2, we will assume that the processes that get
registered get added to the queue in a FCFS basis. In other words, the first
process to register gets the queue with the highest priority, the second process
to register gets the queue with the second highest priority, and so on.

Next, I did the following. If there is no currently running process (i.e., this
is absolutely the first process to register -- the system was empty before
that), then I keep the process running and I schedule its timer to go off at the
corresponding quantum. If there is a currently running process, then I put the
registering process to sleep. That was my way of implementing my system, it is
up to you to design your approach to handling registration. Make sure to
document your decisions in your project report.

# Senior: Rules 3 and 5

# Rubric: Rule 4

