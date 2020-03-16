---
title: Userspace Threads with Preemption
layout: togit
---

# Userspace Threads with Preemption
----

## Table of Contents

1.  [Introduction](#orgfc71f24)
    1.  [How](#orgb1cd0a7)
2.  [Read this](#org6e742af)
3.  [An Example](#org2b33a7e)
4.  [Moving from Basic to Preempt](#orgf36f474)
5.  [What to do](#orgc14b38f)
    1.  [Standalone 1: The Basics](#org58e380d)
    2.  [Standalone 1: Problems](#org2420b5c)
    3.  [Standalone 1: Segmentation Faults Solution](#org577edb5)
    4.  [Standalone 1: What's the deal with the lockup?](#orgea10935)
    5.  [Standalone 1: Weird printing](#orgfacc0f2)
    6.  [Standalone 2: Yields, Creates, & Finishes](#org8626e2b)
    7.  [Test cases](#org452bfcb)
6.  [Conclusion](#org4c9602e)
6.  [Rubric](#fpp)



<a id="orgfc71f24"></a>

# Introduction

In the last lab assignment we built a userspace threading system, but it
required the programmer to manually yield.  In some cases, this might
be OK - in particular it lets us ensure that a thread always gives up
control at some sensible time, but it does require more work for the
programmer using the thread system.  Also, it makes our system
vulnerable to one particular thread grabbing control and refusing to
yield (either because of a bug like an infinite loop, or because it
makes some sort of library function call that just takes a really long
time to run): this kind of thing breaks the illusion of multiple
threads running in parallel.

So in this assignment, we'll make our system preemptive.  That is,
we'll force our threads to give up control even if they don't yield.


<a id="orgb1cd0a7"></a>

## How

To have preemption we need a special facility: a way to force a
currently running thread off the CPU and run some other code (that we
will write to cause a thread switch).  Without this feature,
preemption is impossible.

In an OS, this is handled using interrupts (in particular, timer
interrupts), which are directly supported by your CPU.  That's great
for an OS, but we can't install interrupt handlers in non-kernel code.
So the OS must provide this facility to userspace processes if it's
going to exist.  

In LINUX, we can use signals.  Signals are normally used to catch
unexpected behaviors in code (e.g. user presses Control-C, some
arithmetic operation divides by zero).  These things would normally
crash our programs altogether, but we can install handlers that
trigger when these behaviors occur.  When a signal happens, our normal
code is suspended and the signal handler runs (one might think of it
as an involuntary function call).  That's the feature we want -
running arbitrary code without the say-so of the currently running
function.

There is a special alarm signal SIGALRM that we can use to signal
after a specific amount of time.  In our case, we'll cause a SIGALRM
to yield to the scheduler allowing other code to run.  But as usual
there will be complications..


<a id="org6e742af"></a>

# Read this

This code will require an understanding of signals and signal masks.
Read [this introduction](https://rhit-csse.github.io/csse332/Homework/UserspaceThreadsLab2/sigintro.html).

This was originally from http://titania.ctie.monash.edu.au/signals/


<a id="org2b33a7e"></a>

# An Example

To compile and run this code, use gcc

    gcc preempt_example.c -o preempt_example
    ./preempt_example

You should see the parent and the child switch between each other,
even without a yield in the child code.

You'll want to understand this code completely before continuing on.


<a id="orgf36f474"></a>

# Moving from Basic to Preempt

You will start from your code for the basic threads assignment; 
all the threading functions will be the same.  The only difference
will be a new version of schedule\_threads

    void schedule_threads_with_preempt(int usecs);

When threads are scheduled with this function they will be preempted
every usecs microseconds (if they don't yield or finish naturally).

To begin:

Take your solution to the previous lab assignment and

1.  Copy basic\_threads.c to the directory containing the initial code

for this assignment.

1.  Rename it to preempt\_threads.c
2.  Change it to include preempt\_threads.h rather than basic\_threads.h
3.  Modify schedule\_threads to match the new signature


<a id="orgc14b38f"></a>

# What to do


<a id="org58e380d"></a>

## Standalone 1: The Basics

We'll start with a straightforward application rather than a test.
Take a look at what the code does.

You should be able to compile it like this:

    gcc standalone1.c preempt_threads.c -o standalone1

If you run it without modifying schedule\_threads, you should see
function 1 run and finish, then function 2 should start and finish.
This is the expected behavior because there are no yields.

Using the preempt example as a starting point, modify the code in
preempt\_threads.c to use sigalrm to call yield.  If you do it correctly you
should see the 111/222 working calls interleave.


<a id="org2420b5c"></a>

## Standalone 1: Problems

Adjust the wait time of standalone1 to something low (e.g. 5 usecs
although different systems may require tweaking) and then run it a
bunch of times.

You may see one of a couple problems, though it won't be consistent:

1.  Segmentation faults
2.  Program freezes
3.  The "done" for each thread printing more than once

These issues are caused by 3 unique problems.  Let's look at the first
one first.  This is caused by the alarm signal firing at inopportune
times.  Either:

1.  When executing a swapcontext function
2.  In the parent thread, which shouldn't expect sudden yields and so
    tends to have odd behavior

While setting the wait time low exacerbates these problems, there's
nothing preventing this from happening with long waits except the fact
that switches are less frequent, so the errors are harder to find.

So we need to fix this.


<a id="org577edb5"></a>

## Standalone 1: Segmentation Faults Solution

We need to disable (i.e. mask) the alarm signal from occurring when it
is unsafe.  To do that, we'll use code like this - see the appropriate
man pages for details:

    sigset_t mask;
    sigemptyset (&mask);
    sigaddset (&mask, SIGALRM);
    if(sigprocmask(SIG_BLOCK, &mask, NULL) < 0) {
        perror ("sigprocmask");
    }

You may want to put this in a function.

NOTE: be sure you block SIGALRM as in the example above.  Lots of
other examples block SIGINT (because that tends to be easier to play
around with) but blocking SIGINT won't have any effect on the
segmentation fault problem.

Similarly, we need to re-enable the alarm when it is safe.  You should
write the appropriate code for that.

Whenever you're dealing with preemption issues, you should try to
codify the rules carefully and make sure you haven't missed any edge
cases.  Here are the rules to use:

1.  Alarm signals should be masked when in the scheduler.  This way you
    never have to deal with exciting edges cases involving the
    scheduler yielding to itself.
2.  Alarm signals should be masked when adjusting shared thread
    structures (e.g. the threads array, the datastructure that keeps 
    track of which threads are valid, the currently running thread, etc.). 
    Preemption at these times usually carries the risk of putting these 
    structures in an inconsistent state.
3.  Alarm signals should be masked before switching contexts
    (e.g. swapcontext).  An alarm preempting this code while
    in-progress usually causes all sorts of exciting seg-faults.
4.  One minor non-obvious wrinkle: when you create a thread context
    object, you probably want alarms to be masked.  This is true even
    if you know the creation can't be preempted because you haven't
    started the alarm signal for example.  The reason for this is a new
    context has a different masking state from its parent, but it's
    initialized to the same state as the parent.  But if you switch to
    a context that does not have alarms masked, the switch can be
    preempted in-progress (after we've switched to the new masking
    state, but before the switch fully completes).  So by always
    creating threads in a masked state, we ensure that newly created
    threads finish starting before alarms can happen.  Of course, we
    must then unmask before actually running the thread function.
    
    Note that this also means the call to getcontext must be in
    create\_new\_thread and not initialize\_basic\_threads.
    getcontext is where the alarm mask state is set, so that's the
    line that most especially must be done in a alarm disabled
    context.  In theory you could get away with just masking in
    initialize\_basic\_threads too but that still allows context state
    to "leak" between threads that share the same slot.
5.  Alarm signals should NOT be masked when running ordinary code in
    the threads.
6.  You should NOT mask/unmask alarms in your alarm signal handler.
    This may seem non-obvious because of #2 & #3.
    
    The reason is twofold: 
    
    a. A signal handler automatically masks signal of it's own type.
    So there is no need to mask alarms in the alarm signal handler.
    However, if you then unmask alarms in the signal handler, you'll
    override this default behavior which is not desirable.
    b. Adjusting alarms mask in a signal handler changes the behavior
    of the signal handler, but not the code returned to after the
    signal handler.  So unmasking alarms in the signal handler will not
    mean alarms are unmasked in the code returned to.  Luckily, that
    code keeps its mask from before the handler was called, which is
    that alarms are unmasked.

7.  At the end of schedule\_threads\_with\_preempt, you should reenable
    alarm signals.  But before you do this, ensure that you don't
    accidentally call your alarm handler because of a pending alarm.
    Doing this will deregister your alarm handler:
    
        signal(SIGALRM, SIG_IGN);  //ignore the alarm signal
        
    You probably want to also ensure you don't have a scheduled alarm
    that hasn't fired.  calling alarm with a parameter of 0 seconds
    should disable any pending alarm (check the alarm man page for
    details).

Modify your code to abide by these rules and you should see the
signals in Standalone 1 go away.


<a id="orgea10935"></a>

## Standalone 1: What's the deal with the lockup?

If you notice a lockup (i.e. program stops outputting and never
finishes) that's happening VERY rarely, it may be caused by printing.
Turns out there's an internal "lock" that functions like printf
acquire before they can write to the console.  This is surely designed
for multithreaded code - that is, to prevent weirdness when multiple
threads print at once.  Before the printf happens, the function
"acquires" the I/O lock.  While it's acquired, if any other thread
tries to print, it has to wait for the first thread to release the
lock.  Under normal circumstances the first thread would finish
printing and "free" the lock.

If it surprises you that something designed to prevent threading
issues actually causes a seemingly major problem, don't be.  That is
usual, which is why most programmers are very paranoid about using
things like locks without being sure they don't have dangerous edge
cases.

So how does this cause your program to lockup?

1.  One of the threads acquires the lock and prints
2.  Then the alarm goes off, dropping into the handler which switches
    the context to the scheduler
3.  The scheduler tries to print a status message, but before the
    message can be printed it must first acquire the lock.  So it
    waits.
4.  Unfortunately because it is waiting, it can't actually switch to
    another thread, meaning that thread from #1 will never run again.
    The lock can never be freed.

Ok, how can we solve this?

There are a couple solutions, none of them super satisfactory:
    
    1.  Have all thread code mask alarms before every print, then reenable
        it afterword
    2.  Write your own version of printf that does #1, and then make your
        call to that version of printf rather than the regular one (this is
        moderately annoying to do with a function like printf that takes a
        variable number of arguments)
    3.  Never do any printing in your scheduler
    
    Of these, #3 is probably what I would do.  But this won't turn out to
    matter because of a completely different issue (Weird Printing,
    below).

In this lab (as opposed to real life), I reccommend you don't worry
about this bug and print normally so that you can debug other problems
easily.

If you have a problem with lockups that occurs frequently (like more
than once out of 20 runs or so) it probably is a regular infinite loop
bug that you need to fix not caused by printing.

<a id="orgfacc0f2"></a>

## Standalone 1: Weird printing

This is caused by a major limitation in our threading system.

So the userspace threading library we implemented before is actually a
relatively realistic example. Using SIGALRM to do preemption is not ideal,
really, because of something called async-signal-safety.

For a variety of reasons, it's often not safe to call a particular
function (say printf) while it is already running. For example, printf
uses some static data so if an in-progress call to printf is preempted,
and then the preempting code calls printf itself, this overwrites the
static data.  So when we return to the original call to printf, it
behaves strangely. ("strangely" sounds pretty benign, but bear in mind
this could just as easily segmentation fault or erase files on your
harddrive)

This is usually only a problem in signal handlers, which can preempt
arbitrary code and run at unexpected times.  For these handlers, there
is a list of functions that are considered "async-signal-safe"
(i.e. they can be called in a signal handler because even if they are
already running, they can be called again).  The list is here
<https://docs.oracle.com/cd/E19455-01/806-5257/gen-26/index.html> but
the important thing to know about it is that this is a short list of
functions.  Normally, folks recommend you do as little as possible in
signal handlers, basically set a global that other parts of your
program read and process.

In our signal handlers though, we actually switch the thread and then
run it.  Meaning any function in our thread might be preempted, and
then that same function might well run in a different thread.  So we
are limited to only asyc-signal-safe functions in our threads (or, if
we wanted to we could turn off preemption before we call unsafe
functions).  This would probably not be a realistic requirement for a
LINUX threading system.

For our purposes though, it will be perfectly fine except we'll need
to use the async safe function write, rather than printf (or if you do
use printf for debugging or something, realize it may have occasional
problems).

Here's what using write to print looks like (note you must specify the
size of the string you're writing):

    write(STDOUT_FILENO, "222 working\n",12); 

If you switch your printf calls to use write, this problem should go
away (also the locking problem, because async-signal-safety pretty
much requires you don't acquire locks).  Although if you look at the
man page for write, you'll realize that for 100% correctness, you really
should be checking the return of write and depending on that, maybe 
re-run it. We won't require that, but just realize that making code 
correctly async-signal-safe is a tricky business.


<a id="org8626e2b"></a>

## Standalone 2: Yields, Creates, & Finishes

The second standalone application has threads that call create\_new\_thread, 
yield, and finish\_thread.  In theory, if your solution works perfectly for 
Standalone 1, these should all continue to work.  In practice, you may
discover some bugs.

One common bug has to do with yielding.  So most folks, when initially
building a solution, call the function yield from their signal
handler.  This makes sense, but once we need to think about masking an
issue arises.

  1. If yield is called from a signal handler, then it should NOT mask
     alarms because you cannot safely unmask alarms from within a
     signal handler (see rule 6 above)
  2. If yield is called from user code, it should mask alarms because
     otherwise we will transition between threads with alarming
     unmasked.
     
Because it is not consistent, we need 2 different yield functions -
one for calling from the handler and one for calling from user code.

Note that this code uses write to do all it's printing (for the
reasons discussed above).  Be aware that you should not mix write and
printfs for debugging, because they will not print chronologically
(i.e. some printfs will print after some writes, even though the call
to printf occured before the writes).


<a id="org452bfcb"></a>

## Test cases

To compile the test cases

    gcc preempt_tests.c preempt_threads.c CuTest.c -o preempt_tests

If your standalone #1 and #2 work, there shouldn't be much you need to
do to get the test cases to pass.

However, note that some test cases can infinite loop if your functions
are not implemented correctly, in particular tests 5-7.  If you are
having infinite loop problems on the tests, add some prints to
preempt_test.c so you know what tests you are entering and take a look
at the tests themselves to figure out what's up.

<a id="org4c9602e"></a>

# Conclusion

Submit your assignment in the usual way.

<a id="foo"></a>

# Rubric

| Part         | Points |
|:-------------|--------|
| Standalone 1 | 33     |
| Standalone 2 | 33     |
| Test cases   | 34     |

Note that we give partial credit if your code occasionally has issues
but you addressed the particular problem areas that the lab warns you
about (particularly the issues noted in Standalone 1: Segmentation
Faults Solution).
