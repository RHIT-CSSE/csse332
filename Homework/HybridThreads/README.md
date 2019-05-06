---
title: Hybrid Threads
layout: togit
---

# Hybrid Threads

In this assignment we're going to build a simple hybrid threads
system.  The system will be "hybrid" in the sense that it will include
both userspace threads and kernelspace threads (i.e. pthreads).  Our
goal is to allow a large number of low-cost user threads, but to also
get the ability to run threads on different CPUs at the same time.

The basic approach we will take is create multiple schedulers
(i.e. parent threads) that each run in their own kernelspace thread.
These threads will independently walk across the same global
ucontext\_t threads array, and select userspace threads to run.  To
make this work we will need to use semaphores to prevent concurrency
bugs.

# Moving from Basic to Hybrid

This assignment will start from your code for Userspace
Threading 1. While a real hybrid system would also want to be
preemptive, it's complex to combine threads and signals correctly in
UNIX (though it can be done).  Also, knowing when our userspace
threads yield will simplify our tests.

You will start from your code for the basic threads assignment; all
the threading functions will be the same.  There will be a new version
of schedule\_threads

    void schedule_hybrid_threads(int num_pthreads);

We will also allow for a MAX_THREADS count of 100 rather than 5.

To begin:

Take your solution to the previous lab assignment and

1.  Copy basic\_threads.c to the directory containing the initial code

for this assignment.

1.  Rename it to hybrid\_threads.c
2.  Change it to include hybrid\_threads.h rather than basic\_threads.h
3.  Modify schedule\_threads to match the new signature
4.  Change MAX_THREADS to 100

# Adding more thread states

The first change we need to make won't even require pthreads, but it's
necessary before we can move forward.  In your existing system, you
probably have an array of booleans (I called mine is\_valid) that
determines if each userspace thread slot contains a runnable thread
for the scheduler to switch to.

Now that we can have multiple threads executing at once, we need to be
more careful.  A thread might be valid, but currently be being run by
another process so it isn't safe to run it.  In my hybrid thread
solution changed my is\_valid array into an array called thread\_state
that looks like this:

    #define INVALID 0
    #define PAUSED 1
    #define RUNNING 2
    #define FINISHED 3
    #define CREATING 4
    
    char thread_state[MAX_THREADS];

Paused is the state of a thread that is runnable but not currently
running.  Finished is the state of a thread that has been completed
but not yet had its memory freed (consider the concurrency bug you
might get if you instead just marked it as INVALID).  Creating is for
when a create thread process has claimed a spot, but it is not yet
fully filled in with things like makecontext.

You aren't required to use my states but it's at least a good starting
point.  Moreover, it's best to switch your system over to the new
states and test before we introduce the complexity of pthreads.

A small issue is how to detect completion now that we have a bunch of
new states.  The rule we'll want is that the scheduler can return once
every thread is in the INVALID state.

We've included a test file called us1tests.c which is basically a copy
of the userspace threading 1 tests, simply revised to use the new
schedule threads but always pass "1" as the number of pthreads.

Build/run them like this:

    $ make us1tests
    $ ./us1tests

Revise your code to use the new states and then make sure these tests
still pass before your proceed.

# Basic parallelism

Take a look at standalone1.c and build/run it.  You should not see
interleaving between "ONE" and "TWO" because there are no yields.  We
will fix that by running each thread in its own pthread.  The basic
operation is pretty simple - take your current scheduler code and put
it into a function suitable to invoking from pthread\_create (I called
mine schedule\_threads\_pthread but whatever you like is fine).
schedule\_threads\_pthread will take an return a void pointer, but for
what we're doing those values will always be null.

Then make a new schedule\_hybrid\_threads which simply calls
pthread\_create once for each thread we want, then joins them to
complete.

There are two problems:
1. Concurrency issues - we'll deal with that in the next step
2. Thread specific globals

Consider the int you use to store the current thread (I called mine
current\_thread\_index).  In the old system, there was one scheduler
and one running thread so that global make sense.  In the new system,
there is one scheduler per pthread and one running thread per pthread.
So when a thread yields how does it know what index it ought to return
to?

This is a great use for thread local storage - that is, data that is
global within one thread but not others (you may recall one of the
disadvantages of userspace threads was that this feature can't exist).

Make a variable thread local like this:

    __thread int current_thread_index;

Now when one of your scheduler threads sets current\_thread\_index (or
whatever you called it) it only sets it within their thread.  When a
yield function reads current\_thread\_index it reads the value for the
thread it is running in.

Think carefully about which variables you want to make thread local.
You don't want your thread array or state array local - then threads
won't pull their userspace threads from a common queue.  The only
other variable I had to make thread local was my parent ucontext\_t
but your implementation could be slightly different.

If you handle the thread local storage correctly, you standalone1 will
probably start interleaving.  Do keep a careful eye on how standalone1
terminates.  It should print a final "done" at the end - if it's
not you've messed up something.

# Concurrency Bugs

Now that we've got the basics going, it's time to think about
concurrency issues.  The most straightforward issue is that schedulers
might in parallel select the same thread for running and then that
thread would run twice.  We need to prevent this from happening.  But
on the other hand, we can't have some semaphore that's locked for the
duration of a userspace thread's execution...that removes the
parallelism that pthreads gives us.

Here's the basic requirements:

1. When a scheduler claims a paused userspace thread to run, it should
   "own" that thread until it reenters the paused state again or
   finishes and is marked as invalid. No other schedulers should be
   able to claim it.
2. When a userspace thread yields, the scheduler pthread should disown
   it into the paused state for other schedulers to access it. A
   scheduler should check for other runnable user space threads before
   rerunning the same one (i.e. the schedulers should round robin).
3. If there are unowned userspace threads to run and unoccupied
   schedulers, it should always be possible for the scheduler to claim
   the thread (i.e. the unoccupied scheduler shouldn't have to wait
   for some other user space thread to yield).
4. If at least one thread is running, no schedulers should exit
   (because that thread could spawn more threads in the future).  If
   there are not enough userspace threads for the number of pthreads,
   it's OK if there's some amount of busy-waiting occurring looking
   for new threads to run (BUT if you'd like to try the busy waiting
   can be fixed with a only slightly more complex algorithm).

These last three are similar but have to do with creation:

5. When a thread creation claims an invalid slot to reuse for a newly
   created thread, it should "own" that slot until it is marked as
   paused.  No other creation should be able to claim it.
6. If there are invalid slots to be used, thread creation should not
   be blocked (similar to 3 above).
7. You can assume that the number of threads desired is always less
   than the MAX_THREADs of the system (i.e. you don't need to worry
   about blocking thread creation due to lack of thread space). (BUT
   if you're curious this also has a pretty simple fix)

You can make the non-creation tests pass first before you try and
implement the last 3 rules.

There is a simple solution that implements all these rules...and does
not require a large number of semaphores.  I won't reveal the solution
but here's two hints:

1.  Of the 4 thread states, 2 correspond to thread slots that are
    unowned (INVALID, PAUSED) while for the rest a thread in that
    state must already be owned.  Contention only arises when multiple
    threads attempt to transition something from an unowned state to
    an owned state.
2.  It's not usually a good idea to make a decision to do some action
    unless you have the resources to implement your decision.  For
    example, don't pick a thread you want to run and then acquire a
    resource that lets you mark a thread as running.  By the time you
    acquire the resource, the situation may have changed (i.e. the
    thread might be running or finished).  Instead acquire the
    resource first, then find some appropriate thread to run (maybe
    freeing if you can't).

To test your system without creation (i.e. all threads are created
upfront so there's no concurrency problem there):

    $ make basic_para_tests
    $ ./basic_para_tests

To test creation:
    
    $ make create_para_tests
    $ ./create_para_tests

# Submitting

Submit hybrid\_threads.c and hybrid\_threads.h.

# Rubric

|                     | Points | Notes                                                                                             |
|:--------------------|--------|---------------------------------------------------------------------------------------------------|
| us1tests            | 15     | we'll look at your code and make sure you really made it use multiple states                      |
| standalone1         | 15     | should be interleaved                                                                             |
| basic\_para\_tests  | 40     | Note that we will also examine your code to ensure you don't have concurrency bugs the tests miss |
| create\_para\_tests | 10     | should be pretty straightforward if you did the last step right                                   |


