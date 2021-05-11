---
layout: post
title: Term Project (Milestone 3: Checkpoint II)
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
* [Junior:](#junior)
* [Senior:](#senior)
* [Rubric:](#rubric)

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

# Junior:

# Senior:

# Rubric:

