---
title: Userspace Threads
layout: togit
---

# Userspace Threads
----

## Table of Contents

1.  [Introduction](#orgaaadb60)
    1.  [The Idea](#org5e7ea37)
    2.  [Why would we want this?](#org81db187)
2.  [How](#orgbd7272e)
3.  [An Example](#orgee594ef)
4.  [What we want](#org6d43de9)
5.  [Compiling and running tests](#orga50c516)
6.  [What to do](#org46abf10)
    1.  [Test 1: Just one thread](#org8dd306b)
    2.  [Test 2: A Bunch of Threads](#orgb87c837)
    3.  [Test 3: Reclaiming finished threads's slots](#orga88a057)
    4.  [Test 4: Thread parameters](#orgba95b39)
        1.  [Parameters?](#org0338c6a)
        2.  [How to do this](#orgba68775)
    5.  [Test 5: Removing the need for finish\_thread](#org97f505c)
    6.  [Test 6: Freeing what we have malloced](#org1cda0e7)
        1.  [Why it's a terrible idea](#orgc225273)
        2.  [How to fix it](#org03037c3)
        3.  [Testing this](#orgae81ea6)
7.  [Conclusion](#org58afea7)



<a id="orgaaadb60"></a>

# Introduction

Let's say that you would like to write a multi-threaded application,
but your operating system doesn't support threads (i.e. it just runs
whole processes, which can't have sub-parts that are scheduled
separately).  Are you doomed to a single threaded program - no!  You
can implement your own user-space (as opposed to kernel-space)
threads.  The process you'll use is very similar to the way the OS
itself handles threads.


<a id="org5e7ea37"></a>

## The Idea

Recall that what program is currently running in the CPU is entirely
determined by a few CPU registers (program counter, current stack
pointer, and a few others).  We usually adjust these registers, saving
and restoring their state, to allow things like function calls to
happen.  That's not the only way we can use them though!  We can also
save them, then switch them for entirely different values -
essentially switching our program into a whole new state with a whole
new stack.  Then at some later time, we can restore the old values and
from our code's perspective it will seem like they never left.  If we
switch between these two codebases fast enough, the two **threads** of
execution will appear to be running simultaneously.


<a id="org81db187"></a>

## Why would we want this?

Although the threads will appear to be happening simultaneously,
actually they are only running one at a time on one CPU.  Given that,
we won't get any performance increase by using our threads to solve an
algorithm in parallel.  But there are many more uses for threads than
just writing parallel algorithms.

The most common use is event-based programming.  Imagine you want some
code to run when the mouse is clicked, while at the same time doing
some tricky calculation.  Without threads, your calculation code is
going to polluted in various places by checking the mouse.  Or imagine
data is periodically coming in from some source (say a network
download) and the same time other aspects of your program in running.
You want the download data to written to a file (say) but that's not
something the rest of you application should be constantly checking
and doing.  Even if the OS doesn't support threads, with userspace
threads we can easily handle all of this.

Another benefit is userspace threads tend to be quite a bit more
lightweight than kernel managed threads.  So if you want to write code
with a LOT of parallelism, usually using a kernel thread for each one
uses too much memory and is too slow.  The parallel languages Erlang,
Elixir, and Go for example, make use of a mix of userspace and kernel
threads to allow the creation of way more threads than could normally
be supported.  Using userspace threads also gives you complete control
over scheduling the threads, which means you can implement
optimization that the OS can't know about.


<a id="orgbd7272e"></a>

# How

So what we need to start with is a way to save the current execution
thread running on the CPU into memory, switch to a different execution
thread, and then later restore that old process.  We could write all
that in assembly, but luckily there are some handy unix functions that
do that kind of thing.

|-------------|-------------------------------------------------------------------------------------------|
| getcontext  | gets the current state into a ucontext variable, which you can then modify                |
| makecontext | modifies a context, making it include a function call of your choice                      |
| swapcontext | stores the current context in a variable, and switches execution to the given new context |


Of course that's not very specific, so to get more information use the
man command line tool.

    man getcontext


<a id="orgee594ef"></a>

# An Example

[example1.c](example1.c)

To compile and run this code, use gcc

    gcc example1.c -o example1
    ./example1

You should see the parent and the child switch between each other.

You'll want to understand this code completely before continuing on.


<a id="org6d43de9"></a>

# What we want

You must implement all the functions outlined in
[basic_threads.h](basic_threads.h).  For detailed descriptions of what
each function does, take a look at [basic_threads.c](basic_threads.c).
But take heart, the various test cases below will walk you through it.


<a id="orga50c516"></a>

# Compiling and running tests

The code you're writing is a library and doesn't have a main.

Compile this code like this to build the test application.

    gcc tests.c CuTest.c basic_threads.c -o run_tests

If you pass the test application an integer, it will only run tests
less than or equal the given test step (see "What to do" for the test
steps).

For example, this will run test steps 1 & 2:

    ./run_tests 2


<a id="org46abf10"></a>

# What to do

This code is arranged as a series of tests that step you through the
required functionality one stage at a time.

As you start each test set, take look at what you're trying to make
pass.


<a id="org8dd306b"></a>

## Test 1: Just one thread

This should be a matter of just copying and pasting the various parts
of [<example1.c>] into the appropriate functions of
[<basic_threads.c>] with a few modifications.  Feel free to add the
parent and child variables from example1 for the time being - in Test
2 we'll replace them with references to the threads array but let's
not get overly complicated.


<a id="orgb87c837"></a>

## Test 2: A Bunch of Threads

This is a bigger change so get Test 1 passing before you start.

The basic idea is that you're going to have an array of ucontext\_t
variables and as new threads get created you'll slot them into the
array.  Then your schedule\_threads() will switch between all the
threads in that array.

A few wrinkles:

1.  Just looking at the threads array by itself, it's going to be
    impossible to tell if a particular position has a valid context or
    just garbage data from the array's creation.  You'll probably want
    an array of booleans that indicate if a particular slot is filled
    with a valid context.
    
    Of course, that array of booleans must be initialized itself!

2.  When you yield, it's important to know what the index of the
    currently running thread is so that you can replace the correct
    context in the threads array when you swap.  That's usually stored
    in a global.


<a id="orga88a057"></a>

## Test 3: Reclaiming finished threads's slots

This test case may work automatically for you, depending on how you
implemented your Test 2.  

There are basically 2 ideas here:

1.  One thread should be able to create new threads.
2.  When a particular thread completes, its slot in the threads array
    becomes available again and is reused.  Thus although we might be
    only able to support 5 threads at the same time, we can have more
    threads over the lifetime of the program.

Note that we're still not going to worry about freeing what we
malloced (yet) just the slot in the threads array.


<a id="orgba95b39"></a>

## Test 4: Thread parameters


<a id="org0338c6a"></a>

### Parameters?

Right now all the functions we're threading take no parameters.  This
is very unusual - most of the time thread functions need parameters
because we often want to start the same function in parallel multiple
times with different parameters (e.g. pull the data from several
datafiles in the same way, but each datafile happen in its own
thread).  But because of C's type checking, this is diffcult because
each of these functions would have a different function pointer type.
We don't want several versions of create new thread one of which takes
integer parameters:

    void create_new_thread(void (*fun_ptr)(int)) {

and one which takes two strings

    void create_new_thread(void (*fun_ptr)(char*, char*)) {

etc.

The solution to this problem will be C void pointers (look them up if
you don't recall what they are).

All our parameterized thread functions will take one parameter, a void
pointer that can point to any kind of data.  In cases where we'd like
multiple parameters, we'll pass a void pointer to a struct.  Then the
function can cast that pointer to whatever pointer kind it needs.

Take a look at the test cases to see how this is used.

**Note:**

When passing parameters to threads like this, we are going to have to
sure that the memory location we are passing is actually going to
still be allocated at the time the data is used.

For example, this is a subtle memory corruption bug (if you don't see
it, get some help so you do understand):

    void runs_as_thread()
    {
        int value;
        create_new_parameterized_thread(other_function, &value);
        finish_thread();
    }

Programming in C is all about cultivating an appropriate amount of
paranoia.

To solve this problem, the correct thing to do would be to implement
join.

    void runs_as_thread()
    {
        int value;
        thread_id result = create_new_parameterized_thread(other_function, &value);
        join_thread(result);
        finish_thread();
    }

that would be totally doable, but we're not going to go through the
trouble in this assignment.


<a id="orgba68775"></a>

### How to do this

Take a look at the manpage for makecontext.  You'll see that you can
pass an arbitrary number of parameters to the makecontext function (in
that way, it is similar to C's printf).  Just be sure the 3rd argument
is the number of extra parameters you want to pass (1 in this case).

The code otherwise will be exactly like create\_new\_thread.

A few minor wrinkles:

1.  The function parameter to makecontext is still specified as "void
    (\*)()" (that is, a pointer to a function that takes no parameters
    and returns nothing).  That's because there's no way to say "a
    pointer to a function that takes an arbitrary number of parameters
    and returns nothing".  So to make this work we'll have to cast our
    function parameter to that type:
    
        void(*cast_ptr)() = (void(*)()) fun_ptr;

2.  It should irk you that create\_new\_thread and
    create\_new\_parameterized\_thread have basically the same code.  Will
    we allow this code duplication? Hell no!  Turns out fixing this is
    easy though - we can just make create\_new\_thread call
    create\_new\_parameterized\_thread with a null parameter.  Think for a
    second: why is this safe? 
    
    Be sure you do this - as always, we will deduct points from your
    assignment for egregiously duplicated code.


<a id="org97f505c"></a>

## Test 5: Removing the need for finish\_thread

If you've experimented with writing your own test thread functions,
you may have noticed how super-bad news it is if you write a threaded
function that doesn't call finish\_thread when it returns.  Your
program instantly and errorlessly terminates, and even judicious use
of a debugger can't identify the problem (because this is considered a
"natural" exit, not an error).

If you haven't seen this, try running Test 5 without implementing any
code so you see what that looks like.

We could make the error more obvious, but rather than that it would be
better if the thread function returning just called finish\_thread
implicitly.  The way to do this is adding a new helper function that
makecontext calls (instead of the actual thread function).  This
function will take 2 parameters, the actual thread function pointer
and the void pointer parameter to pass it.

Then the helper function will call the actual thread function, and
once it returns, call finish\_thread.  If we wanted to, we could also
add some initialization that occurs before the function call - not
needed quite yet, but it will be quite handy once we have preemption
in a future assignment.


<a id="org1cda0e7"></a>

## Test 6: Freeing what we have malloced

Your first instinct might be that that helper function we implemented
in the previous step would be a great place to call free to free the
thread stack allocation.  This is a terrible idea!  (stop for a second
and see if you can figure out why it is a terrible idea without
reading my solution)


<a id="orgc225273"></a>

### Why it's a terrible idea

The memory we have malloced is the stack of the thread.  The stack of
the thread that is currently running your helper function.  If we free
it, any following code (i.e. that code that executes the context
switch to the scheduler) will be running in a freed stack.

This is particularly bad news because most of the time this code will
probably work, because the stack remains in use for such a short time
the OS will probably not repurpose its page.  But then 1 out of 1000
runs, you'll spontaneously get a segmentation fault - good luck
tracking that down.

Remember - an appropriate amount of paranoia is what is necessary.


<a id="org03037c3"></a>

### How to fix it

The free cannot happen while we are running the thread.  So the
scheduler must be the place to execute the free.  We need to signal
scheduler that this is what it should do (maybe by marking the thread
invalid, or some sort of magic global you setup).


<a id="orgae81ea6"></a>

### Testing this

We can't write an ordinary test for this but what we can do is use
valgrind (a C memory safety checker).  It's pretty easy to use, here's
how to run it (if this doesn't work you may have to install valgrind):

    valgrind ./run_tests 6

1.  What a successful output looks like

           valgrind ./run_tests 6                                    
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
    
    Warning indicating we're switching stacks is find - because we are
    switching stacks in this code.

2.  What a memory leak looks like

    If you aren't freeing what you malloced, it should look something like this
    
           valgrind ./memtry 6                                           
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
    
    Leaked over a megabyte in 8 tests&#x2026;not ideal.

3.  What an access of illegal (freed) memory looks like

    In this example I unsafely free memory in my finish thread function.
    
           valgrind ./memtry 6                                           
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
    
    As you can see, although the tests seem to run fine I've actually made
    many illegal accesses here.


<a id="org58afea7"></a>

# Conclusion

Submit all assignment source files (c files, h files) but not binaries
(example1, run_tests, .o files) via Moodle.
