---
title: Userspace Threads I
layout: post
date: Wed Jan 25 19:44:36 2023 
readtime: true
---

# Introduction

Let's say that you would like to write a multi-threaded application, but your
operating system doesn't support threads (i.e. it just runs whole processes,
which can't have sub-parts that are scheduled separately).  Are you doomed to a
single threaded program - no!  You can implement your own user-space (as opposed
to kernel-space) threads.  The process you'll use is very similar to the way the
OS itself handles threads. You will have full control over when these threads
get to run, in which order, and when do they get taken off of the CPU.

# Learning Objectives

At then end of this lab, you should be able to:

- Develop a basic multi-threading library built entirely in user space.
- Develop a scheduler that attempts to pick the next thread to run based on the
  set of available threads.

# Getting the Source Code

For this lab, you will be using the native Linux virtual machine (or baremetal
machine if you have one) and not the xv6 operation system. Please note that this
lab might behave slightly differently if you are running it on Windows or
MacOs; therefore, we highly recommend that you stick to using a Linux machine,
either natively or via WSL2.

To obtain the starting code for this lab, navigate to the top level directory of
your __csse332 class repository__ and `cd` into the `labs/lab09` directory as
follows:
```shell
$ cd /path/to/csse332/git/repository/
$ git pull
$ cd labs/lab09
```

