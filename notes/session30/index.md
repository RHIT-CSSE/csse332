---
layout: post
title: Scheduling II
readtime: true
date: Mon Jan 30 2023
---

# Learning Objectives

At the end of this lecture, you should be able to:

- Define the Multi-Level-Feedback-Queue scheduling policy.
- Identify some challenges with implementing the MLFQ policy and how they can be
  addressed.

---

# Topics

In this lecture, we will cover the following topics:

- CPU scheduling using multiple priority queues.
- Dynamic priority adjustment based on thread behavior.

---

# Notes


<!-- vim-markdown-toc GFM -->

* [Motivation](#motivation)
* [Basic MLFQ](#basic-mlfq)
  * [Adjusting Priorities](#adjusting-priorities)
* [Challenges with Basic MLFQ](#challenges-with-basic-mlfq)
  * [Solving Problems 1 and 2](#solving-problems-1-and-2)
  * [Solving Problem 3](#solving-problem-3)
* [Continue xv6 Scheduler Activity](#continue-xv6-scheduler-activity)

<!-- vim-markdown-toc -->

# Motivation
- Last time, we talked about simple scheduling algorithms, such as FIFO, SJF,
  etc. 
- Our conclusion was that round robin was our best bet given our assumptions and
requirements, since it allows us to achieve fairness as best as we can.
- However, we still run the risk of our tasks taking too long to run, especially
  when they are of different natures.
  - For example, we might want to treat an I/O bound job differently than we
  treat a CPU-intensive job.
- ❓ But how do we do that without knowing the nature of the job
  beforehand?
  - Maybe we can learn from the behavior of each job as it enters our system?
  - Thus we introduce the Multi-Level Feedback Queue (MLFQ) scheduling policy.

# Basic MLFQ

- At its core, the MLFQ policy operates by having multiple queues with multiple
  priorities.
- First, here are the rules for scheduling from these queues. Consider two jobs
  A and B that are part of an MLFQ system.
  1. If `Pr(A) > Pr(B)`, then A runs before B (A is in a higher priority queue).
  2. If `Pr(A) = Pr(B)`, then A and B run in round-robin.
- What this means is that each priority has its own quantum value.
  - Generally, lower priority queues have higher quantums, why ❓
- Within a queue, we run in round-robin, thus achieving fairness between jobs in
  the same queue.
- Finally, we never run a job from a lower pririty queue until all jobs from
  higher priority queues are done.

## Adjusting Priorities

- Ok, but how do we decide on the priorities of the jobs?
- We will augment our policy with the following new rules:
  1. If `Pr(A) > Pr(B)`, then A runs before B (A is in a higher priority queue).
  2. If `Pr(A) = Pr(B)`, then A and B run in round-robin.
  3. Initially, all jobs enter at the highest priority level.
  4. If a process runs for a full quantum, then we demote that process into a
     lower priority level.
  5. If a process runs for less than its quantum, then we keep that process in
     its current priority level.

- The intuition here is that our I/O bound processes will end up in the highest
  priority queues while CPU-intentive processes end up in the lowest priority
  queues.
- Example here with two processes: A is CPU-intensive and B is I/O-bound.

# Challenges with Basic MLFQ

- At a first glance, the MLFQ policy sounds good. But there are two main
problems with it, what are those :question:
  1. **Starvation**: If I/O-bound processes keep coming, then processes that are
     CPU-intentive become starved in the lower priority levels.
  2. **No changes in behavior**: This is similar to the starvation problem, if a
     process is CPU-intensive at some point, but then becomes I/O-bound, there
     is no coming back for it.
  3. **Malicious process**: A malicious process can game our MLFQ policy by
     running very close to its quantum and then yielding right before that
     happens. 

## Solving Problems 1 and 2

- To prevent process from starving and allow them to change their behavior, we
  add the following rule to our MLFQ scheduling policy.
  1. If `Pr(A) > Pr(B)`, then A runs before B (A is in a higher priority queue).
  2. If `Pr(A) = Pr(B)`, then A and B run in round-robin.
  3. Initially, all jobs enter at the highest priority level.
  4. If a process runs for a full quantum, then we demote that process into a
     lower priority level.
  5. If a process runs for less than its quantum, then we keep that process in
     its current priority level.
  6. **Every period of time `S`, promote all jobs to the highest prioirity level
     queue**.
- Intuition is that by promoting everyone back into the highest priority level,
  then will get a at least one chance to run in the highest level quantum before
  being demoted.
  - Also, if a process changes bevahior, its new behavior will be updated when
    it gets pushed into the highest priority queue.

## Solving Problem 3

- To prevent malicious processes from gaming our system, we will add some form
  of accountability. 
- We will update rule 5 above to the following:
- Keep track of a process's runtime so far, when it reaches a full quantum (even
  across several runs), then demote that process into the next lower priority
  queue.

# Continue xv6 Scheduler Activity

- Let's pick up our discussion of the xv6 scheduler from where we left it off
  last time.  

