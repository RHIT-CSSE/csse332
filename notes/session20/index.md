---
layout: post
title: Condition Variables
readtime: true
date: Mon Jan 16 07:29:50 2023
---

# Learning Objectives

At the end of this lecture, you should be able to:

- Perform event synchronization using condition variables.
- Implement thread join using condition variables.

---

# Topics

In this lecture, we will cover the following topics:

- Condition variables.

---

# Notes


<!-- vim-markdown-toc GFM -->

* [Condition Variables](#condition-variables)
  * [Condition Variables API](#condition-variables-api)
* [Activity: Thread Join using Condition Variables](#activity-thread-join-using-condition-variables)
  * [Approach 1](#approach-1)
  * [Approach 2](#approach-2)
  * [Approach 3](#approach-3)
  * [Final solution](#final-solution)
* [Activity](#activity)

<!-- vim-markdown-toc -->

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

# Activity: Thread Join using Condition Variables

- For our activity for the day, we would like to implement `pthread_join` using
  mutex locks and condition variables.
- Let's first run this without adding any implementation and observe what
  happens
  ```shell
  $ make
  $ ./condwait.bin
  ```
- We will see that the parent will exit directly without waiting for the child.
- What happens to the child in that case?
  - The child will become an orphan and we lost the semantics of the thread
    creation and joining approach.
- ❓ How can we use condition variables to implement such an approach?

## Approach 1

- Let's simply use a single condition variable and ignore the need for a mutex
lock.
- So we would do something that looks like the following in the parent:
  ```c
  void thread_join(void)
  {
    if(!done)
      pthread_cond_wait(&c);
  }
  ```
- And in the child:
  ```c
  void thread_exit(void)
  {
    done = 1;
    pthread_cond_signal(&c);
  }
  ```

- ❓ What could go wrong with such an approach?
- Again, think of the scheduler as an adversary, what could be the worst
  possible sequence of events?

## Approach 2

- Let's use a mutex lock then to protect the waiting and signaling on the
  condition variable.
- Something like the following in the parent:
  ```c
  void thread_join(void)
  {
    pthread_mutex_lock(&m);
    pthread_cond_wait(&c, &m);
    pthread_mutex_unlock(&m);
  }
  ```
- And in the child:
  ```c
  void thread_exit(void)
  {
    pthread_mutex_lock(&m);
    pthread_cond_signal(&c);
    pthread_mutex_unlock(&m);
  }
  ```

- ❓ This is still broken, can you think of a case when that would fail?

## Approach 3

- So now, let's combine both approaches together, the `done` variable along with
  the mutex lock.
- In the parent, that would look like the following:
  ```c
  void thread_join(void)
  {
    pthread_mutex_lock(&m);
    if(!done)
      pthread_cond_wait(&c, &m);
    pthread_mutex_unlock(&m);
  }
  ```
- In the child, that would look like the following:
  ```c
  void thread_exit(void)
  {
    pthread_mutex_lock(&m);
    done = 1;
    pthread_cond_signal(&c);
    pthread_mutex_unlock(&m);
  }
  ```

- That looks about right for two threads.
- ❓ Now a tougher question, let's expand to three threads working, and
  we would like `thread_join` to return when either one of the two children
  are done.
  - How can the scheduler mess us up in this case?

## Final solution

- So we will need to protect the access to the `done` variable with a `while`
  loop instead of a simple `if` statement.
- In the parent, that would look like:
  ```c
  void thread_join(void)
  {
    pthread_mutex_lock(&m);
    while(!done)
      pthread_cond_wait(&c, &m);
    pthread_mutex_unlock(&m);
  }
  ```
- In the child, nothing much will change:
  ```c
  void thread_exit(void)
  {
    pthread_mutex_lock(&m);
    done = 1;
    pthread_cond_signal(&c);
    pthread_mutex_unlock(&m);
  }
  ```

- As a general rule, when a thread awakens from `pthread_cond_wait`, it will
  need to recheck the state of the world, as a wake up from a
  `pthread_cond_wait` only suggests that the state of the world has changed.
  - There is nothing that guarantees that the world has also not yet changed
    again by the time the thread is awake.
- So always protect access to shared state variable using a `while` statement
  instead of a simple `if` statement.
  - That will prevent such edge cases from happening, even if sometimes it might
    be a bit redundant.

# Activity

- Try to solve `barrier.c` to get more practice with condition variables.
