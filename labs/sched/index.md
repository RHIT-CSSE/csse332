---
layout: post
title: Scheduling Activity
readtime: true
date: Tuesday Jan 23 2024
last-updated: Mon Jan 22 2024
---

# Introduction

In this activity, we will be exploring the xv6 scheduler by first trying to
understand the way it is actually implemented. We will then think about the
advantages and disadvantages of the current xv6 implementation and try to
identify ways in which it can be improved. Finally, we will spend the remainder
of the session trying to implement said optimization and testing out their
effects.

## Getting the source code

For this activity, we will need to mess around with the code for the xv6
kernel. So before you start this activity, please make sure that all of your
code for your labs is pushed to your own private repo, otherwise, we might
risk mixing things up and losing your progress.

### Backing up your lab code 

If you have made a private xv6 repo, first commit and push all of your changes
to your repo. You can do so using:

```shell
$ git commit -am "saving progress"
$ git push
```

### Getting on the right branch

First, switch to the `main` branch to get everything back in order, to do so,
use (recall, if your main branch's name is `master`, use `master`)

```sh
$ git checkout main
```

Next, fetch the changes from our end using

```sh
$ git fetch upstream
```

Then, make sure that the branch `klist` shows up

```sh
$ git branch -a 
  clab_solution
  heapmm_solution
* main
  remotes/origin/clab_solution
  remotes/origin/heapmm_solution
  remotes/origin/main
  remotes/upstream/clab
  remotes/upstream/heapmm
  remotes/upstream/main
  remotes/upstream/paging_act
  remotes/upstream/lazy_alloc
  remotes/upstream/klist
```

Then, get the `klist` branch as follows:

```sh
$ git checkout -b klist_sol upstream/klist
branch 'klist_sol' set up to track 'upstream/klist'.
Switched to a new branch 'klist'
```

Finally, push the empty stub to your own repo using:

```sh
$ git push --set-upstream origin klist_sol
Enumerating objects: 99, done.
Counting objects: 100% (99/99), done.
Delta compression using up to 56 threads
Compressing objects: 100% (80/80), done.
Writing objects: 100% (88/88), 3.15 MiB | 11.39 MiB/s, done.
Total 88 (delta 15), reused 19 (delta 2), pack-reused 0
remote: Resolving deltas: 100% (15/15), completed with 11 local objects.
remote:
remote: Create a pull request for 'klist_sol' on GitHub by visiting:
remote:      https://github.com/user/csse332-labs-user/pull/new/klist_sol
remote:
To github.com:user/csse332-labs-noureddi.git
 * [new branch]      klist_sol -> klist_sol
branch 'klist_sol' set up to track 'origin/klist_sol'.
```

# Understanding the xv6 Scheduler

To get a better idea of the scheduling approach in xv6, it might be a good idea
to skim [Chapter 7 of the xv6
book](https://pdos.csail.mit.edu/6.828/2022/xv6/book-riscv-rev3.pdf).

In your groups, take a few moments to explore the xv6 kernel's source code and
attempt to answer the following questions about the xv6 scheduler:

- ❓ What scheduling policy is xv6 implementing?
- ❓ What are the possible states that an xv6 process can be in?
- ❓ Which function is used to switch from one process back to the
  scheduler and vice versa?
- ❓ How does the xv6 kernel handle scheduling on multiple CPUs?

- 🎶 You might find it helpful here to draw a small chain that represents the
  sequence of events that can happen when scheduling is involved.

_Hint_:  Most of what you are going to need to answer the above questions can be
found in the files `kernel/proc.h` and `kernel/proc.c`. Moreover, to answer the
last question accurately, looking at `kernel/trap.c` might also be helpful.

# Drawbacks for the current implementation

Within your group, think of at least two possible disadvantages of the current
implementation of the xv6 scheduler. You will need to argue for your choices
when presenting them. You will also need to provide possible solutions for
those disadvantages. You might find it helpful to think of edge cases that the
current implementation of the scheduler does not consider.

# Activity

In this activity, after discussing the drawbacks with your instructor, work
with your group to solve both disadvantages. To help you out, we have provided
you with helper functions that will make your life easier.

## The `btop` utility

To help you visualize the utilization of the resources on your device, you can
make use of the `btop` utility. You can install it on your Linux virtual
machine (not xv6) using `sudo apt install -y btop`.

## Part 1: Turning off the CPUs

In some cases, it might be useful for the operating system to turn off the CPUs
to conserve power, heating, and batter life (if applicable). Therefore,
instruction set architectures (ISA) such as RISC-V come equipped with the wfi
(wait-for-interrupt) instruction. `wfi` puts the CPU core on which it is
executing into a low power state until an interrupt is received. At that point,
the core is awakened and it can resume executing from where it left off. This
is very similar to the way condition variables work, but instead, we are
dealing with the physical hardware on the machine.

Under `kernel/proc.c`, we have added the following function for you:

```c
static inline void
__wfi(void)
{
  asm volatile("wfi");
}
```

This function will allow you to issue the `wfi` assembly instruction from your
C source code whenever you need it.

Solve the first inefficiency of the xv6 scheduler using the `wfi` instruction.
Verify that your kernel is conserving power and efficiency using the `btop`
utility to visualize your processor's utilization.

## Part 2: Better data structures

To solve the implementation problems with the xv6 scheduler, it is desirable
for us to implement an more efficient scheduling data structure. Feel free to
implement your own from scratch if you feel up for it, but to help you out, we
have provided you with our own custom implementation of a kernel linked list.
Our implementation actually mirrors the one used in the Linux kernel.

### The list API

To help you out with this part, we have provided you with the implementation of
a kernel-level doubly-linked list that you can use. The list is defined in
`kernel/proc.h` and implemented in `kernel/list.c`.

The list is implemented in a way such that the head of the list is always a
__sentinel__, i.e., it does not contain any data. In other words, the first
element in the list (or in our case the queue) will be `head->next`.

There are four important functions that you can use from the list API:

1. `init_list_head(struct list_head *list)`: This function initialize a list
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

  // 7. read an item from the top of the list
  struct list_head *top = &head.next;
  if(top != &head) {
    // always do error checking to make sure that the list is not empty
    struct my_data_struct *container = (struct my_data_struct*)top;
    printf("%d\n", container->data);
  }

  // 8. delete s1 from the list
  list_del(&s1.list);
  // list now becomes head -> s3.list -> s2.list

  // 9. swap s2 and s3
  list_del_init(&s2.list);
  list_del_init(&s3.list);
  list_add(&head, &s2.list);
  list_add_tail(&head, &s3.list);

  // 10. pop the top item from the list
  top = &head.next;
  if(top != &head) {
    // always check if the list is empty to avoid serious segmentation faults
    list_del_init(top);
  }
}
```

## Implementation plan

- Add a `struct list_head` to the process control block (make sure that it is
  __the top element__ of the structure).

- Add a `struct list_head runq` as a global variable in `proc.c`.

- Anytime a process becomes **ready** to be executed, add it to the run queue.
  This step will require you to understand the scheduling code for xv6. Make
  sure to use Chapter 7 of the book as a reference. You might find it useful to
  add print statements to the scheduler so that you can figure out the call
  hierarchy of the scheduler.

At this point in time, you are adding the processes to the run queue, but you
are not scheduling them. Modify the `scheduler` function in `proc.c` to schedule
processes from the run queue instead of scanning the entire list of processes.
Take a look at the original scheduling code and devise a plan to replace it with
the code that uses the run queue.

### Testing

At this point in time, you should try to run your scheduler, after compiling
your code, you should be able to drop into the main xv6 shell and start issuing
commands. If you can get to the shell but your xv6 kernel crashes after that,
that's okay, there's one more thing we need to do.

## Synchronization

xv6 support multiprocessing and is currently configured to run a virtual machine
with two cores (or harts). This means that there are two schedulers running for
each CPU. Well, that creates issues!  Imagine that the scheduler for the first
CPU accesses the run queue and wants to schedule the process at the top of the
queue. But before doing that, the scheduler from the second CPU grabs that same
process and tries to schedule it. All hell can break loose in that case, imagine
trying to run the same process on two CPUs, Yikes!

### Turning Off Multiprocessing

Before we start dealing with locking, let's just confirm that our scheduler
works. Since xv6 runs on top of the `qemu` emulator, we do have full control over
how many processors can the xv6 machine run. Therefore, to avoid the above
synchronization problem, we can ask `qemu` to run xv6 on a single hart machine,
thus eliminating the possible race conditions.

To do so, you can use `make CPUS=1 qemu` instead of `make qemu` to build and
run xv6. This will ensure that your qemu system launches with a single RISC-V
hart.


## Handling Synchronization using Locks

To completely help resolve this issue, you must make sure that all changes to
the run queue are atomic. Changes to the run queue include both adding elements
to it and removing elements from it.

Modify your solution so far and use synchronization primitives to make sure that
all changes to the run queue happen atomically. You can find the `struct
spinlock` useful. Take a look at how the process locks are managed to learn the
API.

### Testing

For final testing, make sure that you compile with multiple processors enabled
using `make qemu` instead of `make CPUS=1 qemu`.

At this point, your scheduler should be ready for final testing. To make sure
everything works correctly, try the fork tests again

```shell
$ usertests forkforkfork
usertests starting
test forkforkfork: OK
ALL TESTS PASSED
$
```

# Submitting Your Code

From the Linux terminal, issue the command (make sure you are in the `xv6-riscv`
directory in your repository):

  ```shell
  ./create_submission.sh <username>
  ```
and replace `<username>` with your RHIT username (without the `<` and `>`). For
example, for me, that would be:

  ```shell
  ./create_submission.sh noureddi
  ```

If you get a message saying that you don't have permission to run
`./create_submission.sh`, then issue the following command first 

  ```shell
  chmod +x ./create_submission.sh
  ```

Here's the output as it shows up on my end:

  ```
  Cleaning up xv6 directory...
  Process started: writing temporaries to /tmp/dab9bfedf8d508c2a1c3f1c95e6ba1fc.txt
  Found the following modified files:
  ./user/rhmalloc.c
  Creating the submission zip file.
    adding: user/rhmalloc.c (deflated 54%)
  Done...
  ################################################################
          submission_noureddi.zip has been created.
     Please submit THIS FILE AND THIS FILE ONLY to Gradescope.
  ################################################################
  ```

This will generate a single file called `submission-username.zip` (for me, it
would be `submission-noureddi.zip`). That is all you need to upload to
[Gradescope]({{site.gradescope_url}}).

## Submission Checklist

- [ ]  My code compiles and generates the right executables.
- [ ]  I ran the submission script to generate my `zip` file.
- [ ]  I submitted the `zip` file  to [Gradescope]({{site.gradescope_url}}).

---

If you notice any typos or inaccuracies, please open a GitHub issue on this
[repository]({{site.gh_repository_url}}).

