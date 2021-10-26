---
layout: post
title: Term Project (Milestone 3)
readtime: true
date: Tue Oct 26 17:47:18 2021 
gh-repo: rhit-csse332/csse332-202130
gh-badge: [star,watch,follow]
---

# Table of contents

<!-- vim-markdown-toc GFM -->

* [Introduction and aim](#introduction-and-aim)
  * [Learning objectives](#learning-objectives)
  * [Plan of implementation](#plan-of-implementation)
* [The FIFO-RR Scheduler](#the-fifo-rr-scheduler)
  * [Freshmen: Kernel threads](#freshmen-kernel-threads)
    * [Waking up the kernel thread](#waking-up-the-kernel-thread)
    * [Stopping the thread](#stopping-the-thread)
    * [Testing](#testing)
  * [Sophomore: First-In-First-Out Round Robin Scheduling](#sophomore-first-in-first-out-round-robin-scheduling)
    * [Logistics](#logistics)
      * [Testing](#testing-1)
    * [Data structures](#data-structures)
      * [The queue](#the-queue)
      * [The currently running process PCB](#the-currently-running-process-pcb)
    * [Scheduling](#scheduling)
    * [Registration](#registration)
    * [Yield](#yield)
    * [Deregistration](#deregistration)
    * [Expected output](#expected-output)
    * [Testing](#testing-2)
* [Multi-Level Feedback Queue](#multi-level-feedback-queue)
  * [Requirements](#requirements)
  * [Submission](#submission)
  * [Implementation plan](#implementation-plan)
    * [Junior: Rules 1 and 2](#junior-rules-1-and-2)
      * [Data structures](#data-structures-1)
      * [Timers](#timers)
      * [Registration](#registration-1)
      * [Yielding](#yielding)
      * [Deregistration](#deregistration-1)
  * [The scheduler](#the-scheduler)
      * [Testing](#testing-3)
    * [Senior: Rule 5](#senior-rule-5)
      * [Testing](#testing-4)
    * [Graduation: Rules 3 and 4](#graduation-rules-3-and-4)
      * [Rule 3](#rule-3)
      * [Rule 4](#rule-4)

<!-- vim-markdown-toc -->

# Introduction and aim

In this milestone, we will finally put thing together and implement our
scheduler. You have the choice of implementing one of two schedulers: a
first-in-first-out (FIFO) round robin (RR) scheduler that is worth 30% of your
grade, or a Multi-Level-Feedback-Queue (MLFQ) scheduler that is worth 30% + 10%
bonus of your grade (yes that is 10%, so if you screwed up an exam, you can make
it up by implementing the MLFQ). 

## Learning objectives

- __solidify__ our understanding of CPU scheduling algorithms
- __design__ and __implement__ a FIFO-RR or an MLFQ scheduling algorithm
- __complete__ our module with scheduling parameters
- __test__ our scheduler using custom-designed test cases
- __write__ a documentation and user guide report for our module and test cases.

## Plan of implementation

If you are going for the MLFQ, you must first go through the FIFO scheduler as a
lot of the concepts that you will need will depend on things that we introduce
in the FIFO section. Make sure you understand that part real well before diving
into the MLFQ implementation. 

---

# The FIFO-RR Scheduler

In this first section of milestone 3, we will modify our kernel module to
implement a First-In-First-Out round robin scheduler. In other words, the kernel
will support only __one run queue__, put registering processes on the queue in a
first-come-first-serve fashion, and then schedule threads from the queue in a
round robin fashion, with a __prespecified__ quantum of 5 seconds.

To provide support for this functionality, we will need our processes to be
well-behaving (you can assume that) and we must provide support for the
following operations:
- `Registration`: We will need to slightly modify the semantics of our
  registration operation to now add the process to the run queue. It will start
  off in the ready state.
- `Deregistration`: We will need to slightly modify the semantics of our
  deregistration operation to now remove the process from the run queue.
- `Yield`: (NEW OPERATION) We will need to provide support for processes to
  express them voluntarily release the CPU on which they are running.

## Freshmen: Kernel threads

Okay, we have made it to college and now things start to get real. We will have
our first interaction with creating and managing kernel threads. In this step, we
will modify our code from milestone 2 to use a kernel thread rather than a work
queue. This will serve a dual puprose:
1. We will learn the API for kernel threads management (creation, killing, etc.)
2. Kernel threads have to be __manually scheduled by you__, i.e., if a kernel
   thread runs and you do not deschedule it, it will keep running forever.
   Therefore, this will serve as a good introduction to what is coming next in
   the Sophomore year.

Kernel threads are simply threads that run in kernel space. They are
represented, as any other process, using a `struct task_struct`. Once a kernel
thread is scheduled to run on a CPU, it will keep running there forever, unless
you specifically ask it not to. Luckily, the kernel allows us to interface with
the scheduler to schedule and deschedule our kernel thread.

First, let's create our thread. At the top of your file, add the declaration for
the kernel thread as follows
```c
struct task_struct *scheduler_thread;
```

Now, let's create an empty kernel thread handler function with the following
signature:
```c
int
scheduler_fn(void *arg)
{
  return 0;
}
```

Then, let's create the thread. In your initialization function, add the
following line after you have created your timer, but BEFORE you schedule your
timer to go off. Delete the line that schedules your timer, i.e., delete the
line that calls the function `mod_timer`.
```c
scheduler_thread = kthread_run(scheduler_fn, NULL, "csse332_scheduler_thread");
if (!scheduler_thread) {
  pr_err("Could not create kernel thread, abandoning project\n");
  /* add your appropriate error handling code, or simply return */
}
```
`kthread_run` will create AND run a kernel thread for us. In our case so far,
nothing will happen since our handler function is empty. The parameters to
`kthread_run` are the following:
- `scheduler_fn`: The thread handler function (much like the handler function in
  `pthreads`).
- `NULL`: This is the argument to pass to the thread, we will not need it for
  now.
- `"csse332_scheduler_thread"`: The name of our thread.

Okay, now comes the fun part. Let's write our thread handler. A kernel thread
handler function is generally structured as follows:
```c
while(!kthread_should_stop()) {
  /* do important work here, but watch out for the fact that you must deschedule yourself.
   * That's some tricky business going on ...
   */
}
```
The function `kthread_should_stop` (implemented
[here](https://elixir.bootlin.com/linux/v4.9/source/kernel/kthread.c#L79)) is a
function that tells your kernel thread to stop executing. `kthread_should_stop`
returns `true` if and only if `kthread_stop(struct task_struct *)` is
called on your kernel thread. You can use the combination of these two functions
to signal to your kernel thread that it is time to stop working.

So let's start putting things together. When your kernel thread runs for the
first time, we will require it to simply set a timer and go to sleep. To do so,
add the following code to your thread handler function:
```c
int
scheduler_fn(void *arg)
{
  pr_info("Kernel thread is born! Happy birthday to me...\n");

  set_current_state(TASK_INTERRUPTIBLE);
  while(!kthread_should_stop()) {
    /* set the timer to go off after 5 seconds */
    /* DO NOT MOVE THIS LINE AFTER THE SCHEDULE CALL! BAD THINGS WILL HAPPEN
     * Think about why, and if you can't figure it out, ask me during office hours!
     */
    mod_timer(&timer, jiffies + msecs_to_jiffies(5000));
    /* tell the scheduler we're ready to leave for now */
    schedule();

    /* print hello message and then go back to sleep, unless you should exit */
    pr_info("Hello from kernel thread =)\n");
    set_current_state(TASK_INTERRUPTIBLE);
  }
  return 0;
}
```

Wow that's a lot to unpack, let's do it one by one! First thing, the thread will
simply print a message telling us that it was born. Great!

Next, what's up with this line: `set_current_state(TASK_INTERRUPTIBLE);` huh?
Well, using this call, we will tell the kernel scheduler that our state will
change now from `TASK_RUNNING` to `TASK_INTERRUPTIBLE`, this means that we are
getting ready to go to sleep, but not there yet.  `TASK_INTERRUPTIBLE` is a
state in which a thread goes into sleep but it can be interrupted by software or
hardware interrupts.

Okay, next up, we will schedule the timer to go off in 5 seconds, using the same
old `mod_timer` function (or a combination of `expires` + `add_timer`).

Now, the magic will happen with the call to `schedule()`. This
[function](https://elixir.bootlin.com/linux/latest/source/kernel/sched/core.c#L5145)
is part of the core operations of the Linux scheduler. It will remove the
current process (our kernel thread in this case) from the current CPU and call
the scheduler to schedule another thread. Since we update the state of our
kernel thread to `TASK_INTERRUPTIBLE`, the scheduler will put our thread to
sleep.  Good night!

Next, once we wake up from our sleep (we'll see who does that later on), we will
simply print a small message. What do you think the state of the kernel thread
will be at this point?

After that, we will rinse and repeat. We set our state to `TASK_INTERRUPTIBLE`,
modify the timer and ask the scheduler to put to us to sleep, and keep doing
that until `kthread_should_stop()` returns `true`.

### Waking up the kernel thread

Okay so now we know how the kernel thread operates, but who wakes up the kernel
thread when it goes to sleep? Looking at our code, try to answer this question
before moving along in the tutorial.

So you guessed it right, it is the timer handler. All the timer handler has to
do is to wake up our scheduling thread (the kernel thread we created above) and
that's it. One line of code. To do so, add the following line of code to your
timer handler
```c
wake_up_process(scheduler_thread);
```
And that's it. Now, every time the timer expires, we will wake up our kernel
thread. Nifty!


### Stopping the thread

Let's finish off by telling our kernel thread to stop when the module is
removed. To do so, in your exit function,  add the following line:
```c
kthread_stop(scheduler_thread);
```
This will signal to your kernel thread that it should stop executing and exit.
Now can you guess I asked you to keep the `mod_timer` call at the top of the
loop? Think about it!

### Testing

At this point, your code should run the kernel thread __once__ every 5 seconds
and print the message `Hello from kernel thread =)` every time. Once you see
this behavior and it is stable, you can move on to the next stage.

Once you are confident that your code works, it is time to move on to sophomore
year! Exciting...

## Sophomore: First-In-First-Out Round Robin Scheduling

Now that we know how to use kernel threads, let's go ahead and start with our
implementation of a FIFO round robin scheduler. There are a few things that
we'd want to worry about, so let's go through them one by one.

### Logistics

We saw in the previous section that we can use `set_current_state` to change the
state of the `current` task structure. However, to implement a scheduler, we
will also need to change the state of other tasks, not just `current`. To help
you out, I have provided you with code to do so. Copy the code below into your
module source.

```c
static inline void
set_task_state(struct task_struct *tsk, long state)
{
	smp_store_mb(tsk->state, state);
}
```

#### Testing

In order to help you test things out, I have added a user space library and a
few applications for you to use. You can find the source code for these
applications and its documentation
[here](https://github.com/rhit-csse332/userlib). Feel free to simply clone this
repository or to fork it if you would like to write your own test cases.


### Data structures

First, let's figure out our data structures.

#### The queue

To implement a FIFO scheduler, we will definitely need a run queue to hold the
set of processes that we would like to run in the order that they come into our
system. Our linked list of processes can serve this purpose very well, however,
you can also create a separate list to represent the run queue; that is totally
up to you. In my implementation, I used the database to serve the dual purpose
of holding the list of registered processes as well as the run queue.

#### The currently running process PCB

Since our scheduler will now take over the scheduling mechanism, it must then
remember which process it is currently running. We will assume that we will only
be running a single process at a time, even if we have several processors (or
CPU cores) available. We therefore have to keep track of the currently scheduled
process. To do so, add a global PCB variable by the name
`current_scheduled_process` (or any name you like) to your module, and
initialize it to `NULL` in your initialization function.

### Scheduling

The scheduling thread is the crux of this project and it is where all the fun
stuff happens. Every time the scheduler thread wakes up, it needs to do the
following (in the following order):
1. Cancel any pending timers (since it might be waken up from a `yield` call).
1. If there is no currently a running process (first time we run, no processes
   to schedule, etc.), it will pick the next process to run from the run queue.
   Go to step 4.
2. If there were a process running (i.e., we scheduled a process before),
   then we must:
   - Put the currently running process to sleep (set its state to
       `TASK_INTERRUPTIBLE`). Do not call `schedule()` yet.
   - Pick the next process to run
3. If the run queue is empty, the scheduler will do nothing
3. Prepare the next process for running (update the run queue, etc.)
4. Set the kernel thread's state to `TASK_INTERRUPTIBLE` (as we did in the
   previous section)
5. Set a new timer for a new quantum
6. Call `schedule()`

__Hint__: Make sure to put the previously running process back on the run
queue.

__Hint__: The macro `list_first_entry` might cause a segmentation fault if
called on an empty linked list. Use the `list_empty` function (defined
[here](https://elixir.bootlin.com/linux/latest/source/include/linux/list.h#L280))
to check if the list is empty before using `list_first_entry`.

### Registration

Now let's take care of registering new processes. When a process registers, the
following must happen:
- Allocate and initialize a PCB for the process
- Add the PCB to the database
- (NEW) Add the process to the tail of the run queue (this might be resolved by
  the previous step if you are using the database as the run queue).
- (NEW) __Put the process to sleep__.

Since a process that has just registered does not have a turn in our round robin
scheduler yet, we will initially put the process to sleep. This will have the
undesired effect of having the first process to register into the system 
wait for one quantum in order to run, even if there are no other processes in
the system. But we will live with that.

To put a process to sleep, you can use the following code
```c
set_current_state(TASK_INTERRUPTIBLE);
schedule();
```

### Yield

When a process yields, it should inform the scheduler that it is yielding and
call on the scheduler to schedule another process. To wake up the scheduling
thread, you can use the following code
```c
wake_up_process(scheduler_thread);
```

### Deregistration

Now this one is a little bit tricky, because a process might write `W` to
deregister, but then be swapped out right before it has deregistered, so then
bad things will happen if you are not careful.

To avoid this problem, we will delete things from the deregistration handler and
then call on the scheduler to take care of the rest. Therefore, in the
deregistration handler, do the following:
1. If the process trying to deregister is the currently running process (use the
   global struct that we defined in the logistics section), then simply `kfree` the
   pointer and set `current_scheduled_process` to `NULL`.
2. If the process trying to deregister is not the currently running process
   (might happen due to unfortunate timing from our scheduler), then find the
   process in the database and the run queue, delete it from both, then free the
   pointer.

__HINT__: Make sure in all these steps to hold appropriate locks over the
database and run queue, otherwise, as we always say, bad things will happen!

### Expected output

I have recorded a short video for you to show what the expected output of
running the test code.

<iframe width="560" height="315" src="https://www.youtube.com/embed/aRq45kF2xsU" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>


### Testing

One of the challenges of this milestone is designing test cases for your system.
Read the code provided in the `userlib` repository and design your test cases so
that you test your scheduler. Add a description of your test case to your
project report and feel free to record a video showing the details of your
testing. 

---

# Multi-Level Feedback Queue


Recall [our
discussion](https://moodle.rose-hulman.edu/mod/page/view.php?id=3024301) of the
MLFQ scheduling algorithm from Session 14. In MLFQ, we will try to optimize for
both the _turnaround_ as well as the _response time_. The MLFQ scheduling
algorithm is composed of five rules:
1. If `Priority(A) > Priority(B)`, then A runs and B doesn't.
2. If `Priority(A) == Priority(B)`, then A and B run in round-robin fashion
   using the quantum length __of the given queue__.
3. When a process enters the system, it is placed at the highest priority level
   possible.
4. Once a job uses up its time allotment at a given level (regardless of how
   many times it has given up the CPU), its priority level is reduced (i.e., it
   moves down one queue).
5. After some time period `S`, move all the jobs in the system to the topmost
   queue.

An MLFQ scheduler is interesting for the following reason: It does not require
any prior knowledge about the nature of the processes that it is scheduling. It
will observe the behavior of these processes and then will adjust and adapt
accordingly. Therefore it can deliver on the performance of algorithms such as
Shortest-Job-First or Shortest-Time-to-Completion-First, as well as make similar
fairness guarantees as a Round Robin scheduler.

## Requirements

Here is a list of requirements that your implementation must satisfy:
- Your system must have __at least__ 3 queues.
- Each queue must have a different round robin quantum.
- Your scheduler must implement the above five rules correctly.
- Your system must allow for processes to register and deregister at any time,
  i.e., it must support processes accessing your data structures concurrently.

__WARNING: This a design-based project, i.e., I will give you the general
idea and some hints as to how to approach it, but it is up to you to make and
justify your design choices__.

## Submission

Please submit the following to the moodle dropbox by 5:00 pm on the last day of
finals:
- Your project source code (so far, we've been using `project.c`)
- Your makefile
- Your test files
- A project report (in PDF format) that contains the following:
  - An overview of your design (one paragraph)
  - A list of your design decisions, along with justification for each design
    decision (one paragraph per design decision)
  - A list of the challenges you faced, along with a description of how you
    overcame (or did not overcome) the challenge.
  - A description of your testing environment along with screenshots showing
    your test cases. I suggest your record a video of your test cases to
    explain them to me, but that is optional, screenshots will do.

If you were not able to complete the project, then please include a detailed
description of why you think that happened, what you think you could have done
differently, and list of the parts of the project that you think are working.
This will be crucial for me to give you partial credit.

## Implementation plan
Here is a suggested plan of implementation, however, feel free to follow
whichever plan that works best for you.

### Junior: Rules 1 and 2

To start putting things together, let's put our focus on Rules 1 and 2 first. We
will first expand our FIFO scheduler to support multiple queues instead of just 1.
Moreover, to test things out, we will add processes to the queues one at a time
(i.e., first process to register gets queue 1, second process gets queue 2,
etc.).

#### Data structures

To achieve our goal, let's first start by modifying our data structures. Instead
of a single linked list, we will now need a linked list (i.e., a queue) for each
of our run queues (recall, you must have at least three). In my implementation,
I found it easier to declare a structure for each queue, something along the
lines of:
```c
#define MAX_QUEUES 3

struct run_queue {
  /* The priority of this queue */
  int priority;
  /* The round robin quantum for this queue */
  int quantum;

  /* The head of the queue */
  struct list_head queue_head;
};

struct run_queue priority_queues[MAX_QUEUES];
```

Remember to initialize your list heads using `INIT_LIST_HEAD` and set the
default values in the `__init` function, otherwise, nasty things will happen.

__Note:__ You can still reuse your database as one of the priority queues,
however, I would advise against it. Feel free to keep the database or just get
rid of it all together.

#### Timers

A big difference between this checkpoint and the first one is the fact that now
we cannot rely on a single timer that always goes off periodically. Instead,
depending on which queue we are using, the timer will need to go off at
different intervals (recall, each queue must have its own quantum).

For example, if the quantum for queue 1 is 1 second, the quantum for queue 2 is
2 seconds, and the quantum for queue 3 is 3 seconds, then our timer will behave
differently depending the queue from which we are scheduling. If we are
scheduling from queue 1, then we need our timer to go off after 1 second. If we
are scheduling from queue 2, then we need our timer to go off after 2 seconds,
and so on.

In my implementation, instead of keeping a single global timer, I found it
easier to create a timer for each process that is being scheduled. Once I know
the process I need to schedule and the queue from which it was scheduled, I can
use `mod_timer` to appropriately set when the next timer will go off.

#### Registration

To implement and test rules 1 and 2, we will assume that the processes that get
registered get added to the queue in a FCFS basis. In other words, the first
process to register gets the queue with the highest priority, the second process
to register gets the queue with the second highest priority, and so on.

Next, I did the following. If there is no currently running process (i.e., this
is absolutely the first process to register -- the system was empty before
that), then I keep the process running and I schedule its timer to go off at the
corresponding quantum. If there is a currently running process, then I put the
registering process to sleep. That was my way of implementing my system, it is
up to you to design your approach to handling registration. Make sure to
document your decisions in your project report.

#### Yielding

The yield process does not change much from the FIFO RR scheduler, all you have
to do is to make sure that the process that is yielding is the currently
scheduled one (how would you do that?) then wake up the scheduler to take on the
work after that.

#### Deregistration

The process of deregistration is similar to the case we did in the first case.
If the deregistering process is the currently running one, then we need to do
the following:
1. Cancel the process's timer
2. Remove the process from its corresponding queue
3. Set the currently running process pointer to NULL (to avoid segfaults form
   the scheduler -- think about why).
4. Wake up the scheduler to take over.

Otherwise (i.e., if the deregistering process is not the currently running one
-- due to the weird scheduling issue with multiple CPUs), then we only need to
delete the process from its queue, and no need to wake up the scheduler. 

## The scheduler

The scheduler is where most of the work needs to happen. _The scheduler must
always maintain knowledge of the currently running process_. Note that the
scheduler may be called (or waken up) from three different places in the code:
1. A timer from a process
2. A process yielding
3. A process deregistering

To avoid issues with the scheduler being called from outside of a timer
interrupt, I suggest that you always cancel the timer of the currently running
process, if any. This will guarantee that the scheduler will only run once.

The first thing the scheduler must do is to pick a new process to run, if any.
To do so, you must scan all of the queues in order of priority, and select the
first available process to be run, at the highest priority level available. 

After finding the next process to run, the scheduler must put the currently
running process to sleep. 

Finally, the scheduler will wake up the next process to run, adjust the
currently running process pointer (change your local pointer, DO NOT CHANGE THE
`current` pointer, you will brick your machine if you do), and set the timer for
the process to trigger after exactly one quantum (note that the quantum will be
different depending on which queue does the process come from).

#### Testing

At this point, you should have an MLFQ system that satisfies rules 1 and 2 of
the MLFQ rules. 

Test your system by first inserting the module and running two instances of the
`userapp` from the `userlib` that I provided you with. The first one you run
should go into the higher priority queue, while the second one should go in the
queue following that one. The expected output is that the first process will run
to completion and then and only then will the second process run. 

Next, try to run three instances of `userapp` and make sure they the first one
runs and finishes first, then the second one, then the third. Here's a quick
tutorial video that shows you what you should expect when running your code.

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=7716083a-db37-4d11-abcf-ad2801527267&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>


### Senior: Rule 5

In this section, we will simply add support for rule 5, which states that every
`S` seconds, we will put everything into the top most queue. I chose a value of
50 seconds, but it is up to you to choose and justify the value you choose for
your module.

You can implement this step either using a kernel thread or using a work queue,
it is totally up to you. I chose to use another kernel thread in my
implementation. 

Here's how things go in this step, every `S` seconds, a timer will go off and
wake up a thread or add a work onto the work queue. Once that work executes (or
the thread wakes up), it will look at all the lower priority queue in order, and
move their corresponding entries to the topmost priority queue. That way, the
next time that the scheduler runs, it will for sure run those processes that
were originally in the lower priority queues. 

__Note__ If you maintain references in each process to which queue it belongs
to, don't forget to update those after you make the changes!

#### Testing

To test support for rule 5, set a shorter time period for the bumping mechanism
to happen (I tested it with a 1 second timer). Then start two `userapp`s from
the given test code. You should first see the first process running on its own,
then a short while later, the second process will join it in the execution, and
they will start running in a round-robin fashion. Here's a quick video to
illustrate this process:

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=81a34d71-aa73-406a-9bc3-ad29017b2dde&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

### Graduation: Rules 3 and 4

Let's finalize our system with the addition of support for rules 3 and 4. 

#### Rule 3

Adding support for rule 3 is pretty much straightforward. Change your code for
the registration function to add the new processes to the topmost queue. Until
we implement rule 4, we are back now to a round robin scheduler.

#### Rule 4

Implementing rule 4 requires some more infrastructure. We need to track how long
have the process been running in each queue, and if that time interval goes over
the quantum of the queue it belongs to, downgrade the process to the next level
queue. 

Here is an example to illustrate this: let's assume process A registers into our
system. It therefore starts at the topmost queue with a quantum of 1 second.
Assume also that the quantum for the second queue is 2 seconds, and the quantum
for the third queue is 3 seconds. Here's what going to happen to process A:
```
A registers --> A runs for 1 second
                   --> A gets demoted to queue 2 --> A runs for 2 seconds
                                                      --> A gets demoted to queue 3 --> A runs for 3 seconds
```

Assume now that process B registers, but process B yields execution every 400
ms. Here's what's going to happen to B:
```
B registers --> B runs for 400 ms --> B yields --> B runs for 400 ms --> B yields --> B runs for 400 ms --> B yields --> B gets demoted to queue 2 --> ...
```

You will need to add information to your `csse332_info` structure to support
remembering for how long has this process been running in the current queue, and
then every time you schedule out a process, you check if it should be demoted. 


