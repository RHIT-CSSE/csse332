---
title: Userspace Threads I
layout: post
date: Wed Jan 25 19:44:36 2023 
readtime: true
---

# Introduction

Let's say that you would like to write a multi-threaded application, but your
operating system doesn't support threads (i.e. It just runs whole processes,
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
get started. The starter code is contained under the `ust/` directory.

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
get started. The starter code is contained under the `ust/` directory.

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
luckily there are some handy Unix functions that do that kind of thing.

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

   > Don't forget to initialize the array of booleans itself.

2. When a thread yields, it is important to know what the index of that
   currently running thread is. We need that index value to find out which entry
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
parameters because we often want to start the same function in parallel multiple
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

Here is an example to illustrate this. This is a subtle memory corruption bug if
you pass your parameters to the threading function as follows:

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

## Implementation

Your job in this task is to implement `create_new_parameterized_thread`.  First
start by taking a look at the man page for `makecontext`. You will see that you
can pass an arbitrary number of parameters to the `makecontext` function (in
that way, it is similar to C's `printf` function. That is represented by the
`...` keyword in the function signature). You just have to make sure that the
third argument is the number of extra parameters you want to pass to
`makecontext`.

The code for this step otherwise will be exactly like `create_new_thread`.

A few minor notes and wrinkles:

- The function parameter to `makecontext` is still specified as `void(*)()`,
  that is a pointer to a function that take no parameters and returns nothing.
  That's because there is now way to say something like "a pointer to a function
  that takes an arbitrary number of parameters and returns nothing". Therefore,
  to make this work, you will have to cast our function parameter to that type.
  Something like the following:
  ```c
  void(*cast_ptr)() = (void(*)()) fun_ptr; ```

- It should irk you that `create_new_thread` and
  `create_new_parameterized_thread` have basically the same code with minor
  differences. Will we allow this code duplication? Hell no! It turns out that
  fixing this is easy though. We can just make `create_new_thread` call
  `create_new_parameterized_thread` with a null parameter. It might be good
  practice to take a moment and think why would this be a safe approach?
  Make sure you fix this slight issue as our graders do not duplicated code.

As usual, compile your `tests.bin` binary and run it using `./run_tests.bin 4`
and make sure you pass all the tests.

# Test 5: Removing the Need for `finish_thread`.

If you've experimented with writing your own test thread functions, you may have
noticed how super-bad news it is if you write a threaded function that doesn't
call `finish_thread` when it returns.  Your program instantly and without
errors terminates, and even judicious use of a debugger can't identify the
problem (because this is considered a "natural" exit, not an error).

If you haven't seen this, try running Test 5 without implementing any code so
you see what that looks like.

We could make the error more obvious, but rather than that, it would be better
if the thread function returning just called `finish_thread` implicitly. The way
to do this is to add a new helper function that `makecontext` calls (instead
of the actual thread function).  This function will take 2 parameters, the
actual thread function pointer and the void pointer parameter to pass it.

Then the helper function will call the actual thread function, and once it
returns, call `finish_thread`.  If we wanted to, we could also add some
initialization that occurs before the function call - not needed quite yet, but
it will be quite handy once we have preemption in a future assignment.

As usual, compile your `tests.bin` binary and run it using `./run_tests.bin 5`
and make sure you pass all the tests.

# Test 6: Freeing What We Have Malloced

Finally, we have to take care of freeing everything that we have malloced for
each thread once it finishes.  Your first instinct might be that the helper
function we implemented in the previous step would be a great place to call 
`free` to free the thread stack allocation.  **This is a terrible idea!**  (stop
for a second and see if you can figure out why it is a terrible idea without
reading my solution)

This is particularly bad news because most of the time this code will probably
work, because the stack remains in use for such a short time the OS will
probably not re-purpose its page.  But then 1 out of 1000 runs, you'll
spontaneously get a segmentation fault - good luck tracking that down.  Remember
that an appropriate amount of paranoia is what is necessary.

## Fixing the Issue

The `free` call we need cannot really happen while we are running the thread,
thus calling it from `finish_thread` or the helper function is a not a good
idea. Therefore, the only place we can actually call `free` is the scheduler.
Once a thread is finished, we need a way to tell our scheduler that that thread
is done and that we can free its stack. Your job in this step is to implement a
way to inform the scheduler of when it must free a thread's stack.

## Testing Your Code

To accurately test your implementation, we need to make that every byte of
memory that you malloc must be freed. One impressive tool that can help us test
that if `valgrind`. It is real easy to use.

First, make sure you have `valgrind` installed. If not, then you can install
using your favorite Linux package manager. Assuming you're on Ubuntu, that would
look something like `sudo apt install -y valgrind` or maybe using `snap`.

Second, you can run `valgrind` on our final set of test cases. As usual, compile
your code to generated the updated `tests.bin` binary and then run that binary
in `valgrind` as follows:

```shell
$ valgrin ./tests.bin 6
==17524== Memcheck, a memory error detector
==17524== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==17524== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==17524== Command: ./memtry 6
==17524== 
==17524== Warning: client switching stacks?  SP change: 0x1ffefffc78 --> 0x5202818
==17524==          to suppress, use: --max-stackframe=137336181856 or greater
==17524== Warning: client switching stacks?  SP change: 0x52027c8 --> 0x1ffefffc80
==17524==          to suppress, use: --max-stackframe=137336181944 or greater
==17524== Warning: client switching stacks?  SP change: 0x1ffefffc78 --> 0x5212858
==17524==          to suppress, use: --max-stackframe=137336116256 or greater
==17524==          further instances of this message will not be shown.
........

OK (8 tests)

==17524== 
==17524== HEAP SUMMARY:
==17524==     in use at exit: 0 bytes in 0 blocks
==17524==   total heap usage: 54 allocs, 54 frees, 2,238,125 bytes allocated
==17524== 
==17524== All heap blocks were freed -- no leaks are possible
==17524== 
==17524== For counts of detected and suppressed errors, rerun with: -v
==17524== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

If your code is correct, you should see something like the output above.  The
warning you see in there about switching stacks are okay since we are indeed
switching stacks.

### Possible Buggy Outcome: Memory Leaks

If your code does not free what it mallocs (i.e., you have memory leaks), then
your output might look something like the following:

```shell
valgrind ./run_tests.bin 6                                           
==17632== Memcheck, a memory error detector
==17632== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==17632== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==17632== Command: ./memtry 6
==17632== 
doing a malloc
doing a malloc
==17632== Warning: client switching stacks?  SP change: 0x1ffefffc78 --> 0x5202c58
==17632==          to suppress, use: --max-stackframe=137336180768 or greater
==17632== Warning: client switching stacks?  SP change: 0x5202c08 --> 0x1ffefffc80
==17632==          to suppress, use: --max-stackframe=137336180856 or greater
==17632== Warning: client switching stacks?  SP change: 0x1ffefffc78 --> 0x5212c98
==17632==          to suppress, use: --max-stackframe=137336115168 or greater
==17632==          further instances of this message will not be shown.
........

OK (8 tests)

==17632== 
==17632== HEAP SUMMARY:
==17632==     in use at exit: 2,228,224 bytes in 34 blocks
==17632==   total heap usage: 54 allocs, 20 frees, 2,238,125 bytes allocated
==17632== 
==17632== LEAK SUMMARY:
==17632==    definitely lost: 1,835,008 bytes in 28 blocks
==17632==    indirectly lost: 65,536 bytes in 1 blocks
==17632==      possibly lost: 131,072 bytes in 2 blocks
==17632==    still reachable: 196,608 bytes in 3 blocks
==17632==         suppressed: 0 bytes in 0 blocks
==17632== Rerun with --leak-check=full to see details of leaked memory
==17632== 
==17632== For counts of detected and suppressed errors, rerun with: -v
==17632== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

Notice that the output above is telling that we have leaked over 1 MB in our 8
tests, that is far from ideal!

### Possible Buggy Outcome: Illegal Memory Access

Another type of memory bug that might occur is that of an illegal access to a
(freed) memory page. In that case, you `valgrind` output might look something
like the following:

```shell
$ valgrind ./run_tests.bin 6
==17746== Memcheck, a memory error detector
==17746== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==17746== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==17746== Command: ./memtry 6
==17746== 
==17746== Warning: client switching stacks?  SP change: 0x1ffefffc78 --> 0x5202818
==17746==          to suppress, use: --max-stackframe=137336181856 or greater
==17746== Warning: client switching stacks?  SP change: 0x52027c8 --> 0x1ffefffc80
==17746==          to suppress, use: --max-stackframe=137336181944 or greater
==17746== Warning: client switching stacks?  SP change: 0x1ffefffc78 --> 0x5212858
==17746==          to suppress, use: --max-stackframe=137336116256 or greater
==17746==          further instances of this message will not be shown.
==17746== Invalid write of size 8
==17746==    at 0x4C2E10B: free (vg_replace_malloc.c:530)
==17746==    by 0x10901D: finish_thread (in /home/hewner/Private/play/threading/memtry)
==17746==    by 0x108D75: thread_run_helper (in /home/hewner/Private/play/threading/memtry)
==17746==    by 0x4E8307F: ??? (in /usr/lib/libc-2.26.so)
==17746==  Address 0x52027a8 is 65,400 bytes inside a block of size 65,536 free'd
==17746==    at 0x4C2E10B: free (vg_replace_malloc.c:530)
==17746==    by 0x10901D: finish_thread (in /home/hewner/Private/play/threading/memtry)
==17746==    by 0x108D75: thread_run_helper (in /home/hewner/Private/play/threading/memtry)
==17746==    by 0x4E8307F: ??? (in /usr/lib/libc-2.26.so)
==17746==  Block was alloc'd at
==17746==    at 0x4C2CEDF: malloc (vg_replace_malloc.c:299)
==17746==    by 0x108E52: create_new_parameterized_thread (in /home/hewner/Private/play/threading/memtry)
==17746==    by 0x108D95: create_new_thread (in /home/hewner/Private/play/threading/memtry)
==17746==    by 0x10960C: test_5 (in /home/hewner/Private/play/threading/memtry)
==17746==    by 0x109DFB: CuTestRun (in /home/hewner/Private/play/threading/memtry)
==17746==    by 0x10A53B: CuSuiteRun (in /home/hewner/Private/play/threading/memtry)
==17746==    by 0x109828: main (in /home/hewner/Private/play/threading/memtry)
==17746== 

***MANY MANY MORE ERRORS OMITTED HERE***

........

OK (8 tests)

==17746== 
==17746== HEAP SUMMARY:
==17746==     in use at exit: 0 bytes in 0 blocks
==17746==   total heap usage: 54 allocs, 54 frees, 2,238,125 bytes allocated
==17746== 
==17746== All heap blocks were freed -- no leaks are possible
==17746== 
==17746== For counts of detected and suppressed errors, rerun with: -v
==17746== ERROR SUMMARY: 136 errors from 36 contexts (suppressed: 0 from 0)
```

As you can see above, even though the tests seem to run fine, there were
actually plenty of illegal memory accesses here.

# Submitting Your Code

Submit all assignment source files (c files, h files) but not binaries
(example1, run_tests, .o files) via Gradescope.

## Submission Checklist

- [ ]  My code compiles and generates the right executables.
- [ ]  I ran all of the test cases and all the tests pass.
- [ ]  I ran `valgrind` on my code and no memory bugs were detected.
- [ ]  I submitted all my source files to Gradescope for grading.

# Grading Rubric

| Part                                                                                   | Point Value |
|:---------------------------------------------------------------------------------------|-------------|
| Test 1: Scheduling a single thread once                                                | 40          |
| Test 2(a): Scheduling 2 threads that each run the same code                            | 16          |
| Test 2(b): Scheduling 3 threads where 2 run the same code and 1 runs different code    | 12          |
| Test 2(interleave): Scheduling 2 threads that interleave their execution               | 12          |
| Test 3: Scheduling a thread that creates many quick threads                            | 40          |
| Test 4: Scheduling threads that each runs a function that takes a parameter            | 20          |
| Test 5: Scheduling threads that run functions that don't explicitly call finish_thread | 20          |
| Test 6: Running Test 5 with valgrind to look for memory leaks and other errors         | 40          |

