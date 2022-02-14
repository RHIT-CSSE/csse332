---
layout: post
title: xv6 scheduler project
readtime: true
date:
gh-repo: rhit-csse332/csse332
gh-badge: [star,watch,follow]
---

# Introduction

In this checkpoint of the project, we will be modifying the xv6 scheduler to
work off of a run-queue instead of the entire list of processes.  We will have
to worry greatly about synchronization since the scheduler might be running from
different CPUs, which can create chaos if we don't take care of our race
conditions.

## The xv6 scheduler

To get more detailed information about the xv6 scheduler, I recommend that you
read [chapter 7 of the xv6
book](https://pdos.csail.mit.edu/6.828/2021/xv6/book-riscv-rev2.pdf).

xv6 implements a round-robin preemptive scheduler with a fixed quantum. In other
words, the scheduler picks a process and runs it for a specific fixed amount of
clock ticks, it then forces that process to `yield` if it is not done, and then
schedules the next process to run from the list of processes.

The main scheduler loop can be seen below and in `proc.c` in the `scheduler`
function:
```c
for(p = proc; p < &proc[NPROC]; p++) {
	acquire(&p->lock);
	if(p->state == RUNNABLE) {
		// Switch to chosen process.  It is the process's job
		// to release its lock and then reacquire it
		// before jumping back to us.
		p->state = RUNNING;
		c->proc = p;
		printf("Schedule process %d\n", p->pid);
		swtch(&c->context, &p->context);

		// Process is done running for now.
		// It should have changed its p->state before coming back.
		c->proc = 0;
	}
	release(&p->lock);
}
```

The scheduler iterates over the entire list of processes and checks them
one-by-one to find a schedulable process. Once it finds a schedulable process,
i.e., one in the `RUNNABLE` state, the scheduler will switch contexts to that
process __while holding its appropriate lock__. It is the job of the process
itself to then release its lock once it wakes up. For reasoning about why this
break of convention (the process locking the lock (the scheduler) is not the one
releasing it (the process itself)), see Chapter 7 of the xv6 book referenced
above.

The current implementation of the scheduler is clearly wasteful: imagine that
there are no processes to run (i.e., all processes are sleeping), the scheduler
will wake up and keep iterating over the list of processes forever, not finding
any process to schedule for execution. Even worse, each CPU (we have 2 in this
case) will run its own per-CPU scheduler, so we will end up with two cores
running forever, iterating over a list that does not contain any schedulable
processes.

Your job in this milestone is to design and write a more efficient scheduler
that can turn off CPU cores when no processes are ready to be scheduled.

## Getting the code

Update your public repository using `git pull` to obtain the update starter code
for this milestone. 

# Adding a run queue

So the first step we'd like to do is to switch from traversing the entire list
of processes to traversing only those processes that are in a state ready to be
scheduled.
Your job in this step is to implement a __ready queue__, i.e., a queue of
processes that are ready to be executed and thus can be scheduled. The scheduler
would then routinely grab the top of the queue and schedule it without the need
for any iteration.

## The list API

To help you out with this step, we have provided you with the implementation of
a kernel-level doubly-linked list that you can use. The list is defined using in
`kernel/proc.h` and implemented in `kernel/list.c`.

The list is implemented in a way such that the head of the list is always a
__sentinel__, i.e., it does not contain any data. In other words, the first
element in the list (or in our case the queue) will be `head->next`.

There are four important functions that you can use from the list API:
1. `init_list_list(struct list_head *list)`: This function initialize a list
head to point to itself.
2. `list_add(struct list_head *head, struct list_head *new)`: Add `new` at the
head of the list, i.e., `new` becomes the first element of the list.
3. `list_add_tail(struct list_head *head, struct list_head *new)`: Add `new` at
the tail of the list, i.e., `new` becomes the last element in the list.
4. `list_del(struct list_head *list)`: Remove `list` from its corresponding
list.
5. `list_del_init(struct list_head *list)`: Remove `list` from its corresponding
list and reinitialize it to be reused later on.

Below is an example of how to use the API:
```c
// This is the head of the list: Contains no data!
struct list_head head;

// This is the structure containing the data we want to put into a list
struct my_data_struct {
	// PUT THIS AT THE START OF YOUR STRUCTURE OR ANY STRUCTURE YOU USE IT IN
	// THE LIST. IT WON'T WORK OTHERWISE.
	struct list_head list;

	// data and other stuff go here
	int data;
};

void main(void)
{
	// 1. initialize the sentinel head of the list.
	init_list_head(&head);

	// 2. create 3 nodes
	struct my_data_struct s1, s2, s3;
	// or can malloc the same:
	// struct my_data_struct *s1 = malloc(sizeof(struct my_data_struct));

	// 3. Init the list heads inside of the structure
	init_list_head(&s1.list);
	init_list_head(&s2.list);
	init_list_head(&s3.list);

	// 4. Add s1 to the start of the list
	list_add(&head, &s1.list);
	// list now become head -> s1.list

	// 5. Add s2 to the tail of the list
	list_add_tail(&head, &s2.list);
	// list now becomes head -> s1.list -> s2.list

	// 6. Add s3 to the head of the list
	list_add(&head, &s3.list);
	// list now becomes head -> s3.list -> s1.list -> s2.list

	// 7. iterate over the list
	struct list_head *iterator = &head.next;
	struct my_data_struct *container;
	while(iterator != &head) {
		// 7.1 Extract the data from the list head
		// This is why the list_head must be the first element in the structure,
		// because we need to cast the smaller structure into the containing
		// structure.
		container = (struct my_data_struct *)iterator;
		printf("%d\n", container->data);

		iterator = iterator->next;
	}

	// 8. delete s1 from the list
	list_del(&s1.list);
	// list now becomes head -> s3.list -> s2.list

	// 9. swap s2 and s3
	list_del_init(&s2.list);
	list_del_init(&s3.list);
	list_add(&head, &s2.list);
	list_add_tail(&head, &s3.list);
}
```

## Implementation plan
- Add a `struct list_head` to the process control block (make sure that it is
__the top element__ of the structure).
- Add a `struct list_head runq` as a global variable in `proc.c`.
- Anytime a process becomes ready to be executed, add it to the run queue. This
step will require you to understand the scheduling code for xv6. Make sure to
use Chapter 7 of the book as a reference. You might find it useful to add print
statements to the scheduler so that you can figure out the call hierarchy of the
scheduler.

At this point in time, you are adding the processes to the run queue, but you
are not scheduling them. Modify the `scheduler` function in `proc.c` to schedule
processes from the run queue instead of scnanning the entire list of processes.
You may have noticed that the hand-holding instructions end here, that is
because we want you to design your own scheduler based on what we have seen
about a round-robin scheduler in class and in our userspace threads assignment.

Take a look at the original scheduling code and devise a plan to replace it with
the code that uses the run queue.

### Testing

At this point in time, you should try to run your scheduler, after compiling
your code, you should be able to drop into the main xv6 shell and start issuing
commands. If you can get to the shell but your xv6 kernel crashes after that,
that's okay, we'll take care of that in the next step.

### Synchronization

xv6 support multiprocessing and is currently configured to run a virtual machine
with two cores (or harts). This means that there are two schedulers running for
each CPU. Well, that creates issues!  Imagine that the scheduler for the first
CPU access the run queue and wants to schedule the process at the top of the
queue. But before doing that, the scheduler from the second CPU grabs that same
process and tries to schedule it. All hell can break loose in that case, imagine
trying to run the same process on two CPUs, Yikes!

To help resolve this issue, you must make sure that all changes to the run queue
are atomic. Changes to the run queue include both adding elements to it and
removing elements from it.

Modify your solution so far and use synchronization primitives to make sure that
all changes to the run queue happen atomically. You can find the `struct
spinlock` useful. Take a look at how the process locks are managed to learn the
API.

### Testing

At this point, your scheduler should be ready for final testing. To make sure
everything works correctly, try the fork tests again
```shell
$ usertests forkforkfork
usertests starting
test forkforkfork: OK
ALL TESTS PASSED
$
```


# Turning off the CPUs

Our code so far is an improvement over the original xv6 scheduler in that it
doesn't require constant polling of the entire list of processes, we only care
processes that are ready to be scheduled. However, imagine the case where
nothing is happening on the machine, i.e., we are just waiting for user input
and nothing is running in the background. In that case, each CPU's scheduler
will keep checking the ready queue, and since it's empty, will do nothing and
go back to checking the ready queue again, and so on. This is clearly very
wasteful since the CPUs are spinning infinitely but are not doing any useful
work.

To alleviate this problem, we would like to turn off CPU cores when there are no
processes in the ready queue. In that case, the CPU core will enter a low power
state until an interrupt is received, either from a timer, or from a user
pressing a key, or a hardware interrupt, etc. This can be achieved by using the
`wfi` (wait-for-interrupt) instruction from the RISCV instruction set. `wfi`
puts the corresponding core into a low power state until an interrupt is
received. At that point, the core is awakened and it can resume execution from
where it left off. This is similar to a condition variable wait, but instead it
affects the actual hardware on the machine.

## Implementation

In order to implement this feature, we need to first be able to call an assembly
instruction from our kernel code implementation. Luckily, the compiler allows
you to do by using the `asm` directive as follows:
```c
static inline
__wfi(void)
{
	asm volatile("wfi");
}
```
When you call the function `__wfi()` from the kernel code, the compiler will
replace the body of the function with the assembly instruction `wfi`.
Your job in this step to figure out where and when you must call the `__wfi()`
function so that you turn off the CPU cores and make scheduling more efficient.

## Testing

To test this step, we will have to take a look at the CPU itilization of the
qemu virtual machine when it is waiting for user inputs. For that, we will make
use of the `htop` utility that allows us to track the state of processes and the
current CPU utilization.

First of all, install `htop` using
```shell
sudo apt install -y htop
```

Next, start the xv6 virtual machine using `make qemu` and DO NOT press any user
inputs. In another terminal, start the `htop -F qemu` utility and observe the
CPU utilization of the `qemu` processes. On my machine, before adding the `wfi`
instruction, CPU utilization was always at 100%, while after adding `wfi`, the
CPU utilization of the qemu processes dropped down to 0.


<!--
# Scheduling with priorities

- Add a system call (called `nice`) to change the priority of a give process
- Add a priority element to the PCB.
- Schedule from the run queue but now it is a priority queue
	- It is okay to iterator over the run queue and find the process that
	has the highest priority
	- Better if you can implement a max-heap.
-->
