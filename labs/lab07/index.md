---
title: Condition Variables Basics
layout: post
date: Mon Jan 16 07:48:02 2023
readtime: true
---

# Condition Variables Basics

In this assignment, we will have a quick practice with condition variables and
mutexes. There are three problems that you must solve using condition variables. 

# Learning Objectives

At the end of this assignment, you should be able to:

- Implement multi-threaded code using the `pthreads` library.
- Solve basic concurrency problems using condition variables.

# Getting the Source Code

For this lab, you will be using the native Linux virtual machine (or baremetal
machine if you have one) and not the xv6 operation system. Please note that this
lab might behave slightly differently if you are running it on Windows or
MacOs; therefore, we highly recommend that you stick to using a Linux machine,
either natively or via WSL2.

To obtain the starting code for this lab, navigate to the top level directory of
your __csse332 class repository__ and `cd` into the `labs/lab07` directory as
follows:
```shell
$ cd /path/to/csse332/git/repository/
$ git pull
$ cd labs/lab07
```

# In-order (20 points)

This problem refers to the file `inorder.c`. In this system, there are threads
numbered 1 through 4 and there is a critical section that the threads are trying
to enter. The threads are required to enter the critical section in order (e.g.,
1 enters the critical section first, 2 following that, then 3, and finally 4).

If a thread `n` finishes and thread `n+1` is waiting, then thread `n+1`
immediately executes. __However, if thread `n+1` is not waiting, no threads
should be allowed into the critical section until the correct thread arrives in
order.__ For example, if thread 1 finishes and thread 2 is not yet waiting to
enter the critical section, then no other thread should be allowed in until
thread 2 comes in and finishes. 

# Max (20 points)

This problem refers to the file `max.c`. In this system, there is a special
critical section in which 3 threads are allowed to be in at once. Once 3 threads
are in the critical section, any further threads must wait until one of the
threads leaves the critical section. 


# Producer Consumer Again (20 points)

This problem refers to the file `prodcons_condvar.c`.In this system, there 2
types of processes - producers and consumers. Producers write integers to the
shared buffer data. Consumers read integers from the share buffer data.
Everybody has to be careful when modifying data and the integer that keeps track
of how many elements are in data. 

When the producers are being slow (i.e., the buffer is empty), the consumers
should be blocked. Alternatively, when the consumers are being slow (i.e., the
buffer is full), the producers should be blocked.

The order in which the items are consumed does not matter, i.e., it does not
need to be the same as the order they were produced. 


## Guidance on how we grade:
Some guidance on how we grade:

* You can get 25% of points if your solution reliably produces correct output
  with the given data (and small modifications). 0 points if doing this requires
  weird sleeps or other hacky non-general purpose code.
* You can get 50% points you did above and your solution is plausible and does
  not do something obviously bad. It's impossible to enumerate every obviously bad
  thing but here's a big one : having multiple concurrent threads modify a global
  variable. Basically this is for solutions that basically are OK but miss some
  edge cases.
* You can get 50% points if you have a fully working solution but you utilize
  non-standard functions (i.e. stuff other than `pthread_cond_signal/wait`,
  `pthread_mutex_lock/unlock`)
* A fully correct solution, even if it's not the one we envisioned, is worth
  100%
