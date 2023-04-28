---
layout: post
title: The xv6 Scheduler
readtime: true
date: Thu Feb 2 2023 
---

# Learning Objectives

At the end of this lecture, you should be able to:

- Read the xv6 code and identify the scheduler.
- List the advantages and disadvantages of the xv6 scheduler.
- Optimize the scheduler by solving some of its inefficiencies.

---

# Topics

In this lecture, we will cover the following topics:

- CPU scheduling.
- Data structures.

---

# Notes


<!-- vim-markdown-toc GFM -->

* [Improving the xv6 Scheduler](#improving-the-xv6-scheduler)
* [Make sure you have the right piece of code](#make-sure-you-have-the-right-piece-of-code)
* [Activity 1: Putting processors to sleep](#activity-1-putting-processors-to-sleep)
* [Activity 2: O(1) lookups](#activity-2-o1-lookups)
* [Submission](#submission)

<!-- vim-markdown-toc -->

# Improving the xv6 Scheduler

This session will be dedicated to the following tasks that you will do in your
groups:

1. Experimenting with one of the main inefficiencies of the xv6 scheduler.

2. Implementing a quick and easy fix to the above problem and testing it out.

3. Identifying another possible source of inefficiency in the xv6 scheduler.

4. Developing a possible solution to solve the above problem using our custom
   API.

We will discuss both of the above problems together in class, and then you will
be given time to address those issues.

# Make sure you have the right piece of code

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

# Activity 1: Putting processors to sleep

The fist inefficiency we discussed with the xv6 scheduler happens when there are
no processes to be scheduled (i.e., no processes ready to run). In that case,
the scheduler, on each CPU, will be spinning out of control, wasting CPU cycles
until a new process comes along. This is clearly inefficient. 

The solution we would like to propose is to use the _wait-for-interrupt_ (`wfi`)
instruction from the RISC-V RSA. [Here](wfi/) are the instructions for you to
implement this step, your job to identify the case when you should be issuing
the `wfi` instruction and then testing its impact.

# Activity 2: O(1) lookups

On top of the first problem with the xv6 scheduler, we have discussed that
searching for a process to run has a worst-case runtime of `O(N)`, with `N`
being the number of processes.

To fix this, we would like to change xv6's data strcture from a static array
into a first-in-first-out queue. To help you achieve that, we have provided you
with some [helper](list/) code.  Your job is to switch the scheduler's main data
structure to use a queue, using the linked list API we provided you with.

# Submission

To submit your code, please run `make submit` to generate the submission files
and then upload both `.tar` and `.patch` files to the appropriate [Gradescope
box]({{ site.gradescope_url }}) titles "xv6 Scheduler Activity". We will use
this submission box to keep track of how attended class and participate in this
activity.

