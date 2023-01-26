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

You can look at the code in `basic_threads_in_use_example.c` for an idea about
how our threading library will eventually look like.

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

# The Idea

Recall that what program is currently running in the CPU is entirely determined
by a few CPU registers (program counter, current stack pointer, and a few
others).  We usually adjust these registers, saving and restoring their state,
to allow things like function calls to happen.  That's not the only way we can
use them though!  We can also save them, then switch them for entirely different
values - essentially switching our program into a whole new state with a whole
new stack.  Then at some later time, we can restore the old values and from our
code's perspective it will seem like they never left.  If we switch between
these two codebases fast enough, the two **threads** of execution will appear to
be running simultaneously.

## Why would we want this?

Although the threads will appear to be happening simultaneously, actually they
are only running one at a time on one CPU.  Given that, we won't get any
performance increase by using our threads to solve an algorithm in parallel.
But there are many more uses for threads than just writing parallel algorithms.

The most common use is event-based programming.  Imagine you want some code to
run when the mouse is clicked, while at the same time doing some tricky
calculation.  Without threads, your calculation code is going to be polluted in
various places by checking the mouse.  Or imagine data is periodically coming in
from some source (say a network download) and at the same time other aspects of
your program are running.  You want the download data to be written to a file
(say) but that's not something the rest of you application should be constantly
checking and doing.  Even if the OS doesn't support threads, with userspace
threads we can easily handle all of this.

Another benefit is that userspace threads tend to be quite a bit more
lightweight than kernel or OS-managed threads.  So if you want to write code
with a LOT of parallelism, usually using a kernel thread for each one uses too
much memory and is too slow.  The parallel languages Erlang, Elixir, and Go for
example, make use of a mix of userspace and kernel threads to allow the creation
of way more threads than could normally be supported.  Using userspace threads
also gives you complete control over scheduling the threads, which means you can
implement optimization that the OS can't know about.

## How to Achieve That

So what we need to start with is a way to save the current execution thread
running on the CPU into memory, switch to a different execution thread, and then
later restore that old process.  We could write all that in assembly, but
luckily there are some handy unix functions that do that kind of thing.

| Function      | Description                                                                                 |
| :------------ | ------------------------------------------------------------------------------------------- |
| `getcontext`  | gets the current state into a `ucontext` variable, which you can then modify                |
| `makecontext` | modifies a context, making it include a function call of your choice                        |
| `swapcontext` | stores the current context in a variable, and switches execution to the given new context   |

Of course that's not very specific, to get more information, you can always look
for the documentation on these functions using the `man` utility, something like
the following:

```shell
man getcontext
```

## Starting with an Example

Take a look at `example1.c`, it provides a good example on how to start using
the `getcontext` and `makecontext` and `swapcontext` functions.

You can compile this code using

```shell
$ make example1.bin
```

When this program runs, you should see the execution switching between the
parent and the child threads. Something like:

```shell
$ ./example1.bin
Creating child thread
Switching to child thread
child thread running 1 of 10
doing parent stuff and then switching back to child
child thread running 2 of 10
doing parent stuff and then switching back to child
child thread running 3 of 10
doing parent stuff and then switching back to child
child thread running 4 of 10
doing parent stuff and then switching back to child
child thread running 5 of 10
doing parent stuff and then switching back to child
child thread running 6 of 10
doing parent stuff and then switching back to child
child thread running 7 of 10
doing parent stuff and then switching back to child
child thread running 8 of 10
doing parent stuff and then switching back to child
child thread running 9 of 10
doing parent stuff and then switching back to child
child thread running 10 of 10
doing parent stuff and then switching back to child
Child thread exiting
Child done
Child thread returned and stack freed
```

I strongly recommend that you take the time to understand this code completely
before moving on to the next step.

## What you Have to Do

Your job in this lab is to turn this basic approach in `example1.c` into a
threading library. You would want to implement all of the function outlined in
`basic_threads.h`. We have provided you with detailed description about each
function in `basic_threads.c`.

### Running the Test Cases

The files that you will be editing define a library, and thus have no `main`
function. However, we have provided you with a bunch of test cases that you can
run depending on which step you are in this lab.

To compile the test cases, you can use `make tests.bin` and then run the tests
for steps 1 and 2 (for example) using:

```shell
$ ./tests.bin 2
```

# Test 1: Just One Thread

In this first test, we will only consider having a single thread. This pretty
much revolve around making the code that is in `example1.c` become part of our
library. It should only be a matter of just copying and pasting the various
parts of `example1.c` into the appropriate functions of `basic_threads.c` with
slight modifications. Note that to do this, you must really spend some time
understanding what the different parts of `example1.c` are doing.

For now, feel free to add the `parent` and `child` variables from `example1.c`
as globals. In test 2, we will replace those with references a threads array,
but let's just get through test 1 first.

To run the test, simply compile and run the `tests.bin` binary and run it with
argument 1 as follows: `./tests.bin 1`.

# Test 2: A Bunch of Threads

Make sure you have test 1 passing before you move on to this part.

The basic idea we have here is that we would like to have an array of
`ucontext_t` variables. As new threads get created, we will slot them into that
array, in the next available slot. After that, the `schedule_threads()` function
will switch between all the active threads in that array.

A few things to pay attention to:

1. Just looking at the threads array by itself, it's going to be impossible to
   tell if a particular entry has a valid context or just garbage data that
   hasn't been initialized yet. To address this issue, you will probably want an
   array of booleans that indicate if a particular entry in the array contains a
   valid context or not.

   > Don't forget to initalize the array of booleans itself.

2. When a thread yields, it is important to know what the index of that
   currenlty running thread is. We need that index value to find out which entry
   in the array the yielding thread occupied so that we can replace the correct
   context when swapping between threads. An easy way to keep track of this is
   via using a global index variable.

When you are ready, compile the `tests.bin` binary and run it for step 2 using
`./tests.bin 2`. Get all the tests passing before moving on to step 3.

# Test 3: Reclaiming a Finished Thread's Slot

Depending on how you implemented your code so far, this test case may work for
you out of the box. But here is the gist. There are two main ideas that we want
to address here:

1. An active thread should be able to create new threads into the system.
2. When a particular thread finishes, its corresponding entry in the threads
   array becomes available for reuse by future threads. Therefore, although our
   current implementation can have at most 5 active threads at a time, we can
   actually support much more threads over the lifetime of the programs as
   threads finish and get replaced by new ones.

Modify your code so you can reclaim the entry of a thread that has completed
execution. Once you are ready, compile the `tests.bin` binary and run it on the
third set of tests using `./tests.bin 3`.

Note that so far, we are still not worrying about freeing what we malloced for
each thread, we only care about reclaiming the finished thread's spot in the
threads array.

# Test 4: Thread Parameters

Up until now, all the functions that we passing as threading functions take no
parameters. This is very unusual; most of the time thread functions need
paramters because we often want to start the same function in parallel multiple
times with different parameters (as we have done multiple times when using
thread functions in `pthreads`).

Since we need to support thread functions that accept different types and number
of arguments, our best bet is to rely on C void pointers, just like we do when
using `pthreads`. If you need a refresher, take a look at the test cases for a
quick reminder on using void pointers as thread function arguments.

## Caveat

Remember the case we mentioned in class about being careful how to pass
parameters to threads. We must make sure that the memory location we are passing
as a parameter to a thread function is still going to be allocated at the time
the data is used.

Here is an exampe to illustrate this. This is a subtle memory corruption bug if
you pass your parameters to the threaeding function as follows:

```c
void runs_as_thread()
{
  int value;
  create_new_parameterized_thread(other_function, &value);
  finish_thread();
}
```

Programming in C is all about cultivating an appropriate amount of paranoia!

To solve this problem, we need to make sure that `value` always exists as long
as the other created threads is alive. One solution would be to use something
like a thread join function, similar to `pthread_join()`.

```c
void runs_as_thread()
{
  int value;
  thread_id id = create_new_parameterized_thread(other_function, &value);
  join_thread(id);
  finish_thread();
}
```

For simplicity, we are not gonna go through the trouble of implementing
something like this in this lab.

