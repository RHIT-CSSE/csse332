---
layout: post
title: Buffalo's Unofficial Guide to CV Problems*
readtime: true
date: Mon Apr 17 2023 
---

Last Updated: Spring 2023.

Most of the simple solutions to concurrency problems using condition variables
fall into a simple style.  This is not necessarily the only way condition
variables can be applied correctly but doing it this way makes it easier to see
if things are correct.  Also limiting your options to one style makes it less
overwhelming to start until things start to feel natural and obvious.  So I
recommend you follow this system.

Note that this guidance is rules of thumb and hints; it’s not a substitute for
really thinking about your problem.  You won’t get leverage with the graders by
saying _hey this \<incorrect thing I did\> is consistent with the guidance_ or
_the guidance didn’t warn me not to \<incorrect thing I did\>_.  The goal is to
solve the problem without concurrency bugs – any style advice that introduces
bugs should be ignored.

Although we can provide hints, you still must figure out your concurrency state,
decide when threads need to wait, decide when threads need to signal,
etc. That creative problem solving is the essence of writing correct
concurrent code, and learning that is the essential skill we hope you
will internalize on Exam 2.  That kind of problem solving can never be
condensed into a simple recipe, that's why some day you’ll hopefully
get paid the 💰 💰 .

# Getting Started

Here are some general guidelines to get your started:

- You’ll need 1 mutex, any number of condition variables depending on the number
  of reasons to wait.

  Note that the mutex is to protect the state of the world (or the concurrency
  state) and not the critical section. So any time you touch the state of
  the word, make sure you hold the lock on you.

- You’ll also need some number of variables to represent the _concurrency
  state_ (aka, the state of the world).  I use concurrency state here to
  represent part of the (usually global) state that your code accesses to
  enforce correctness. 

  For example, if there are only 3 threads allowed in the
  critical section, we need some variable to represent how many threads are
  currently in the critical section.

- I often have some idea of what the CVs/state will be when I start, but end up
  tweaking them a bit as I write the code.

# In Your Code

- If your thread might wait OR needs to read/update the concurrency state, lock
  the mutex. 

- When holding the lock, you can safely read/write at a consistent time.
  Usually, you’ll want hold the lock for the entire read-write experience.   

  For example, don’t lock/unlock to read/write var A, then lock/unlock to
  read/write var B, as this will tend to introduce bugs where A and B have an
  inconsistent state.  Similarly, don’t read variable A, lock, unlock, re-lock,
  and then write variable A. 

  Here's a more concrete example:
  ```c
  void *thread_fn(void *ignore)
  {
    pthread_mutex_lock(&lock);
    while(people_in_room > 5) {
      pthread_cond_wait(&cv, &lock);
    }
    pthread_mutex_unlock(&lock);

    // this is dangeours, this gap in time where you don't hold the
    // lock might allow another thread to sneak by and change the
    // value of people_in_room.
    // Remember that the scheduler hates you, if a context switch
    // is going to hurt you, then that's when the scheduler will do it!

    pthread_mutex_lock(&lock);
    people_in_room++;
    pthread_mutex_unlock(&lock);
  }
  ```


- If reading the concurrency state indicates you need to wait – use wait on an
  appropriate signal within a **while loop** (and this implicitly unlocks). 

- After you wake from a wait, remember you are holding the lock again so it is
  safe to read or write to the state 

- If it is safe to proceed (as opposed to waiting) unlock the mutex after you
  know it’s safe and after you have updated the state appropriately (see example
  above).

- Never hold the mutex for a long amount of time (more accurately, I usually say
  never hold the mutex for an unbounded amount of time).  Especially never hold
  the mutex during one of the sleeps we use to simulate long processing in these
  problems. 

  Holding the mutex for a long time makes it unsafe to read/update the
  concurrency state (because doing so might introduce a long pause or deadlock). 

- Never use the mutex to protect something – only use the mutex to safeguard the
  concurrency state.  Use waiting to ensure that all threads do not proceed when
  it is unsafe to do so. 

- Every wait needs a corresponding signal or signals.  Consider carefully
  whether you wish to use signal or broadcast.  Insert the signal/broadcasts
  where they are needed. 

  > Try to avoid threads waking up and immediately re-sleeping but sometimes
    this is unavoidable. 

- Never busy wait.  A process that is unable to proceed because it’s waiting for
  some other process to work should always be waiting for a signal.

- Always grab the lock before signaling. It might seem superfluous but it will
  help you avoid very weird conditions that are depending on the implementation
  of the `pthreads` library.

- Do not make any assumptions about the order in which thing wake up on a
  `pthread_cond_signal`. From the POSIX specification, we know that at least one
  thread will wake up, but which one is left for the scheduler/implementation to
  determine.

# The Pattern

Altogether, this pattern will tend to produce patterns that look like this
(again, simply guidance here, not substitute for your own thinking/coding): 

```c
/* ORDINARY CODE  */

// Lock mutex 
pthread_mutex_lock(&lock);

// Read/Update state, maybe signal 

while(some undesired condition) { 
  // Wait on some signal with mutex 
  pthread_cond_wait(&cv, &lock);
} 
// IMPORTANT: Recall that at this point, you HAVE the mutex

// Read/Update state, maybe signal 

// Unlock mutex 
pthread_mutex_unlock(&lock);

/* CODE THAT HAS SPECIAL RULES, GENERALLY A CRITICAL SECTION */

// Lock mutex 
pthread_mutex_lock(&lock);

// Read/Update state, maybe signal 

// Unlock mutex 
pthread_mutex_unlock(&lock);

/* ORDINARY CODE  */
```

# Some Pitfalls

Here's a small list of typical pitfalls that we have seen students attempt to do
when they feel stuck on a problem. Generally, doing one of these is not a good
idea.

- When reading the man pages, you might discover new functions that might seem
  appealing, do not make a point to try out these new functions during an exam. 
  Try those out on your free time.

  For example, some students, when running into a deadlock, discover that
  `pthread_mutex_trylock` attempts to grab a lock, but does not block if it is
  not in an unlocked state. That is not the way to solve a deadlock!

- Swapping out `pthread_cond_signal` for `pthread_cond_broadcast` (and vice
  versa) in the hopes that it magically solves a concurrency problem is not a
  good idea. You should be intentional about your signals/broadcasts.

- Do not try to dance with where your locks/unlocks should go. Changing the
  locations of your lock/unlock statements is not going to cut it.  Go back to
  your paper and revisit your design.

- Make sure that each `pthread_cond_wait` has at least one corresponding
  `pthread_cond_signal` (or `pthread_cond_broadcast`).

- Similarly, make sure that each `pthread_mutex_lock` has exactly one
  corresponding `pthread_mutex_unlock`.

  > Do not forget about the implicit lock/unlock statements in
    `pthread_cond_wait`.

- Be wary of "over-locking". For example, doing something like this is bad:
  ```c
  void *thread_fn(void *arg) {
    pthread_mutex_lock(&lock);
    while(some undersired condition) {
      pthread_cond_wait(&cv, &lock);
    }

    /* CODE THAT HAS SPECIAL RULES, GENERALLY A CRITICAL SECTION */

    pthread_cond_signal(&cv);
    pthread_mutex_unlock(&lock);
  }
  ```
  Note that in the code above, the **entire** code block is guarded by a signal
  lock, which renders the condition variables pretty much superfluous. Exacly
  one thread can be that area at a time, so your code will run completely
  serially, which defeats the purpose of using condition variables.

- If you need to count the number of waiting threads, be careful not to double
  count a thread that goes back to sleep. For example:
  ```c
  pthread_mutex_lock(&lock);
  while(some undesired condition) {
    num_waiting_threads++;
    pthread_cond_wait(&cv, &lock);
  }
  num_waiting_threads--;
  pthread_mutex_unlock(&lock);
  ```
  This can be dangerous if multiple threads are awakened by broadcast, but only
  a subset of them exit the loop. In that case, each thread that waits again
  will double count itself as another waiting thread, and the
  `num_waiting_threads` variable will never go back to 0.

  You can either do this:
  ```c
  pthread_mutex_lock(&lock);
  while(some undesired condition) {
    num_waiting_threads++;
    pthread_cond_wait(&cv, &lock);
    num_waiting_threads--;
  }
  pthread_mutex_unlock(&lock);
  ```
  or,
  ```c
  pthread_mutex_lock(&lock);
  num_waiting_threads++; // assume I'm gonna wait
  while(some undesired condition) {
    pthread_cond_wait(&cv, &lock);
  }
  num_waiting_threads--; // decrement anyway, even if I didn't really wait
  pthread_mutex_unlock(&lock);
  ```


---

\* With some modifications and additions by Mohammad.
