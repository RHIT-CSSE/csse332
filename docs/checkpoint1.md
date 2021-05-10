---
layout: post
title: Term Project (Milestone 3: Checkpoint I)
readtime: true
date: Fri May  7 11:49:55 2021
gh-repo: rhit-csse332/csse332-202130
gh-badge: [star,watch,follow]
---

# Table of contents

<!-- vim-markdown-toc GFM -->

* [Introduction and aim](#introduction-and-aim)
  * [Learning objectives](#learning-objectives)
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

<!-- vim-markdown-toc -->

# Introduction and aim

In this first checkpoint of milestone 3, we will modify our kernel module to
implement a First-In-First-Out round robin scheduler. In other words, the kernel
will support only one run queue, put registering processes on the queue in a
first-come-first-serve fasion, and then schedule threads from the queue in a
round robin fashion, with a pre-specified quantum of 5 seconds.

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

## Learning objectives

In this checkpoint, we will gain familiarity with the following concepts:
- __Kernel threads__: We will switch our code from using a work queue to using a
  kernel thread.  Kernel threads are much more versatile, yet a little more
  challenging.
- __Scheduling__: We will interface with the kernel scheduler and learn how do
  we swap in and out processes in the kernel. Remember, with great power comes
  great responsibility.
- __Round Robin Scheduling__: We will solidify our understanding of a
  round-robin scheduler by implementing it in the linux kernel.

# Freshmen: Kernel threads

Okay, we have made it to college and now things start to get real. We will have
our first interation with creating and managing kernel threads. In this step, we
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
following line after you have created your timer, but BEFORE you scheduler your
timer to go off. Delete the line that schedules your time, i.e., delete the line
that calls the function `mod_timer`.
```c
scheduler_thread = kthread_run(scheduler_fn, NULL, "csse332_scheduler_thread");
if (!scheduler_thread) {
  pr_err("Could not create kernel thread, abandoning project\n");
  /* replace this with your appropriate error handling code, or simply return */
  goto exit;
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
returns `true` if and only if when `kthread_stop(struct task_struct *)` is
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
old `mod_timer` function.

Now, the magic will happen with the call to `schedule()`. This
[function](https://elixir.bootlin.com/linux/latest/source/kernel/sched/core.c#L5145)
is part of the core operations of the Linux scheduler. It will remove the
current process (our kernel thread in this case) from the current CPU and call
the sceduler to scheduler another thread. Since we update the state of our
kernel thread to `TASK_INTERRUPTIBLE`, the scheduler will put our thread to
sleep.  Good night!

Next, once we wake up from our sleep (we'll see who does that later on), we will
simply print a small message. What do you think the state of the kernel thread
will be at this point?

After that, we will rinse and repeat. We set our state to `TASK_INTERRUPTIBLE`,
modify the timer and ask the scheduler to put to us to sleep, and keep doing
that until `kthread_should_stop()` returns `true`.

Now that we know what the kernel thread handler does, let's add our code from
the work queue in milestone 2 to the scheduler function, between the call to
`schedule()` and the last call to `set_current_state(...)`.

## Waking up the kernel thread
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


## Stopping the thread

Let's finish off by telling our kernel thread to stop when the module is
removed. To do so, in your exit function,  add the following line:
```c
kthread_stop(scheduler_thread);
```
This will signal to your kernel thread that it should stop executing and exit.
Now can you guess I asked you to keep the `mod_timer` call at the top of the
loop? Think about it!

## Testing
At this point, your code should behave in the same way as it did in milestone 2.
Test it out using the same techniques you used for milestone 2 and make sure
that the behavior is consistent.

Once you are confident that your code works, it is time to move on to sophomore
year! Exciting...

# Sophomore: First-In-First-Out Round Robin Scheduling

Now that we know how to use kernel threads, let's go ahead and start with our
implementation of a FIFO round robin scheduler. There are a few things that
we'd want to worry about, so let's go through them one by one.

## Logistics

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

### Testing

In order to help you test things out, I have added a user space library and a
few applications for you to use. You can find the source code for these
applications and its documentation
[here](https://github.com/rhit-csse332/userlib). Feel free to simply clone this
repository or to fork it if you would like to write your own test cases.


## Data structures

First, let's figure out our data structures.

### The queue

To implement a FIFO scheduler, we will definitely need a run queue to hold the
set of processes that we would like to run in the order that they come into our
system. Our linked list of processes can serve this purpose very well, however,
you can also create a separate list to represent the run queue; that is totally
up to you. In my implementation, I used the database to serve the dual purpose
of holding the list of registered processes as well as the run queue.

### The currently running process PCB

Since our scheduler will now take over the scheduling mechanism, it must then
remember which process it is currently running. We will assume that we will only
be running a single process at a time, even if we have several processors (or
CPU cores) available. We therefore have to keep track of the currently scheduled
process. To do so, add a global PCB variable by the name
`current_scheduled_process` (or any name you like) to your module, and
initialize it to `NULL` in your initialization function.

## Scheduling

The scheduling thread is the crux of this checkpoint and it is where all the fun
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

## Registration

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

## Yield

When a process yields, it should inform the scheduler that it is yielding and
call on the scheduler to schedule another process. To wake up the scheduling
thread, you can use the following code
```c
wake_up_process(scheduler_thread);
```

## Deregistration

Now this one is a little bit tricky, because we a process might write `W` to
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

# Expected output

I have recorded a short video for you to show what the expected output of
running the test code.

<iframe width="560" height="315" src="https://www.youtube.com/embed/aRq45kF2xsU" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

