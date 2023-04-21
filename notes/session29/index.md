---
layout: post
title: Scheduling I
readtime: true
date: Sun Jan 29 2023
---

# Learning Objectives

At the end of this lecture, you should be able to:

- Define a scheduling policy and the requirements it must achieve.
- Identify different scheduling policies, their advantages and their
  shortcomings.

---

# Topics

In this lecture, we will cover the following topics:

- CPU scheduling and scheduling policies.
- First-In-First-Out, Shortest Job First, Shortest Time to Completion First, and
  Round Robin scheduling.

---

# Notes

<!-- vim-markdown-toc GFM -->

* [Motivating Analogy](#motivating-analogy)
* [Definitions](#definitions)
  * [Scheduling Policies](#scheduling-policies)
  * [Assumptions](#assumptions)
* [FIFO Scheduling](#fifo-scheduling)
  * [The Convoy Effect in FIFO](#the-convoy-effect-in-fifo)
* [Shortest Job First (SJF)](#shortest-job-first-sjf)
  * [The Convoy Effect in SJF](#the-convoy-effect-in-sjf)
* [Shortest Time to Completion First (STCF)](#shortest-time-to-completion-first-stcf)
  * [Fairness](#fairness)
* [Round Robin (RR)](#round-robin-rr)
* [Activity: Improving the xv6 scheduler](#activity-improving-the-xv6-scheduler)
  * [Getting the Source Code](#getting-the-source-code)
* [Understanding the xv6 Scheduler](#understanding-the-xv6-scheduler)
  * [Advantages of the xv6 Scheduler](#advantages-of-the-xv6-scheduler)
  * [Drawbacks of the xv6 Scheduler](#drawbacks-of-the-xv6-scheduler)
  * [Improving efficiency](#improving-efficiency)

<!-- vim-markdown-toc -->

# Motivating Analogy

- Imagine you are running a kiosk for answering questions about the OS class
- How would you schedule the students that come in to ask questions?
- What if you had a very annoying student, call him Robert, that hogs your time,
  how would you handle that?
  - Hire a bouncer, sort students by their number of questions
- Well what if Robert arrives very early, before everyone else shows up? 
- Even worse, imagine if your crush arrives to ask question and Robert is still
  asking questions, what would you do?
  - Kick Robert out! —> preemption!
- We also might like to give priority to some students over others
  - Put them in a priority queue
- Finally, if you are a social justice warrior, then you want to give everyone
  an equal shot
  - Round Robin
- This is exactly the same questions that OS developers had to deal with when
  designing the scheduler for an operating system

# Definitions

- We have a shared limited resource that must be shared among several processes
  or threads (or jobs or tasks, etc.)

## Scheduling Policies

- Scheduling:
  - Figuring out how to share a limited resource among many tasks
- Scheduling policy:
  - Process of determining who gets to run when
- Factors that affect a policy:
  1. The number of tasks to run.
  2. The run time of each task.
  3. The arrival time of tasks.
  4. The I/O patterns of the tasks.
      1. Heavy on I/O operations.
      2. CPU intensive (`while(1)`).
  5. Are we allowed to kick tasks off of the CPU?
      1. Preemption

## Assumptions

- When designing our scheduling policies, we will make (and then relax) the
  following assumptions:

  1. Each job runs for the **same amount of time**.
  2. All jobs **arrive at the same time**.
  3. Once started, **each job runs to completion**.
  4. All jobs **only use the CPU** (no I/O).
  5. The **runtime** of each is known beforehand.

# FIFO Scheduling

- We have three jobs: A, B, and C
  - A needs 3 time units
  - B needs 3 time units
  - C needs 3 time units
  
- Show the diagram and calculate the turnaround and response times.

## The Convoy Effect in FIFO

- Let’s relax the first assumption, what do you think can happen in that case?

  1. ~~Each job runs for the **same amount of time**.~~
  2. All jobs **arrive at the same time**.
  3. Once started, **each job runs to completion**.
  4. All jobs **only use the CPU** (no I/O).
  5. The **runtime** of each job is **known** beforehand.
 
- A long-running process can hog the CPU and prevent other processes form
  running, even if they have a short runtime.
- This is called the **convoy** effect:
  - Imagine the person that drives slowly on the left lane, yikes!

# Shortest Job First (SJF)

- ❓ To address the challenge of the FIFO scheduler, what would you suggest?
  - Run the job with the shortest amount of time first.
- So order the jobs by increasing runtime in a priority queue.
- Show diagram and compute the turnaround and response time.

## The Convoy Effect in SJF

- Let’s relax the second assumption, what do you think can happen in that case?

  1. ~~Each job runs for the **same amount of time**.~~
  2. ~~All jobs **arrive at the same time**.~~
  3. Once started, **each job runs to completion**.
  4. All jobs **only use the CPU** (no I/O).
  5. The **runtime** of each job is **known** beforehand.

- ❓ What could happen in this case?
  - Another convoy effect if the longest running process arrives before anyone
    else.

# Shortest Time to Completion First (STCF)

- To address the challenges of the SJF scheduling policy, we need to relax the
  third assumption

  1. ~~Each job runs for the **same amount of time**.~~
  2. ~~All jobs **arrive at the same time**.~~
  3. ~~Once started, **each job runs to completion**.~~
  4. All jobs **only use the CPU** (no I/O).
  5. The **runtime** of each job is **known** beforehand.

- We will introduce the idea of preemption to solve this challenge.
- We prioritize the jobs that have the least amount of time remaining to
  complete.
  - Thus the name shortest time to completion first
  
- If a new task arrives that needs less time to complete than the currently
  running one:
  - Preempt the currently running job and put it back in the queue.
  - Schedule the newly arriving job to run.

- Show the diagram and compute the turnaround and response time

## Fairness

- ❓ What if we a whole bunch of short tasks and a few long tasks?
  - What could happen to the long tasks?
  - This is not a fair system!

# Round Robin (RR)

- Run each job for a fixed time slice called a **quantum**.
- This can help us achieve fairness.

# Activity: Improving the xv6 scheduler

In this activity, we will be exploring the xv6 scheduler by first trying to
understand the way it is actually implemented. We will then think about the
advantages and disadvantages of the current xv6 implementation and try to
identify ways in which it can be improved. Finally, we will spend the remainder
of the session trying to implement said optimization and testing out their
effects.

## Getting the Source Code

In this session, we will be exploring the xv6 kernel again, so we must first get
on the right branch of the code. To do so, navigate to the root of your xv6
source repository (should be in a folder called `xv6-riscv-public`) and then
issue the following commands:

```shell
$ git fetch
$ git checkout klist
$ git pull
```

Make sure that you are now on the `klist` branch of the repository. To check
your current branch, you can use the command:

```shell
$ git branch
```

# Understanding the xv6 Scheduler

To get a better idea of the scheduling approach in xv6, it might be a good idea
to skim [Chapter 7 of the xv6
book](https://pdos.csail.mit.edu/6.828/2022/xv6/book-riscv-rev3.pdf).

In your groups, take a few moments to explore the xv6 kernel's source code and
attempt to answer the following questions about the xv6 scheduler:

- ❓ What scheduling policy is xv6 implementing?
- ❓ How are processes represented in xv6?
- ❓ What are the possible states that an xv6 process can be in?
- ❓ Which function is used to switch from one process back to the
  scheduler and vice versa?
- ❓ How does the xv6 kernel handle scheduling on multiple CPUs?
- ❓ How does the kernel implement its scheduling policy?
  - 🎶 You might find it helpful here to draw a small chain that represents the
    sequence of events that can happen when scheduling is involved.

_Hint_:  Most of what you are going to need to answer the above questions can be
found in the files `kernel/proc.h` and `kernel/proc.c`. Moreover, to answer the
last question accurately, looking at `kernel/trap.c` might also be helpful.

_Hint_: If you have done the userspace threads assignments, you will find some
of the terminology we used there very relevant.

## Advantages of the xv6 Scheduler

After answering the above questions, within your group, think of at least two
possible advantages of the current implementation of the xv6 scheduler. You will
then need to argue for your choices when presenting them.

## Drawbacks of the xv6 Scheduler

After identifying two advantages, within your group, think of at least two
possible disadvantages of the current implementation of the xv6 scheduler. You
will need to argue for your choices when presenting them. You will also need to
provide possible solutions for those disadvantages. You might find it helpful to
think of edge cases that the current implementation of the scheduler does not
consider.

## Improving efficiency

If time permits, start by improving the way the xv6 scheduler handles its
scheduling by changing the data structure that is used to represent the
processes that should be considered for scheduling.

You might find the list api that we define for you [here](../session31/list/)
useful in updating the implementation.

