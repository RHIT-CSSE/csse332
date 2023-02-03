---
title: (Challenge) Hybrid Threads
layout: post
date: Thu Feb  2 2023 
readtime: true
---

# Introduction

In this assignment we're going to build a simple hybrid threads system.  The
system will be "hybrid" in the sense that it will include both userspace threads
and kernelspace threads (i.e. pthreads).  Our goal is to allow a large number of
low-cost user threads, but to also get the ability to run threads on different
CPUs at the same time.

The basic approach we will take is to create multiple schedulers (i.e. parent
threads) that each run in their own kernelspace thread.  These threads will
independently walk across the same global `ucontext_t` threads array, and select
userspace threads to run.  To make this work we will need to use mutex locks to
prevent concurrency bugs.

# Learning Objectives

At then end of this lab, you should be able to:

# Getting the Source Code

For this lab, you will be using the native Linux virtual machine (or baremetal
machine if you have one) and not the xv6 operation system. Please note that this
lab might behave slightly differently if you are running it on Windows or
MacOs; therefore, we highly recommend that you stick to using a Linux machine,
either natively or via WSL2.

To obtain the starting code for this lab, navigate to the top level directory of
your __csse332 class repository__ and `cd` into the `labs/lab11` directory as
follows:
```shell
$ cd /path/to/csse332/git/repository/
$ git pull
$ cd labs/lab11
```

# Step 0: Moving from Basic to Hybrid

This assignment will start from your for userspace threads I. While a real
hybrid system would also want to be preemptive, it's complex to combine threads
and signals correctly in Unix (though it can be done). Also, knowing when our
userspace threads yield will simplify our tests.

You will start from your code for the basic threads assignment; all the
threading functions will be the same. There will be a new version of
`schedule_threads` with the following signature:

```c
void schedule_hybrid_threads(int num_pthreads);
```

We will also allow for a `MAX_THREADS` count for 100 instead of just 5. To begin 
your implementation, it might be useful to start as follows:

1. Copy `basic_threads.c` from your userspace threads I assignment to the
   directory containing the code for this assignment.

2. Rename the file to `hybrid_threads.c`.

3. Modify the file to include `hybrid_threads.h` instead of `basic_threads.h`.

4. Change the value of `MAX_THREADS` to 100.

# Step 1: Adding More Thread States

The first change we need to make won't even require pthreads, but it is
necessary before we can move forward with this assignment. In you followed the
instructions in the userspace threads I assignment, you are probably keeping
track of the thread states in an array of booleans (I called mine `is_valid`)
where each entry determines if the corresponding thread contains a valid, thus
runnable, thread for the scheduler to switch to.

In this assignment, we will have multiple threads executing at once, so we need
to be a bit more careful. For example, a user thread might be valid, but it is
currently being run by another kernel thread (perhaps on another CPU core), so
it is not safe at all to run it. Therefore we need to differentiate between
tasks that are valid (i.e., runnable) and those are actually running. One way to
achieve that is to change the`is_valid` array from an array of booleans into an
area of `unsigned char` (i.e., 8 bit unsigned integers). Something that looks
like the following:

```c
#define   INVALID    0
#define   PAUSED     1
#define   RUNNING    2
#define   FINISHED   3
#define   CREATING   4

unsigned char thread_state[MAX_THREADS];
```

You can also define the states as an `enum`, but I will leave that to you if you
are interested in doing that.

Now notice that instead of just two possible states, we have several. `PAUSED`
is the state of a thread that is runnable (ready to be run) but is not currently
running. `FINISHED` is the state of a thread that has been completed but has not
yet had its memory freed. `CREATING` is for when we are in the process of
creating a thread by claiming its spot in the `thread_state` array, but the
thread has not been fully created yet with things like `makecontext`.

Feel free to use any states that you deem fit, you don't have to stick to the
ones defined above. Yet, the ones above serve as a good start for our
discussion. However, it is better to decide on your thread states now and switch
to using them before introducing the complexity of pthreads.

A small wrinkle now would be how to detect that the scheduler has completed
since we have a bunch of new states. The general rule we will follow is that the
scheduler will stop and return once each and every thread is in the `INVALID`
state.

To help make the transition to the new set of states, we have included a test
file called `us1tests.c`, which is basically a copy of the userthreads threads I
tests, simply revised to use the new scheduler, but always passes `1` as the
number of pthreads.

To test things out, build the test file and run it as follows:

```shell
$ make us1tests
$ ./us1tests
```

Revise your code to use the new states and then make sure these tests still pas
before you proceed.

# Step 2: Basic Parallelism

Let's start this step by taking a look at `standalone1.c` and try to build and
run it using the provided `Makefile`. At this stage, you should not see
interleaving happening between "ONE" and "TWO" because there are no `yield`
calls. We will fix this in this step by running each userspace thread in its own
pthread.

The basic operation is pretty simple, take your current scheduler code and put
it into a function suitable to be invoked from `pthread_create` (i.e., something
that returns a `void*` pointer and accepts a single `void*` argument, but for
all our purposes here, both values will be `NULL`). I called my function
`schedule_threads_pthread` by whatever you like this fine.

Next, make a new `schedule_hybrid_threads` function which simply calls
`pthread_create` once for each thread we want, and then joins them all to
complete.

There are two problems that will arise in this case:

1. Concurrency issues, we will deal with those in the next step.

2. Thread-specific global variables.

Let's deal with the second one first. Recall that in the userspace threads I
assignment, we use a global `int` to keep track of the index of the currently
running userspace thread (I called mine `current_thread_index`). In our old
system, there was at most one scheduler and exactly one running thread, so using
a single global variable makes sense.

In our current setup, we have one scheduler per pthread, and one running thread
per pthread. Clearly a single global variable is not enough. So when a userspace
thread yields, how can it know what index it ought to return to?

This example is a great use case for _thread-local storage_, that is, data that
is global within one thread but not in other threads (you might notice that one
of the disadvantages of userspace threads is that this feature cannot exist).

To create such a thread-local variable, declare the variable thread-local like
the following:

```c
__thread int current_thread_index;
```

> Note that it is __two underscores__ before `thread`.

If you declare `current_thread_index` in such a manner, when one of your
scheduler threads sets `current_thread_index`, it is only set within that thread
and not in the others. Similarly, when a `yield` function reads
`current_thread_index`, it would be reading the value local to the thread it is
running in.

Think carefully about which variables you want to make thread-local. You don't
want your thread array or state array local. In that case, pthreads won't be
pulling their userspace threads from a common queue, thus causing all sorts of
issues. In my implementation, the only other variable I had to make thread-local
was the parent's `ucontext_t` but your implementation could be slightly
different.

If you handle the thead-local storage correctly, your `standalone1` will start
interleaving. Do keep a careful eye on how `standalone1` terminates: it should
print a final "done" at the end. If it is not, then you've probably messed
something up.

# Step 3: Concurrency Bugs

Now that we've got the basics going, it is time to think about concurrency
issues. The most straightforward issue is that schedulers might, in parallel,
select the same thread for running, and thus that thread would run twice in two
different kernel threads. We definitely need to prevent this from happening as
it can cause all sorts of problems. One solution you might think of is to use a
single lock that the scheduler must grab before running the userspace thread.
However, we cannot have some mutex lock that is locked for the duration of a
userspace thread's execution - that will totally remove the parallelism that
pthreads give us.

To address this issue, we need to meet the following requirements:

1. When a scheduler claims a paused userspace thread to run, it should _own_
   that thread until it reenters the paused state again or finishes and it
   marked invalid. No other schedulers should be able to claim it.

2. When a userspace thread yields, the scheduler pthread should _disown_ it into
   the paused state for other schedulers to access it. A scheduler should check
   for other runnable userspace thread before re-running the same one (i.e., the
   schedulers should run in round robin fashion).

3. If there are unowned userspace threads to run and unoccupied schedulers, it
   should always be possible for a scheduler to claim an unowned thread (i.e.,
   an unoccupied scheduler should not have to wait for some other userspace
   thread to yield).

4. If at least one thread is running, __no schedulers should be allowed to
   exit__. We need to ensure that because that running thread can spawn new
   thread of its own, thus needing the schedulers to be ready. If there are not
   enough userspace threads for the number of pthreads, it is ok if there is
   some amount of busy-waiting occurring to look for new threads to run.
   However, if you'd like to try it, the busy-waiting can  be fixed with only a
   slightly more complex algorithm.

The remaining three requirements are similar to the 4 above but they have to
with thread creation:

5. When a thread creation claims an invalid slot to reuse for a newly created
   userspace thread, it should _own_ that slot until it is marked as paused. No
   other creation should be able to claim that same spot.

6. If there are invalid spots to be used, thread creation should not be blocked.

7. You can assume that the number of threads desired is always less than the
   `MAX_THREADS` of the system (i.e., you do not need to worry about blocking
   thread creation due to lack of thread space).

To simplify your implementation and build it step by step, you can make the
non-creation tests pass first before you try the last 3 creation-related rules.
To test your system without creation problems (i.e., all threads are created
upfront so there's no concurrency problems there), you can do the following:

```shell
$ make basic_para_tests
$ ./basic_para_tests
```

Once you pass the above tests, you can add your creation rules solution and test
creation using:

```shell
$ make create_para_tests
$ ./create_para_tests
```

There is a simple solution that implements all these rules, and it does not
require a large number of mutex locks. Here are a couple of hints:

1. Of the 4 thread states, 2 correspond to thread slots that are unowned
   (`INVALID`, `PAUSED`) while for the rest, a thread in that state must already
   by owned. Contention only arises when multiple threads attempt to transition
   something from an unowned state to an owned state.

2. It is not usually a good idea to make a decision to do some action unless you
   have the resources to implement your decision. For example, don't pick a
   thread you want to run and then acquire a resource that lets you mark a
   thread as running. BY the time you acquire that resource, the situation may
   have changed (i.e., the thread might be running or finished). Instead,
   acquire the resource first, then find some appropriate thread to run (maybe
   freeing it if you can't).

# Submitting Your Code

Submit all assignment source files (c files, h files) but not binaries via
Gradescope.

## Submission Checklist

- [ ]  My code compiles and generates the right executables.
- [ ]  I ran all of the test cases and all the tests pass.
- [ ]  I submitted all my source files to Gradescope for grading.

# Rubric

|                       | Points   | Notes                                                                                               |
| :-------------------- | -------- | --------------------------------------------------------------------------------------------------- |
| us1tests              | 15       | we'll look at your code and make sure you really made it use multiple states                        |
| standalone1           | 15       | should be interleaved                                                                               |
| `basic_para_tests`    | 40       | Note that we will also examine your code to ensure you don't have concurrency bugs the tests miss   |
| `create_para_tests`   | 10       | You may fail 1 of the test cases occasionally even with correct solution, which is okay.        |


