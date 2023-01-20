---
layout: post
title: Deadlocks
readtime: true
date: Tue Jan 17 07:20:09 2023 
---

# Learning Objectives

At the end of this lecture, you should be able to:

- List the four conditions for a deadlock to occur.
- Represent how a deadlock can happen using resource allocation graphs.
- Solve a deadlock-riddled concurrency problem.

---

# Topics

In this lecture, we will cover the following topics:

- Deadlocks and their conditions.

---

# Notes


<!-- vim-markdown-toc GFM -->

* [Definition of a Deadlock](#definition-of-a-deadlock)
* [Conditions for a Deadlock](#conditions-for-a-deadlock)
* [Resource Allocation Graphs](#resource-allocation-graphs)
* [Cigarette Smokers Problem](#cigarette-smokers-problem)

<!-- vim-markdown-toc -->

# Definition of a Deadlock

- A set of threads are said to be in a deadlock state when:
  - Every thread is waiting on an event,
  - that even is only caused by another thread in the set.

- Consider the following simple example:
  ```c
  void *thread1(void *arg)
  {
    pthread_mutex_lock(&m1);
    pthread_mutex_lock(&m2);

    // do stuff

    pthread_mutex_unlock(&m2);
    pthread_mutex_unlock(&m1);
  }

  void *thread2(void *arg)
  {
    pthread_mutex_lock(&m2);
    pthread_mutex_lock(&m1);

    // do stuff

    pthread_mutex_unlock(&m1);
    pthread_mutex_unlock(&m2);
  }
  ```
- In this case, a situation might occur when:
  - T1 locks m1.
  - T2 locks m2.
  - Now we are stuck, T1 is waiting on T2 to release m2 while T2 is waiting on
    T1 to release m1.
  - Thus a deadlock!

# Conditions for a Deadlock

- There are four essential conditions for a deadlock to occur:
  1. Hold & Wait: A thread is requesting and waits on a resource while also
     holding another resource.

  2. No Preemption: Once a thread owns a resource, no one can forcefully take it
     away.

  3. Circular Wait: Thread must be waiting on each other in a circular fashion.

  4. Mutual Exclusion: No sharing of resources can happen between the threads.

- Once all four conditions are satisfied, then we have a deadlock!

# Resource Allocation Graphs

- One way to represent the circular wait condition is using a Resource
  Allocation Graph (RAG)
- If the RAG has a cycle, then __there is a possibility__ of a deadlock.

# Cigarette Smokers Problem

- Look at `cigarette_smokers.c`
