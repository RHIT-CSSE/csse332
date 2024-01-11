---
title: Threads and Mutex Basics
layout: post
date: January 9 2022
readtime: true
gh-repo: rhit-csse332/csse332
---

# Threads and Mutex Basics

This lab serves as an introduction to the basics of creating and manipulating
`pthreads` threads as well as a direct application of using mutex locks to solve
basic concurrency problems.

# Learning Objectives

At the end of this assignment, you should be able to:

- Implement multi-threaded code using the `pthreads` library.
- Distribute the work load between multiple threads in a multi-threaded program.
- Implement basic usage of mutex locks to solve concurrency problems.

# Getting the Source Code

We will do this lab in the `main` branch of your labs repository. To make sure
you are on the right branch, check it out using:

  ```sh
  $ git branch
  ```
The branch you are currently on will be highlighted for you (with a \* next to
its name).

If you are working on the `main` or `master` branch, then follow these
instructions:

  ```sh
  $ git fetch upstream
  $ git pull upstream main
  ```
At this stage, you should have the latest copy of the code, and you are good to
get started. The starter code is contained under the `threads_basics/` directory.

If you are currently on a different branch (say you are still on
`clab_solution` from a previous lab), then we need to switch to `main` or
`master` (depending on your default's name).

First, add, commit, and push your changes to the `clab_solution` to make sure
you do not lose any progress you did on the last lab. To check the status of
your current branch, you can use:
  ```sh
  $ git status
  ```
This will show you all the files you have modified and have not yet committed
and pushed. Make sure you `add` those files, then `commit` your changes, and
`push` them.

If `git push` complains about not knowing where to push, you'd want to push the
current branch you are on. So for example, if I am working on `clab_solution`,
then I'd want to do `git push origin clab_solution`.

Now, you are ready to swap back into `main` (or `master`).

```sh
$ git checkout main
```

Then, grab the latest changes using:

```sh
$ git fetch upstream
$ git pull upstream main
```

At this stage, you should have the latest copy of the code, and you are good to
get started. The starter code is contained under the `threads_basics/` directory.

# Building things

All the files in this lab can be built like using the provided `makefile`.

# Thread Factoring (20 points)

Look in the `thread_factoring` project in the `threads_basics/` directory.

Start by understanding the code in `threadExample.c` (you may have seen this
example already, but if not review it).

Then use threads to parallelize the problem given in factoring.  If you do it
correctly you should be able to see the correct number of threads working in
parallel.

Here's some sample output from my solution.  Note that when given two threads,
my solution will make one thread handle evens and one thread handle odds.  Your
approach does not have to split up the problem this way, as long as it divides
the values to check between each thread approximately evenly.

    Give a number to factor.
    143
    How man threads should I create?
    2
    running thread
    running thread
    thread 2 testing 3
    thread 2 testing 5
    thread 2 testing 7
    thread 2 testing 9
    thread 2 testing 11
    11 is a factor
    thread 2 testing 13
    13 is a factor
    thread 2 testing 15
    thread 2 testing 17
    thread 2 testing 19
    thread 2 testing 21
    thread 1 testing 2
    thread 1 testing 4
    thread 1 testing 6
    thread 1 testing 8
    thread 1 testing 10
    thread 1 testing 12
    thread 1 testing 14
    thread 1 testing 16
    thread 2 testing 23
    thread 2 testing 25
    thread 2 testing 27
    thread 2 testing 29
    thread 2 testing 31
    thread 2 testing 33
    thread 2 testing 35
    thread 2 testing 37
    thread 2 testing 39
    thread 2 testing 41
    thread 2 testing 43
    thread 2 testing 45
    thread 2 testing 47
    thread 2 testing 49
    thread 1 testing 18
    thread 1 testing 20
    thread 1 testing 22
    thread 1 testing 24
    thread 1 testing 26
    thread 1 testing 28
    thread 1 testing 30
    thread 1 testing 32
    thread 1 testing 34
    thread 1 testing 36
    thread 1 testing 38
    thread 1 testing 40
    thread 1 testing 42
    thread 1 testing 44
    thread 1 testing 46
    thread 1 testing 48
    thread 1 testing 50
    thread 1 testing 52
    thread 1 testing 54
    thread 1 testing 56
    thread 1 testing 58
    thread 1 testing 60
    thread 1 testing 62
    thread 1 testing 64
    thread 1 testing 66
    thread 1 testing 68
    thread 1 testing 70
    thread 2 testing 51
    thread 2 testing 53
    thread 2 testing 55
    thread 2 testing 57
    thread 2 testing 59
    thread 2 testing 61
    thread 2 testing 63
    thread 2 testing 65
    thread 2 testing 67
    thread 2 testing 69
    thread 2 testing 71

You will need to pass parameters to your thread functions to solve this problem.
Use void pointers just as we did in class during the threading activities.


# Thread Sorting (40 points)

Look at the `thread_sorting` project in the lab directory. 

In this program you will demonstrate your understanding of thread creation,
joining, and just some general purpose concurrent algorithm design in C.

We want time how long it takes to sort with various sort algorithms.  We'll do
this with 3 different sorting methods (all the sorting code is given to you).
We will do this sorting in parallel using threads.  We will print some info
about how fast the various sort methods were.


The program is in the file `threads.c` and takes two command line arguments: 
    
1.  the number of threads to create (n),
2.  the number of numbers each thread should sort

Here's some example output from my solution:

    ```shell
    $ ./threadSort_solution.bin 6 5                                      
    Sorting indexes 0-4 with brute force
    Sorting indexes 5-9 with bubble
    Sorting indexes 10-14 with merge
    Sorting indexes 15-19 with brute force
    Sorting indexes 20-24 with bubble
    Sorting indexes 25-29 with merge
    Sorting indexes 20-24 with bubble done in 2 usecs
    Sorting indexes 15-19 with brute force done in 2 usecs
    Sorting indexes 10-14 with merge done in 78 usecs
    Sorting indexes 5-9 with bubble done in 1 usecs
    Sorting indexes 25-29 with merge done in 52 usecs
    Sorting indexes 0-4 with brute force done in 1 usecs
    brute force avg 1.500000 min 1 max 2
    bubble avg 1.500000 min 1 max 2
    merge avg 65.000000 min 52 max 78
    Result array:
    26 27 28 29 30 
    21 22 23 24 25 
    16 17 18 19 20 
    11 12 13 14 15 
    6 7 8 9 10 
    1 2 3 4 5 
    ```

**Note that the final result is only regionally sorted.** Namely, only the
numbers within a group which is assigned to one thread are sorted. The overall
data array doesn't need to be sorted in this lab.

1.  We've given you code that handles the input arguments and generating the
    numbers to sort, so you don't need to worry about that.

2.  Use a loop to create n parallel threads, distributing the number of values
    evenly across each thread. 
    
    Each thread will sort its group of values using a different sorting
    algorithm. In each thread, call one of three sorting algorithms. One third
    of the threads created should use a brute force sorting algorithm.  One
    third should use bubble sort. And finally, one third should use merge sort.
    You can assume it divides evenly.  I built a function called
    `thread_dispatch` that my `pthread_create` calls.  `thread_dispatch` then
    looks at its parameters to figure out which of the 3 sorting functions it
    ought to run (exactly what those parameters are though, is up to you).
    
    Make your `thread_dispatch` print `"Sorting indexes..."` messages as in the
    example output above.  Note that I put a `sleep(1)` after the first Sorting
    indexes `printf` - this makes its obvious my threads run in parallel even if
    the sorting they do is really fast.
    
    Be sure to test this code and make sure your result array output matches
    mine before you continue on.  Don't worry about tracking time (the next
    part) till the basic sorting works.
    
3. Now make each thread track how long it takes to do the sort.  Here's some
   example code for time tracking:
   
   ```c
   struct timeval startt, stopt;
   suseconds_t usecs_passed;
   gettimeofday(&startt, NULL);
   // some code that takes time
   gettimeofday(&stopt, NULL);
   usecs_passed = stopt.tv_usec - startt.tv_usec;
   ```
  
   The time elapsed needs to be stored somewhere so it can be aggregated up
   after all threads are done.
   
4.  In a loop, the parent thread should wait (using `pthread_join`) for each
    thread to complete. Once all of the of the n threads have completed, the
    main thread calculates the maximum, minimum, and mean values for the
    execution times of each sorting algorithm.  It should then print these
    values to the console.

# Basic mutexes 1 (20 points)

## The problem

Compile and run `add_a_lot.c`.  You will see that it uses multiple threads to
repeatedly add to a global variable.  However, the final sum is not correct
because as multiple threads edit the variable they interfere.

To be clear, a variable add looks something like this in assembly:

1. Load the variable from memory into the register
2. Add, possibly outputting in some other register
3. Store the variable in the output register into memory

However, if we have multiple threads acting at once, the interleaving can cause
the count to fail.

1. Load the variable from memory into the register (thread 1)
2. Load the variable from memory into the register (thread 2)
3. Add, outputting in some other register (thread 2)
4. Store the variable in the output register into memory (thread 2)
5. Add, outputting in some other register (thread 1)
6. Store the variable in the output register into memory (thread 1) and
   discarding Thread 2's change

## The solution - Mutex locks

You might be tempted to try to use some tricky stuff involving special booleans
to prevent concurrent modification.  We'll explain more in future classes, but
for now realize that these kind of things tend to have their own problematic
edge cases unless built very very carefully.  Let's not do that ourselves -
instead we'll use mutex locks.

Mutex locks are one of a variety of concurrency synchronization mechanisms your
system might provide.  They are very powerful and flexible, but in this
assignment we'll use them in the simplest possible way.


## Operation

Mutex locks represent a binary lock (think of a padlock that can be in two
states: unlocked and locked). There are two operations:

1. `pthread_mutex_lock` - Try to lock the mutex, sleep if mutex is locked
2. `pthread_mutex_unlock`  - Unlock a mutex that has already been locked. Note
   that it MUST be the same thread that locked the lock to be the one that
   unlocks it. 

These are all carefully designed so that, regardless of the interleaving of
calls, the mutex locks behaves consistently.  For example, if the mutex is
unlocked, and two threads try to lock it, then exactly one of the threads will
be able to lock the mutex and proceed, while the other one will find it locked
and wait for it to be unlocked.

How can we use these two operations to protect our code?  We want to use a mutex
lock to ensure that multiple threads never enter our "critical section" (that
is, our global increment) at the same time.

See if you can figure out how to do this on your own without peeking at the next
section.


## Using mutex locks to protect a critical section

So the idea is that we're going to create a mutex and initialize it to be in the
unlocked state.

Then in the code we want to protect from interference, we'll to this:

1. `pthread_mutex_lock` - if the lock is unlocked, then thread will proceed. If
   the lock is locked, then the thread will block waiting for it to be
   unlocked.
2. Now we know we're the only thread in the critical section so we can safely
   update the global variables
3. `pthread_mutex_unlock` - unlock the mutex lock because we're out of the
   critical section and we want to let any other threads that are blocked to
   proceed. 

## Doing this in code

1.  Include the proper header
    
    ```c
    #include <pthread.h>
    ```

2.  Declare a global mutex 
    
    ```c
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    ```

4.  Before the code you'd like to protect starts, first wait to ensure you don't
    proceed till you have "locked" the mutex:
    
    ```c
    pthread_mutex_lock(&mutex);
    ```
    
    You'll want to make the critical section as small as possible to allow for
    maximal concurrency.

5.  After the code you'd like to protect finishes, "unlock" the mutex lock: 
    
    ```c
    pthread_mutex_unlock(&mutex);
    ```

6.  Once you're sure all the threads are finished using the lock, clear it out
    (this usually happens in main after some joins):
    
    ```c
    pthread_mutex_destroy(&mutex);
    ```


## Solve it!

Use the steps above to solve the problem in `add_a_lot.c`.


# Basic Mutex 2: RedBluePurple (50 points)

For a detailed description of the problem look at the comments in the
`red_blue_purple.c` file.


# Rubric

<table border="2" cellspacing="0" cellpadding="6" rules="groups" frame="hsides">


<colgroup>
<col  class="org-left" />

<col  class="org-left" />

<col  class="org-right" />
</colgroup>
<tbody>
<tr>
<td class="org-left">Thread Factoring</td>
<td class="org-left">creates threads and they run</td>
<td class="org-right">10</td>
</tr>


<tr>
<td class="org-left">Thread Factoring</td>
<td class="org-left">correctly divides work and displays well</td>
<td class="org-right">10</td>
</tr>

<tr>
<td class="org-left">Thread Sorting</td>
<td class="org-left">Loop and create n parallel threads to sort parts of the data</td>
<td class="org-right">10</td>
</tr>


<tr>
<td class="org-left">Thread Sorting</td>
<td class="org-left">Each thread uses timeofday to measure total execution time and prints index and execution time</td>
<td class="org-right">10</td>
</tr>


<tr>
<td class="org-left">Thread Sorting</td>
<td class="org-left">Computes and prints min max and mean times for each algorithm</td>
<td class="org-right">20</td>
</tr>

<tr>
<td class="org-left">Basic mutex 1</td>
<td class="org-left">Follows instructions and eliminates concurrency bug using mutex locks</td>
<td class="org-right">10</td>
</tr>


<tr>
<td class="org-left">Basic mutex 2</td>
<td class="org-left">Mutex locks used correctly (e.g. created, freed with proper functions)</td>
<td class="org-right">15</td>
</tr>


<tr>
<td class="org-left">Basic mutex 2</td>
<td class="org-left">All concurrency bugs eliminated</td>
<td class="org-right">15</td>
</tr>
</tbody>
</table>

