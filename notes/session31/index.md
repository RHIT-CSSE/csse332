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
  * [Some Helpers](#some-helpers)
* [Submission](#submission)

<!-- vim-markdown-toc -->

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

