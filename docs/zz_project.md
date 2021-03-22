---
layout: post
title: Term Project (Milestone 1)
readtime: true
date: Mon Mar 22 17:31:48 2021
gh-repo: rhit-csse332/csse332-202130
gh-badge: [star,watch,follow]
---

# Introduction
In this project, we will write a kernel module that allows us to turn our operating system into a
_real-time operating system_ for a short period of time, and a for a specific set of processes. In
other words, our module will take care of keeping track of running processes, monitoring their
status, and scheduling them to meet some real time guarantees. Eventually, our module will run a
_rate-monotonic scheduler_ (RMS) that assigns priorities to running processes, decide when to accept
or reject incoming processes, and preempt a running process in case one with higher priority shows
up. This project will be organized in 3 to 4 milestones, depending on how our quarter goes.  But
let's put to the side for now, and focus on getting our feet wet with kernel modules development. 

## Learning objectives
At the end of this milestone, we will answer the following questions:
1. How does Linux allow for the extension of the operating system? Do we have to recompile and
   reinstall every time?
1. What skills do I need to brag to my friends that I can write kernel code?
1. How do processes talk to a kernel module? And how can we read and write to the module?

## Logistics
* **Starter code**: None. Wait what? Yup, you read it right. There is no starter code in this
project. You will write things from scratch! (Well that's a lit, but you will have to set up your
directory structure and create your files - you should be able to do this by now, right?)
* [Submission box](https://moodle.rose-hulman.edu/mod/assign/view.php?id=2708256)
* **Due date**: Monday March 29, 11:59 pm

## Developmental setups
There are two things I would like to bring to your attention before we get started. 

### Virtual machines
For this project, we will be doing our development using a Linux virtual machine. If you have not
set your up yet, then please do so ASAP. You can use whichever Linux flavor that works best for you. 

However, if you have been using your VM to also do your class assignments, then **PLEASE**
(**PLEASE** **PLEASE**^100) make sure you have a backup of your code (either somewhere else or
better yet, on a git repo). We will brick our virtual machine several times throughout the life of
this project, and data loss is possible. Therefore, make sure you have your code backed up
somewhere!

Virtual machines provide for a very useful feature: taking snapshots. You can, at one moment of
time, take a snapshot of your virtual machine and then, if things go bad, you can always recover
your machine's state from that snapshot (kinda like a context switch right?). I do not anticipate
that we will need to do so in this class, as most of our bugs should be resolved by a quick forced
reboot, it is however good to always have a backup, so I suggest you make use of this functionality. 

### Maintaining your code
Please make sure to create a **PRIVATE** repo of your source code for this project. I do not
recommend that you use the course repository for that, as you can quickly clog it up. Instead,
please create a new git repo for this project, and add your partner(s) and myself (my GitHub ID is
`nouredd2`) as collaborators. Adding me is optional, yet it allows me to take a look at your code
whenever you need me or would like to ask me questions. 

# Linux kernel modules
Linux is a monolithic kernel, i.e., the entirety of the kernel executes as a logical single
process and in a single address space. Which means that, in Linux, all of the illusionist, referee,
and glue roles occur in the same address space, which makes things runs faster and communicate much
more easily. This is in contrast to a microkernel where the functionality of the operating system is
broken down into several isolated processes (sometimes called servers). The microkernel approach
tends to sacrifice performance for the ability to isolate services, i.e., if the schedule crashes,
it can be rebooted without contaminating the memory space the file system. You can read more about
these two choices in the famous Tanenbaum-Torvlads debate
[here](https://en.wikipedia.org/wiki/Tanenbaum%E2%80%93Torvalds_debate) and
[here](https://www.oreilly.com/openbook/opensources/book/appa.html).

However, in order to also protect against the disadvantages of monolithic kernels, the Linux kernel
provides support _kernel threads_, the ability to preempt itself (called _kernel preemption_), and
a modular design that supports the loading and unloading of additional pieces of software, called
__modules__.

> (**Definition**) Kernel module: A piece of code than can be added to the kernel at runtime. 

## Why modules?
Imagine you are running a version of Linux and you are quite happy with it, but then you decide to
purchase a new gaming controller, or a new recording device (let's stick with the controller so I
don't have to write everything twice). Your controller a designed by a fancy new company and they
decided that their devices will adhere to a completely new communication protocol that they
invented, let's call it _WeKnowBetterProtocol_.
So you decide, to plug your controller in through USB, and then nothing happens! Well, the current
version of Linux you have does not speak _WeKnowBetterProtocol_, so your controller at this point is
pretty much useless. What do we fix that?

Well one thing we can do is to throw away our entire operating system, add to it support for
_WeKnowBetterProtocol_, recompiled it, and then install the new version. That doesn't sound like
fun, right? Fortunately, Linux provides with a very useful feature: _kernel modules_. The developer
of _WeKnowBetterProtocol_ can write a module for their controller that allows the kernel to
understand _WeKnowBetterProtocol_ and then magically, and at runtime, insert (or install) the module
into your current Linux kernel. Nifty isn't it?

The Linux kernel offers support for a range of types (or classes) of kernel modules, including, but
not limited to, device drives. You can even add a new filesystem as a kernel module! Well, maybe
next year!

![A split view of the kernel]({{ site.baseurl }}/docs/modules.png)

The figure above (obtained from [Linux Device Drivers, 3rd
Edition](https://lwn.net/Kernel/LDD3/)) shows a sample organization of the Linux kernel, including
tasks that are implemented as kernel modules. 

---

# Milestone 1
In this milestone, we will implement a kernel module that simply manages an array of integers. The
module will interact with user processes and allow them to insert entries into the array. It will
also allow users to read the content of the array, both in full and entry by entry. The module will
make any changes to the array persistent, i.e., if one process changes the element at index `i`,
then another process reading the array must see those changes (we will worry about concurrency
issues in milestone 2 -- exciting stuff!). We will allows processes to communicate with our kernel
module by using `procfs`, a pseudo-filesystem (i.e., does not really exist on disk, only in memory)
that allows us to treat our module as if it was a file we can read and write to!

## Baby: Hello World! kernel module
TBA

