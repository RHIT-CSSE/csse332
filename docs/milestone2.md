---
layout: post
title: Term Project (Milestone 2)
readtime: true
date: Tue Apr 13 12:19:58 2021
gh-repo: rhit-csse332/csse332-202130
gh-badge: [star,watch,follow]
---

# Table of contents
<!-- vim-markdown-toc GFM -->

* [Introduction and aim](#introduction-and-aim)
  * [Learning objectives](#learning-objectives)
  * [Logistics](#logistics)
* [Grade-school: Linked lists and process information](#grade-school-linked-lists-and-process-information)
  * [Example: A linked list](#example-a-linked-list)
  * [What to do now](#what-to-do-now)
* [Middle School: Maintain process information](#middle-school-maintain-process-information)
  * [Reading the database](#reading-the-database)
  * [Writing: Register and De-registering](#writing-register-and-de-registering)
    * [Registering a process](#registering-a-process)
    * [De-registering a process](#de-registering-a-process)
* [High School: Timers, Work queues, and synchronization](#high-school-timers-work-queues-and-synchronization)
  * [Time in the Linux kernel](#time-in-the-linux-kernel)
  * [Using timers to update process information](#using-timers-to-update-process-information)
  * [Deferring work with work queues](#deferring-work-with-work-queues)
    * [Implementation tasks](#implementation-tasks)
    * [Important note](#important-note)
  * [Synchronization](#synchronization)
* [Graduation: Killing misbehaving processes and module parameters](#graduation-killing-misbehaving-processes-and-module-parameters)
* [Milestone Evaluation](#milestone-evaluation)
* [Rubric](#rubric)

<!-- vim-markdown-toc -->

# Introduction and aim
So we have now written our first kernel module. What a thrill! Now let's start
doing something useful with it. In this milestone, we will start doing something
useful that will allow us to finally perform our module scheduling in
milestone 3. This milestone builds on the steps that we built in milestone 1, so
make sure you have that one running. If you do not, please contact me as soon as
possible so that I can bring you up to speed.

In this milestone, we will design and implement a process monitoring module.
This module will allow processes to register with it and de-register from it.
The module will monitor the process's behavior every 5 seconds, updating its
internal imitation process control block (PCB). The module will allow users to
see the status of all process that have registered with it by reading the
status using the `procfs` pseudo file system (much like we did in milestone 1).
Finally, the module will terminate any process that exceeds its allowed time
limit of execution. In other words, the module will keep an eye on processes and
how long they have been executing, and then kill those processes that exceed a
certain predefined time limit. We will also adjust our module to accept a
parameter, namely the time limit after which it should kill a running process.

Below is a diagram that shows the different components that we will implement in
this milestone
![Milestone 2]({{ site.baseurl }}/docs/milestone2.png)

## Learning objectives
In this milestone, we will gain familiarity with the following concepts:
- __kernel data structures__: You will use the data structures provided by the
  Linux kernel, namely a doubly linked list.
- __kernel timers__: You will use kernel timers to perform periodic tasks.
- __kernel contexts__: You will learn to identify the different contexts of
  execution that the kernel can be in. You will take a look at the dangers of
  performing certain operations from certain contexts.
- __kernel synchronization__: The Linux kernel is a multi-threaded kernel.
  Different threads of execution can be executing concurrently, so you will need
  to protect your critical sections.  Concurrency in the Linux kernel is
  challenging and you must be aware of the different contexts of execution to be
  able to identity which types of concurrency mechanisms to use.
- __kernel work__: You will make use of kernel work queues to perform periodic,
  yet non-atomic, tasks. Specifically, timers cannot sleep, so you will need to
  schedule works so that you can perform all the required operations.
- __kernel time management__: You will get exposed to the kernel mechanism for
  low-resolution time management, namely kernel `jiffies`.
- __reading process control blocks__: You will access the `task_struct`
  structure of the Linux kernel to collect information about a process from its
  PCB.
- __(Optional) sequential files__: This is an optional (yet very useful and less
  error-prone) step.  You will learn to use sequential files to allow users to
  read through a linked list.

## Logistics
- __Starter code__: Your code from [milestone
1](https://rhit-csse332.github.io/csse332-202130/docs/zz_project/).
- [Submission
box](https://moodle.rose-hulman.edu/mod/assign/view.php?id=2708259&forceview=1)
- __Due Date__: Monday, April 26, 2021 at 23:59 pm.

# Grade-school: Linked lists and process information
Okay now that we are in grade-school, it's time to start doing interesting
things. In this step, we will set up the infrastructure for us to maintain state
information about each process that registers with our system.

The Linux kernel provides us with a rich set of data structures that we can use,
and which are optimized for in-kernel usage. Of these data structures, we will
focus on a doubly linked list, defined in `linux/list.h`.

The kernel linked list API works on structures of type `struct list_head` that
must always reside as members of any custom structures you define. However, the
head of your linked list should always be an independent `struct list_head` that
you maintain outside of your custom structures (e.g., use a global `struct
list_head head` as the head of your linked list). A `struct list_head` contains
nothing but pointers to the next and the previous elements, i.e.,
```c
struct list_head {
  struct list_head *next, *prev;
}
```

For your convenience, the kernel provides you with the `LIST_HEAD(name)` macro
that defines and initializes a `struct list_head` with name `name` for you. So
define the head of your list as a global variable, you can use something like
```c
static LIST_HEAD(head);
```
and then use `head` anytime you want to go over your linked list.

Now, you must be asking the following question: if the kernel's linked list is
building a doubly linked list of `struct list_head`, then how do I get the
actual entries of my data structures?

Great question! Fortunately, the Linux kernel developers' have you covered. You
can use the macro `list_entry` to go from a `struct list_head` to its
enclosing custom structure.

## Example: A linked list
Here is a small example, let's say I want to create a linked list of strings in
the form of `const char *` pointers. First, I would define the head of my list
and then define my structure to hold the string as well as the list element as
follows:
```c
static LIST_HEAD(head); // this is the list head sentinel

struct my_str_list {
  const char *p_str; /* the actual string */

  struct list_head llist; /* the linked list element */
};
```
Now I can start iterating through my list. For example, assume I have already
inserted an entry into my list and I would like to recover it, then I can do
something like this:
```c
/* get the first entry in the list */
struct list_head *p_list_head = head.next;

/* recover my_str_list from the found list_head */
struct my_str_list = list_entry(p_list_head, /* the element from which to fetch */
                                struct my_str_list,  /* the type of what I want to fetch */
                                llist /* the name of the struct list_head element in my structure */
                                );
```

Take a look at the code in `linux/list.h`, you will find a lot of useful
routines that will turn out to be very useful, such as `list_add_tail` and the
`list_for_each` macro. Here is a [direct
link](https://www.kernel.org/doc/html/v4.15/core-api/kernel-api.html) to the
kernel documentation project that gives you a good amount of information about
the list management API in the kernel.

## What to do now
Okay, so now let's put what we have learned to good use. Modify your code from
milestone 1 to do the following:
1. Add a global variable that will represent the head of a linked list.
1. Replace the integer array with a linked list by doing the following steps
    1. Create a custom class, let's call it `csse332_info` that contains an
       integer and a `struct list_head` element, as described in the example
       above. Something along these lines
       ```c
       struct csse332_info {
         int value;

         struct list_head llist;
       };
       ```
    1. Modify your initialization function to create a linked list of 10
       integers, initialized to -1.
    1. Modify your `write` handler to set the appropriate integer in the linked
       list. In other words, if the user requests to write to index 5, then you
       code must first traverse the linked list to the appropriate element, and
       then use `list_entry` or any other set of list API calls to set the
       appropriate number.
1. Modify your `read` handler to read from the linked list instead from the
   array. This is a little bit tricky, so let's talk about it a bit more.

There are three main ways to modify your `read` handler to handle a linked list
instead of an array:
1. Use `offp` as a `struct list_head` pointer. This method requires that you
   override the semantics of `offp` by casting it into a `struct list_head`
   pointer and updating it carefully. I would recommend against this method as
   it is very error prone, but it requires the least amount of changes to your
   code, so tread carefully.
1. Use the `private_data` void pointer in the `struct file *filp` parameter of
   the `read` handler.  The `private_data` field is a void pointer, initially
   set to `NULL`, that the kernel provides for you to use as you wish (i.e., you
   can cast into whatever you want, allocate it, free it, etc.).  The
   `private_data` field is persistent as long as the user has the file open,
   i.e., for the same user in the same read session, `private_data` can be used
   to maintain state across different and subsequent `read` calls.
    - This approach is better than the first one, and less error prone. It
    requires more rewrite of your `read` handler, but is much more stable than
    the first approach.
1. Finally, our most stable solution would be to use a sequential file, i.e., to
   change our file operations to use sequential file reading instead of block
   file reading. This is going to require a full rewrite of your `read` handler
   and some other functions, but is the most stable and is the one I highly
   recommend for this milestone. It will take a while for you to write it now,
   but will save you a lot of time the further you go into the milestone.

I have provided you with a skeleton of what implementing sequential access to
your linked list would look like. I have highlighted the places in which you
should be adding your code.
```c
static void *csse332_start(struct seq_file *s, loff_t *pos)
{
  /* Your code goes here */
}

static void *csse332_next(struct seq_file *s, void *v, loff_t *pos)
{
  /* Your code goes here */
}

static int csse332_show(struct seq_file *s, void *v)
{
  /* Your code goes here */
}

static void csse332_stop(struct seq_file *s, void *v)
{
  /* Your code goes here! */
}

static const struct seq_operations csse332_seqops = {
	.start = csse332_start,
	.next  = csse332_next,
	.show  = csse332_show,
	.stop  = csse332_stop,
};

static int
csse332_open(struct inode *inode, struct file *filp)
{
	return seq_open(filp, &csse332_seqops);
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(5,6,0)
static const struct file_operations csse332_fops = {
	.owner   = THIS_MODULE,
	.open    = csse332_open,
	.read    = seq_read,
	.write   = csse332_write,
	.llseek  = seq_lseek,
	.release = seq_release,
};
#else
static const struct proc_ops csse332_fops = {
	.proc_open    = csse332_open,
	.proc_read    = seq_read,
	.proc_write   = csse332_write,
	.proc_lseek   = seq_lseek,
	.proc_release = seq_release,
};
#endif
```

# Middle School: Maintain process information
So we have our database set up now, and we can read from it using one of the
three methods above. Awesome! Now, let's make our database hold some important
information. In what follows, let's assume that we did something like
```c
#define HISTORY_LEN 5
```
You can modify this value later on to better test your implementation.

Modify your `csse332_info` structure to hold the following set of information:
1. The `pid` of the process.
1. An integer array of length `HISTORY_LEN` that represents the history of the
   CPUs that the process has run on.
1. An integer array of length `HISTORY_LEN` that represents the history of the
   process's user timestamps (i.e., how much has the process consumed from user
   time).
1. An `long` array of length `HISTORY_LEN` that represents the history of the
   last `HISTORY_LEN` states that the process has been in.
1. A pointer to the kernel's `struct task_struct` that corresponds to this
   process.
1. A `struct list_head` entry for maintaining your structure in a kernel linked
   list.
1. Any other data members that you think you might need now or later on.

Do not worry yet about who fills this information up, we will discuss those in
the next section when we talk about the `write` handler.

Next, modify your `init` function to no longer allocate any of the elements of
the linked list. Entries on this list will be created and maintained by the
`write` handler and some other functions that we will later on worry about.

## Reading the database
Now, we would like to modify our `read` handler (or `csse332_show` if you
are using sequential files) to show the content of our database instead of
showing the list of numbers. Our database will be empty so far, so we can't test
this yet, but we'll do that in the next section.

In this milestone, you __must__ show the following information about each
process:
- The process `pid`
- The process's program name
- The last `HISTORY_LEN` user time timestamps.
- The numbers of the last `HISTORY_LEN` entries of the CPUs that the process has
  run on
- The last `HISTORY_LEN` scheduler states represented as strings.
- A list of children in the form of `(pid, name)` if the process has any
    children, or `NONE` if the process has no children.

I have provided you with a function that takes the process state as an integer
and then return its string representation below
```c
/*
 * sttostr - Return the string representation of a process state
 *
 * @state: The process state to print
 * @return: A const char * pointer to a string
 */
static const char *sttostr(long state)
{
	if (state == -1)
		return "TASK EMPTY";

	if (!state)
		return "TASK_RUNNING";
	else if (state & TASK_INTERRUPTIBLE)
		return "TASK_INTERRUPTIBLE";
	else if (state & TASK_UNINTERRUPTIBLE)
		return "TASK_UNINTERRUPTIBLE";
	else
		return "TASK_OTHER";
}
```

For reading the database of process information, you must use the following
formatting for each process entry:
```
Process Id: <process_id>
Process Name: <process_name>
User Time: <user time[0]> <user time[1]> <user time[2]> <user time[3]> <user time[4]>
CPU Used: <cpu[0]> <cpu[1]> <cpu[2]> <cpu[3]> <cpu[4]>
Sched Status: <str(state[0])> <str(state[1]> <str(state[2]> <str(state[3]> <str(state[4])>
Children: <(pid of child, name of child)> or None
```

Here is an example of my output when I have two processes registered, one of
which is the child of the first (you will only see this after implementing the
full milestone, but this is for your reference when writing your `read` handler)
```
Process Id: 30423
Program Name: userapp
User time: 20064000000 6176000000 9696000000 13208000000 16752000000
CPU Used: 1 0 1 0 1
Sched Status: [TASK_RUNNING] [TASK_RUNNING] [TASK_RUNNING] [TASK_RUNNING] [TASK_RUNNING]
Children: (30426,userapp)

Process Id: 30426
Program Name: userapp
User time: 0 0 0 0 0
CPU Used: 1 1 1 1 1
Sched Status: [TASK_INTERRUPTIBLE] [TASK_INTERRUPTIBLE] [TASK_INTERRUPTIBLE] [TASK_INTERRUPTIBLE] [TASK_INTERRUPTIBLE]
Children: NONE
```

## Writing: Register and De-registering
Alright so we have built our database and have a mechanism for reading it.
Awesome! Let's now switch to the process of registration.

### Registering a process
When a user process would like to register, it must write into our `procfs`
entry (we called it `/proc/csse332/status`) a message with the following format
```
R, pid
```
where `pid` is the id of the process.

Your kernel module must parse this message, and then do the following:
- __VERY IMPORTANT STEP__: You should make sure that the process is trying to
  register itself, and not register another process with another `pid`. But how
  do I do that I hear you say? How can I know the process trying to register is
  the same one as the one issuing the `write` call? Luckily, the kernel's got
  you covered. When executing in process context (review our in-class disucssion
  for more information), the kernel maintains a pointer to the current running
  user process that can be accessed by using the `current` macro defined in
  `linux/sched.h`. So your job boils down to asserting that the `pid` passed to
  `write` is the same as that of the `current` task struct. Make sure to return
  meaningful error codes. Violation of this condition is a permissions issues
  and not a memory corruption or an invalid parameter.
- If the command passed is `R`, then the module must register the process by
  - allocating a `struct csse332_info` for the process
  - updating the variables of this structure
  - adding the created structure to the linked list (order does not matter)

I strongly suggest that you break down your code into individual functions. That
  is much easier to debug and makes extending your code a smoother task.

### De-registering a process
Similar to registration, a process might want to de-register from our framework
and we must provide support for that option. When a process wishes to
de-register, it will write `W, pid` to our module's `procfs` entry. Your module
must parse the command and then
- Make sure that the calling process has the same `pid` as the one that was
  passed in the command
- Find the `struct csse332_info` that corresponds to that specific process, if
  any.
- Remove the structure from the linked list (check `linux/list.h` for helpers)
- Free up any space used by the corresponding `struct csse332_info` and any of
  its elements.

# High School: Timers, Work queues, and synchronization
Okay now that we have our infrastructure set up, let's get started with
regularly updating the contents of our data structures. To do so, we will use
two important concepts provided by the Linux kernel: __timers__ and __work
queues__.

## Time in the Linux kernel
The Linux kernel keeps track of the flow of time with the means of interrupts
generated at regular intervals from timing hardware. In most x86 architectures,
the kernel increments its internal time counter every 1/1000 secs (or with a
frequency of 1000Hz).

The kernel tracks the number of interrupts that have happened using an internal
64 bit variable called `jiffies` (found in `linux/jiffies.h`). `jiffies` is
initialized to 0 at boot times. In other words, `jiffies` represent the number
of ticks (i.e., timing hardware interrupts) since boot time.

To convert a `jiffies` value into milliseconds, you can use the kernel provided
function
```c
unsigned long msecs_to_jiffies(const unsigned int m);
```
For example, if in your code you use something like
```c
unsigned long x = jiffies + msecs_to_jiffies(5000);
```
then `x` represents the time in the future that is 5 seconds away. That is very
helpful when trying to schedule work in the future.

## Using timers to update process information
Now it is time to start updating the `cpu`, `state`, and `user time` elements in
our process information. To do so, we will schedule a kernel timer to run every
5 seconds, read the updated values from `struct task_struct` variable for each
registered process, and then update the values in the `csse332_info` structure.

Timers allow you to schedule the execution of a function at a particular
time in the future. Kernel timers are data structures that instruct the kernel
to execute (1) a user-defined function with (2) user-defined arguments and (3)
at a user-defined time.  Timers run asynchronously, i.e., they can trigger at
any point during execution and cause a context switch of the current running job
if allowed.

Albeit very useful, kernel timers come with a challenge: they are generated as
_software interrupts_ or _SoftIRQs_ (Sofware Interrupt ReQuest). This means that
our user-defined time function will run in the software interrupt context (also
referred to as the softirq context), which means that we have limitations on
what we can do in our user-defined timer functions. Those limitations are:
1. No access to user space is allowed. Because we are not running in process
   context, there is no path to user space.
1. The `current` pointer is not meaningful in this context.
1. More importantly, __no sleeping or scheduling__ may be performed. This means
   that you cannot use any function that might sleep, including `printk` and its
   variants (`pr_info`, `pr_err`, etc.), `kmalloc(..., GFP_KERNEL)` and `kfree`.
   Also, semaphores must not be used in this context as they might sleep.

In other words, when in the softirq context, scheduling is disabled, so we might
end up hogging the CPU and freezing our system in case we are not very careful
about the above constraints. Yikes!

Okay, it seems there's very little we can do from the timer handler function, so
what do we do? Kernel work queues to the rescue.

## Deferring work with work queues
In order to solve the limitations of the softirq context, the kernel provides
you with the ability to create a queue of work that you must do. Once you put a
work on a work queue, the kernel will schedule a kernel thread that will run
that work for you in the kernel process context (i.e., the kernel is running its
own code and not on behalf of a process).

Therefore, it is very common practice to use the softirq handler to only queue
works on the work queue, and then let the works do the jobs that might require
sleeping or synchronization.

### Implementation tasks
At this point, let's go ahead and write our timers and work queue handlers.
Perform the following steps:
- Create a global timer of type `struct timer_list`
- Create a global work queue pointer of type `struct workqueue_struct *`
- Create a global work pointer of type `struct work_struct *`
- Create a function timer_callback as follows:
  ```c
  static void timer_callback(struct timer_list *timer)
  {
    /* add code here */
  }
  ```
- Create a function to handle the work callback as follows:
  ```c
  static void work_handler(struct work_struct *work)
  {
    /* add code here */
  }
  ```
- Your timer handler should only queue works on the workqueue.
- Your work handler is where the real work of updating the database must happen.
  - To figure out which values you need to update the database information,
  take a look at the `struct task_struct` definition
  [here](https://elixir.bootlin.com/linux/v4.5/source/include/linux/sched.h#L1389).
- In your initialization function,
  - Setup and schedule the timer to run in the future after 5 seconds. For more
  information, check out [this LWN article](https://lwn.net/Articles/735887/).
  - Create the work queue and allocate space for the work while passing the
  correct parameters. You can find out more about the work queue API
  [here](https://lwn.net/Articles/11360/).
- Recall that everything you allocate and initialize, you must later on destroy
or free.

### Important note
Note that kernel timers are one-shot only, i.e., they can only be set for one
time in the future. If you need your timer to fire again, then you need to set
it again whenever you need to. Your job in this milestone is to figure out where
in your code you should set your timer so that it runs correctly every 5
seconds.

## Synchronization
You might have noticed now that we do have a huge race condition! Our database
is accessed and updated by several entities. For example, a process might issue a
write right at the same time that a timer expires, and another process is trying
to read. Even worse, several processes might be reading/writing to our database
at the same time, and then a timer expires! Ugly!

Your job in this milestone is the following:
- Read the [Unreliable Guide to
Locking](https://www.kernel.org/doc/html/v4.13/kernel-hacking/locking.html) in
the Linux kernel.
- Figure out what synchronization constructs you must use to protect accesses to
the database from race conditions.

Notes:
1. Locking and synchronization in the Linux kernel largely depends on which
   context you are running from. Read the guide carefully and then list  all of
   the contexts that might access your database, then select the appropriate
   type of locking mechanisms.
1. Be careful not to create deadlocks! A deadlock in the kernel is drastic, it
   will break your machine and you will have to restart it.

# Graduation: Killing misbehaving processes and module parameters
By now, you should be able to read and write to your module, register and
de-register processes, and your timer handler and work are handling the updates
to the database.

The last piece we would like to take care of is the termination of misbehaving
processes. We define a misbehaving process as one that has been running for more
than a predefined threshold of time, let's call that threshold `thr`.

Your job in this task is do the following:
1. Allow your module to accept parameters during insertion, that parameter will
   be the `thr` threshold. To do so, after defining a global variable, you can
   use the `module_param` function and the `MODULE_PARAM_DESC` macro to define
   and describe your parameter. Then, when inserting the module, you can use
   ```shell
   sudo ./insmod project.ko thr=50000
   ```
   to set the threshold to 50 seconds.
1. Implement the mechanism to kill any process that misbehaves and takes more
   than 50 seconds to complete all of its duties. There are multiple ways to do
   so, you are free to select the one that works best for you. Document your
   choices in a `README.md` file so I can give you partial credit.
  - Your code must send the user space process a signal that it cannot ignore or
  overwrite its default handler. To send a signal from kernel space to user
  space, you can use the function `send_sig_info` defined in `linux/sched.h`.

   __Note__: If the process you are trying to kill has children, then we will
   not orphan those children. Instead, we will kill them all, i.e., do a
   genocide.

   __Note__: You may assume that all processes in our system can have children
   by not grandchildren.

# Milestone Evaluation
Please take a moment to fill out the survey about this milestone by following [this
link](https://forms.gle/gRT6fKTKFd2qekiu5)

# Rubric

| Part                                                                        | Point Value |
|:----------------------------------------------------------------------------|-------------|
| Registration                                                                | 10   |
| De-registration                                                             | 10   |
| Reading the database content                                                | 10   |
| Maintaining the database (kernel linked list, correct add, correct delete)  | 40   |
| Timers correctly set up works                                               | 40   |
| Correct implementation of the workqueue                                     | 40   | 
| Correct updates to the database                                             | 20   |
| Kill misbehaving processes                                                  | 20   |
| Kernel synchronization                                                      | 40   |
| Survey                                                                      | 10   |
| __Total__                                                                   | 240  |

