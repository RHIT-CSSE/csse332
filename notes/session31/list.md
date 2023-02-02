---
layout: post
title: The List API
readtime: true
date: Thu Feb 2 2023 
---

# Introduction

To solve the implementation problems with the xv6 scheduler, it is desirable for
us to implement an O(1) scheduling data structure. Feel free to implement your
own from scratch if you feel up for it, but to help you out, we have provided
you with our own custom implementation of a kernel linked list. Our
implementation actually mirrors the one used in the Linux kernel.

# The List API

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
  use Chapter 7 of the book as a reference. You might find it useful to add
  print statements to the scheduler so that you can figure out the call
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
work. Since xv6 runs on top of the `qemu` emulator, we do have full control over
how many processors can the xv6 machine run. Therefore, to avoid the above
synchronization problem, we can ask `qemu` to run xv6 on a single hart machine,
thus eliminating the possible race conditions.

To do so, edit the `Makefile` in the top level directory. Go to line 157 and
change the line that says `CPUS := 3` to `CPUS := 1`, and then recompile xv6
using `make clean && make qemu`.

xv6 should now run on a single core and you can test whether your scheduler is
functional or not.

## Handling Synchronization using Locks

To completely help resolve this issue, you must make sure that all changes to
the run queue are atomic. Changes to the run queue include both adding elements
to it and removing elements from it.

Modify your solution so far and use synchronization primitives to make sure that
all changes to the run queue happen atomically. You can find the `struct
spinlock` useful. Take a look at how the process locks are managed to learn the
API.

### Testing

Remember to modify the number of CPUs back to 3 to make this problem meaningful.
Simple undo the changes that you made to the `Makefile` in the previous step and
reset `CPUS := 3` on line 157.

At this point, your scheduler should be ready for final testing. To make sure
everything works correctly, try the fork tests again

```shell
$ usertests forkforkfork
usertests starting
test forkforkfork: OK
ALL TESTS PASSED
$
```

