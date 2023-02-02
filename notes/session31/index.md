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

* [Introduction](#introduction)
  * [Getting the Source Code](#getting-the-source-code)
* [Understanding the xv6 Scheduler](#understanding-the-xv6-scheduler)
  * [Advantages of the xv6 Scheduler](#advantages-of-the-xv6-scheduler)
  * [Drawbacks of the xv6 Scheduler](#drawbacks-of-the-xv6-scheduler)
* [Improving the xv6 Scheduler](#improving-the-xv6-scheduler)
  * [Some Helpers](#some-helpers)
* [Submission](#submission)

<!-- vim-markdown-toc -->

# Introduction

In this session, we will be exploring the xv6 scheduler by first trying to
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

# Improving the xv6 Scheduler

The rest of this session will be dedicated to the following tasks that you will
do in your groups:

1. Experimenting with one of the main inefficiencies of the xv6 scheduler.

2. Implementing a quick and easy fix to the above problem and testing it out.

3. Identifying another possible source of inefficiency in the xv6 scheduler.

4. Developing a possible solution to solve the above problem using our custom
   API.

We will discuss both of the above problems together in class, and then you will
be given time to address those issues.

## Some Helpers

To help you out on your tasks, we have provided you with some pointers that you
might find helpful. Checkout the [`wfi` RISC-V instruction](wfi/) to solve the
first problem and the custom-built [list API](list/) to help you solve the
second one.

# Submission

To submit your code, please run `make submit` to generate the submission files
and then upload both `.tar` and `.patch` files to the appropriate [Gradescope
box]({{ site.gradescope_url }}) titles "xv6 Scheduler Activity". We will use
this submission box to keep track of how attended class and participate in this
activity.

