---
layout: post
title: Mutex Locks and Condition Variables
readtime: true
date: Fri Jan 13 07:23:26 2023 
---

# Learning Objectives

At the end of this lecture, you should be able to:

- Ensure mutual exclusion using mutex locks.
- Implement optimal locking mechanisms without sacrificing efficiency.
- Perform event synchronization using condition variables.

---

# Topics

In this lecture, we will cover the following topics:

- Mutex locks.
- Condition variables.

---

# Notes


<!-- vim-markdown-toc GFM -->

* [The Need for Mutual Exclusion](#the-need-for-mutual-exclusion)
* [Some Definitions](#some-definitions)
  * [Critical Section](#critical-section)
  * [Race Condition](#race-condition)
* [Thoughts about the Scheduler](#thoughts-about-the-scheduler)
* [Mutex Locks](#mutex-locks)
  * [Mutex API](#mutex-api)
    * [Activity](#activity)
* [Condition Variables](#condition-variables)
  * [Condition Variables API](#condition-variables-api)

<!-- vim-markdown-toc -->

# The Need for Mutual Exclusion

- Let's go back to our `thread_example.c` from last lecture.
- Each thread is trying to add 1 `max` times to a counter variable.
- Let’s try to run this several times and see what we get
- Notice that once we get to a large number, the result of our addition deviates
  from the expected answer. Why?
- Let’s take a closer look and examine what is happening
- The code `counter = counter + 1` results in the following instructions in
  assembly (assuming MIPS):

  ```asm
  la    $t0, counter
  lw    $t1, 0($t0)
  addi  $t1, $t1, 1
  sw    $t1, 0($t0)
  ```

- There is no guarantee that whenever any of these instructions execute, the
  thread will be preempted by the schedule and the other thread will start
  execution
- When is the absolute worst time for the scheduler to preempt us?
  - It will most likely preempt us at that time!

# Some Definitions

- We will look at two important definitions that we need to get familiar with.

## Critical Section

- In a multi-threaded code, a critical section of code is one that access a
  shared resource from multiple threads of execution.
- In our example (and also in `mutex.c`), that section corresponds to:
  ```c
  sum += array[i]
  ```
- In this case, multiple threads are attempting to read the `array` variable,
  which resides on the heap, and is thus shared between all threads.
- At the same time, multiple threads are attempting to read and write to the
  `sum` variable, which is a global variable, and is also shared between all
  threads.

- ❓ Could anything go wrong with the `array` variable?
  - Probably not because the threads only **read** the content of this array.
  - So we are good in this case.

- ❓ What about the `sum` variable?
  - We are likely to face issues because different threads are attempting to
    write to this variable, which might cause some issues.

## Race Condition

- A race condition occurs when multiple threads enter a critical section at
  roughly the same time, and attempt to update or change a shared memory area
  (variable, structure, array, etc.)
- That is the recipe for disaster in multi-threaded code and that is what we
  need to deal with in this class.

# Thoughts about the Scheduler

- In such problems, it is often good to think about the scheduler in an
  adversarial way.
- The scheduler is really malicious; it will pick the worst time to swap a
  process and will introduce concurrency bugs.
- Think back to our first example, a thread might be preempted even while in the
  middle of the execution of a single line of code.

# Mutex Locks

- ❓ Thinking about our first example, what would be a good way to deal with the
  concurrency issue?
  - Maybe if we had a mechanism to ensure that once a thread enters the
    critical section, no other threads are allowed within that section until
    that first thread is done.
- That is what a mutex lock can ensure for us!
  - It is binary lock, in the sense that you either have the lock or you don't
    have it.
  - **No two threads can hold the same lock at the same time!**
  - A mutex lock is a good way to ensure mutual exclusion.

## Mutex API

- To create a mutex lock, simple use:
  ```c
  pthread_mutet_t lock;
  ```
- To initialize a mutex lock, we can use one of two ways:
  1. `pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;`
  2. `pthread_mutex_init(&lock, NULL);`
- You can use either method, depending on the needs of your program.

- To grab a lock, you can use:
  ```c
  pthread_mutex_lock(&lock);
  ```
  - Note that if another thread has the lock, then the thread trying to acquire
    the lock will go to sleep.
    - It will remain sleeping until the lock is released, at which point the
      kernel will wake up **exactly** one sleeping process on that lock.

- To release a lock, you can use:
  ```c
  pthread_mutex_unlock(&lock);
  ```
  - This will cause one thread (if any) that is waiting on the lock to wake up.
  - Waiting threads are woken up in first come first serve order.

- ⚠  Only the thread that grabbed the lock should be the one that unlocks it!
  - It is rarely a good idea to unlock a lock that has been locked by another
    process.

- Be careful when using mutex lock not to create **deadlocks**.
  - It can occur when a thread is waiting on a lock that is never going to be
    released.
  - We will talk more about the conditions of a deadlock later on in class.

### Activity

- Let's solve the problem in `mutex.c`, what would be a good approach to do
  that?
  - Keep efficiency in mind, is there something better that we can do?

# Condition Variables

- Sometimes, a binary lock might not be enough for solving a concurrency
  problem.
- How great it would be if at any point in time where you don't like the state
  of the world, you can go to sleep and ask to be woken up when the state of the
  world is likely to have changed!
  - We will talk more about why I used the word _likely_ here.

- A condition variable is:
  - a queue of threads sleeping and waiting for the world to get better.
  - someone will fix the world and give a hint to those processes that they can
    wake up.
  - ⚠  There is no guarantee that the state of the world has not changed
    between the time when a thread is sent the wake up signal, and when it
    actually wakes up.

## Condition Variables API

- To create a condition variable, you can use:
  ```c
  pthread_cond_t c;
  ```
- To initialize a condition variable, you can use:
  ```c
  pthread_cond_init(&c, 0);
  ```
- Note, for purposes that will become clear later on, a condition variable is
  **always accompanied** by a mutex lock!
- Therefore, to wait on a condition variable, use:
  ```c
  pthread_cond_wait(pthread_cond_t *c, pthread_mutex_t *m);
  ```
- To signal on a condition variable, you can use:
  ```c
  pthread_cond_signal(pthread_cond_t *);
  ```
- You can also broadcast on a condition variable to wake up all sleeping threads
  using
  ```c
  pthread_cond_broadcast(pthread_cond_t*);
  ```
- Let's solve `condwait.c`
