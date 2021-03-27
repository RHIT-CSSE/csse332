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
let's put that the side for now, and focus on getting our feet wet with kernel modules development. 

## Learning objectives
At the end of this milestone, we will answer the following questions:
1. How does Linux allow for the extension of the operating system? Do we have to recompile and
   reinstall every time?
1. What skills do I need to brag to my friends that I can write kernel code?
1. How do processes talk to a kernel module? And how can we read and write to the module?

## Logistics
* **Starter code**: None. Wait what? Yup, you read it right. There is no starter code in this
project. You will write things from scratch! (Well that's a lie, but you will have to set up your
directory structure and create your files - you should be able to do this by now, right?)
* [Submission box](https://moodle.rose-hulman.edu/mod/assign/view.php?id=2708256)
* **Due date**: Monday March 29, 11:59 pm

## Developmental setups
There are two things I would like to bring to your attention before we get started. 

### Virtual machines
For this project, we will be doing our development using a Linux virtual machine. If you have not
set yours up yet, then please do so ASAP. You can use whichever Linux flavor that works best for you. 

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
these two choices in the famous Tanenbaum-Torvalds debate
[here](https://en.wikipedia.org/wiki/Tanenbaum%E2%80%93Torvalds_debate) and
[here](https://www.oreilly.com/openbook/opensources/book/appa.html).

However, in order to also protect against the disadvantages of monolithic kernels, the Linux kernel
provides support for _kernel threads_, the ability to preempt itself (called _kernel preemption_), and
a modular design that supports the loading and unloading of additional pieces of software, called
__modules__.

> (**Definition**) Kernel module: A piece of code than can be added to the kernel at runtime. 

## Why modules?
Imagine you are running a version of Linux and you are quite happy with it, but then you decide to
purchase a new gaming controller, or a new recording device (let's stick with the controller so I
don't have to write everything twice). Your controller is designed by a fancy new company and they
decided that their devices will adhere to a completely new communication protocol that they
invented, let's call it _WeKnowBetterProtocol_.
So you decide to plug your controller in through USB, and then nothing happens! Well, the current
version of Linux you have does not speak _WeKnowBetterProtocol_, so your controller at this point is
pretty much useless. How do we fix that?

Well one thing we can do is to throw away our entire operating system, add to it support for
_WeKnowBetterProtocol_, recompile it, and then install the new version. That doesn't sound like
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
Okay, time to flex our kernel coding muscles and get busy coding our first kernel module. This will
simply be a module that  prints `Module csse332 initializing...` upon entry, and `Module csse332
exiting...` upon exit. Entry? Exit? Printing? What do those mean?

### Anatomy of a module
Our operating system kernel is already running, so what does it mean to add another piece of code to
it dynamically? Well, the kernel is well-equipped to handle runtime addition of code. To support
that, it allows developers to insert modules into the kernel using the `insmod` command. Once a
module is inserted into the kernel, the kernel will run some initialization code, and using the
concept of [callbacks](https://www.geeksforgeeks.org/callbacks-in-c/), the kernel can ask the module
if it wants to run some of its own initialization code. 

So, no `main` function. You just need to point the kernel to what you would like your module to do
upon initialization, and the kernel will make sure that executes for you!

Similarly, the kernel allows you to remove modules. Once is a module is set to be removed, the
kernel will ask the module if it likes to run any cleanup code. All you have to do again is to point the
kernel to which function you'd like to execute when exiting, and the kernel will call that for you.
Nice isn't it?

### Printing in the kernel
We mentioned printing, but where does the kernel print? Does it use the same `printf` functions we
use when writing user applications. Well, not exactly. The kernel writes to its own "files" and have
specific commands that allow you to see those printed messages. However, since the kernel must be
running with performance in mind, it tends to rate-limit your print messages so that you don't end
up blocking the entire kernel while waiting for I/O operations to finish. 

Luckily, the Linux kernel is well-equipped to allow us to log important messages. To that extent,
the kernel provides us with the function `printk` (k standing for kernel, genius right?). `printk`
messages are printed to a kernel log buffer, that can be accessed from userspace by reading
`/dev/kmsg`. Also, luckily, we can use the `dmesg` utility to see the kernel's log messages on our
console. 

However, unlike `printf`, `printk` requires us to provide a logging level. For example, printing at
the info level can be done using
```
   printk(KERN_INFO "My message is %s\n", "Hello world!);
 ```
The kernel has several log levels (`KERN_EMERG`, `KERN_ALERT`, `KERN_CRIT`, etc.), and depending on
how you configure your logging levels, not all messages will be printed. For example, if I choose to
print at the debug-level using `KERN_DEBUG`, the kernel will by default ignore those messages since
it is not configured to print debug information when it is in release mode. In this project, we will
mostly rely on info-level messages since we are writing a small light-weight module. However, be
aware that if you use `printk` inside of a loop, the kernel might limit your printing to avoid
blocking the kernel for I/O operations. 

I bet you are wondering now if we have to write this long and ugly line every time we need to print
a statement? Well, luckily not! The kernel developers have kindly provided us with a set of aliases
that are much more elegant. For example, to print at the info-level, we can simply use the `pr_info`
alias, which simply calls the appropriate `printk` message for us!

Even better, we can customize the output of `pr_info` to print a prefix message that will allow us
to identify that it is our module who's printing. That will be very helpful when looking at our
debug messages so that we can easily find them in the bulk of messages that the other kernel
services print. 

### Time to get started
Let's start by creating a file to start writing our code. You can call it whatever you like. In the
rest of this section, I assume the name of the file is `project.c`.

Alright, with that being said, let's write our first module. First, let's ask the kernel to prepend
our module's name to every print message. To do that, put this line at the top of your source code
```
#define pr_fmt(fmt) "[csse332]: " fmt
```
This will tell the kernel the prepend `[csse332]: ` to every message that we print from our module. 

Okay, time to include the files we care about. In this section, I will provide you with the files
you need to include, but after that, you're on your own. So let's go ahead and include the following
files
```
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h>
```
Pretty much self explanatory right? **NOTE** that you do not have access to the standard `libc`
libraries (like `stdio.h` and `string.h` and so on). You can only work with what the kernel provides
you with (there might be some caveats, but let's ignore those for now). 

Okay, now let's give some descriptive information of our kernel. We will set its description, its
author, and its license as follows:
```
MODULE_LICENSE("GPL");
MODULE_AUTHOR("YOUR NAMES GO HERE");
MODULE_DESCRIPTION("DESCRIBE WHAT YOUR MODULE DOES HERE!");
```

Awesome! So now that we have gotten bookkeeping out of the way, let's get to the meat of this. First
let's write our initialization and exit functions. For our initialization function, let's write it
as follows
```
static int
__init csse_init(void)
{
    pr_info("Module csse332 initializing...\n");
    /* return a non zero value if failure occurs */
    return 0;
}
```
You can see that we used `static` and `__init` in that function's signature. What do those mean?
`static` tells our compiler that the scope of this function is only to be kept in this file (i.e.,
we do not want to export it to other files or scopes). The `__init` attribute informs our compiler
that this function shall only be run once when the kernel loads the module, therefore it can free up
the space occupied by the function after the first time the kernel loads it. 

Similarly, let's write our exit function:
```
static void
__exit csse_exit(void)
{
    pr_info("Module csse332 exiting...\n");
    /* Do exit stuff here... */
}
```
You can see that this is a mirror image of the initialization function except the fact that it
returns nothing (i.e., `void`). Note that it is important that your initialization function to
return 0 ONLY when the initialization is successful. If you do not return an error code when an
error is encountered, you might risk polluting your kernel's memory and crashing your machine. 

Okay, so we have written our initialization and exit functions, the only thing we have left is to
inform the kernel of them. To do so, we can use the `module_init` and `module_exit` macros as
follows
```
 module_init(csse_init);
 module_exit(csse_exit);
 ```

Great, that's it! That's your first module. Congratulations!!!

#### Compiling your module
Now that we have written our first module, we need to compile it so that we can insert it into our
kernel. To do so, we will again call on the help of the `make` tool by writing a `Makefile`, which
will look as follows:
```
CONFIG_MODULE_SIG=n

obj-m += project.o

all:
  make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
  make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
```
That's it! The first line turns off signature verification for our module. This is a security
feature to verify that the module is indeed written by those who claim they have written it (Imagine
you can get a driver that you assume to be from NVIDIA but turns out to be from
SUPER\_NASTY\_HACKER, that would be bad if you plug that code into your kernel wouldn't it be?).
Luckily, we don't have to worry about this in this class. 

The line `obj-m += project.o` is where we tell `make` to compile our `project.c` file into its
corresponding object file (make sure you adjust this if you chose a different one). Then the last
two lines make use of the makefiles located in your own Linux distribution to perform the correct
build steps. There are more options we can specify, but for this class, that's all we need to do!

If your compilation fails due to missing files, make sure you install the Linux kernel heads for
your kernel version. On Ubuntu, you can do so using
```
sudo apt install -y linux-headers-$(uname -r)
```

#### Testing your module
Alright, now go ahead and build your module using
```
make
```
If all goes well, you shouldn't see any error messages showing up. There will be a bunch of files
created, the only one we care about is the file `project.ko`, that is our module that we would like to
insert into the kernel. 

Next, let's insert our module into the kernel. To do so, use the command
```
sudo insmod project.ko
```
If all goes well, you shouldn't see anything popping up. So how do we confirm that everything is
going well? Now comes the time to play some tricks. 

First, let's confirm that the module has been inserted. To do so, use the following command
```
lsmod | grep project
```
You should see your module pop out along with some information that we're not going to worry about
for now, so something that looks like the following
```
project                16384  0
```
Great so we know our module has been inserted, let's check if we printed our hello message. To do
so, let's use `dmesg` as follows:
```
dmesg 
```
That will dump everything that the kernel has written so far. It's a long list, so can we
manipulate it a bit more? Of course we can! Recall that little message we prepended  to the start of
all our kernel printouts? Let's ask `dmesg` to only show kernel messages that start with that
prefix. To do so, use
```
dmesg | grep csse332
```
Do not use the brackets here, as that will cause your command to fail (to see why, lookup regular
expressions!). If all goes well, you should see something that looks as follows
```
[   50.644180] [csse332]: Module csse 332 initializing...
```

Now, let's remove our module and make sure we are printing the exit message. To do so, use 
```
sudo rmmod project
```
And let's confirm again using `dmesg`
```
dmesg | grep csse332
```
and you should see something that looks as follows:
```
[  359.817310] [csse332]: Module csse 332 exiting...
```

Congratulations! You have inserted your first kernel module and are well on your way to become a
kernel hacker!

# Toddler: Step 1: Memory allocation
In the first step of this milestone, we would like to create an array of integers of size 10. Your
module should allocate memory in the kernel for this array, and then free it up when it exits
(either normally or because of an error). 

## Memory management
To create memory in user applications, we used the `malloc` function. Is there something similar to
that in the kernel? Of course! And unsurprisingly, the function is called `kmalloc`! However, since
kernel memory is managed differently than user memory, we need to tell the kernel what kind of
memory we're allocating and in which context. Therefore, we will use
```
kmalloc(size, GFP_KERNEL)
```
The `size` parameter behaves in the same way as the normal `malloc` parameter. The second parameter
we're passing, `GFP_KERNEL`, tells the kernel that the memory allocated is to be used in kernel
space, in the kernel context. You can check more information about the possible flags to `kmalloc`
[here](https://www.kernel.org/doc/htmldocs/kernel-api/API-kmalloc.html).

Your job in this step is to make sure that the memory is allocated when initializing the module and
then it is freed when the module is exiting (or later on, when any errors occur during
initialization). 

To make sure that everything went smoothly, try printing out a few elements of the array during
initialization to make sure that your module is behaving correctly. As always, use
* `isnmod` to insert your module,
* `lsmod` to check if your module is inserted,
* `dmesg` to read kernel messages, and
* `rmmod` to remove your module. 

# Preschool: Step 2: Your module learns to talk 
So our module now maintains an array of integers as a data structure. Great! But what use is this
data structure for us if we cannot read and write to it? In this step, we will need to allow users
to communicate with our module using the `procfs` pseudo filesystem. 

## Create a directory entry for our module
First, we need to create an entry for our module in `procfs`. Our goal is to create an entry for
our module under `/proc/csse332/status`. You can think of it as a file to and from which our user
applications can write and read. Note that this is all managed in software! Nothing that you will do
will be written or read from disk!

First, we need to create the parent directory `csse332` for our module in `procfs`. To do so, you
can use the `proc_mkdir` call as follows:
```
parent_entry = proc_mkdir("csse332", NULL);
/* do some error checking on parent entry here and check that all is good.
 * if not, make sure you do some appropriate cleanup!
 */
```
Your job is to answer the following questions:
1. Where should this piece of code go in your code?
1. What is the return type of `proc_mkdir`? And where do I declare the variable that will hold that
   entry?
1. What should you do if the creation of the entry fails? (_hint_: Recall that if have allocated
   anything, it must be free appropriately!)
1. Finally, when should we cleanup and remove our entry?

Note that the final question is very important. If you fail to cleanup after yourself (i.e., remove
your created directory when needed, you won't be able to insert your module again). So go back to
the drawing board and answer the above questions before you start writing your code. 

To test that your code is working correctly, insert your module using `insmod` and then check that
there a directory created under `/proc`. You can simply use 
```
ls /proc
```
to see all files and directories under `procfs`. 

## Create a file for our module
Awesome, so we have created our directory, now it's time to create our pseudo-file under our
directory. To do so, we will use the `proc_create` call. We would like to create an entry called
`status` under the `csse332` directory we created in the previous step. So the path to our entry
would be `proc/csse332/status`. **Please make sure you stick to the same naming convention as this
document, otherwise, it will break our grading setup.** Here is the signature of the function
```
struct proc_dir_entry *proc_create(
  const char *name, umod_t mode, struct proc_dir_entry *parent,
  const struct file_operations *proc_fops);
```
where
* `name` is the name of your entry to create
* `mode` are your entry's permissions (i.e., who can read/write/execute it). For this project, we
will allow users to read and write to our entry, so use `0666` as your permissions! And no we're not
summoning Satan, at least not yet!
* `parent` is the parent directory of our entry, which is the directory we created previously!
* `proc_fops` are the operations that your module would support (such as reading and writing).


So the first parameters sound reasonable, but what the heck is the last one, `proc_fops`? Where do we
get that one from. Well, you're going to have to create it! That is the crux of this assignment. The
`struct file_operations` structure is simply a structure of function pointers that you must set so
that your users can read and write to the `procfs` entry for your module. You can find more
information about the content of this structure
[here](https://tldp.org/LDP/lkmpg/2.4/html/c577.htm). Note that you do NOT have to implement all of
the functions for this entry. In this step, we will focus on the `read` and `write` callbacks. Any
time a user tries to read from your `procfs` entry, the operating system will magically figure out
that it must call your `read` function. Similarly, any time a user writes to your `procfs` entry,
the OS will magically figure out that is must call your `write` function. 

To help you get started, I have provided you with empty stubs that allow you to successfully create
your `procfs` entry and test it out. 

**NOTE**: It appears that as of Linux version 5.6, the signature of the above function has changed
to accept a `const struct proc_ops *proc_fops` instead of the `const struct file_operations
*proc_fops` that we show above. To handle this issue, first check what your kernel version is. To do
so, use the command
```
uname -r
```

#### If you version number is < 5.6, then use the code below:

```
/**
 * csse332_read - Read handler for milestone 1 of the term project
 *
 * @filp  The file pointer to the current file we are reading
 * @buff  The userspace buffer that we must fill out to the user
 * @count The number of bytes we can write to the user (i.e., size of buff)
 * @offp  The offset into the file we are reading
 *
 * @return 0 if there is nothing left to read from the entry, otherwise, return number
 *  of bytes read so far. On error, return appropriate error code (negative number)
 */
static ssize_t
csse332_read(struct file *filp, char __user *buff,
    size_t count, loff_t *offp)
{
        /* Your read code goes here ... */
        return 0; // please don't remove this line until you are ready to write your own!
}

/**
 * csse332_write - Write handler for milestone 1 of the term project
 *
 * @filp  The file pointer to the current file we are reading
 * @buff  The userspace buffer that we must copy from the user
 * @count The number of bytes we can read from the user (i.e., size of buff)
 * @offp  The offset into the file we are writing to 
 *
 * @return number of bytes written, 0 if none, and a negative error code on failure. 
 */
static ssize_t
csse332_write(struct file *filp, char __user *buff,
    size_t count, loff_t *offp)
{
        /* Your write code goes here ... */
        return count; // please don't remove this line until you are ready to write your own!
}

static const struct file_operations csse332_fops = {
        .owner = THIS_MODULE,
        .read  = csse332_read,
        .write = csse332_write,
};
```

#### If your version is > 5.6, then use the code below:
```
/**
 * csse332_read - Read handler for milestone 1 of the term project
 *
 * @filp  The file pointer to the current file we are reading
 * @buff  The userspace buffer that we must fill out to the user
 * @count The number of bytes we can write to the user (i.e., size of buff)
 * @offp  The offset into the file we are reading
 *
 * @return 0 if there is nothing left to read from the entry, otherwise, return number
 *  of bytes read so far. On error, return appropriate error code (negative number)
 */
static ssize_t
csse332_read(struct file *filp, char __user *buff,
    size_t count, loff_t *offp)
{
        /* Your read code goes here ... */
        return 0; // please don't remove this line until you are ready to write your own!
}

/**
 * csse332_write - Write handler for milestone 1 of the term project
 *
 * @filp  The file pointer to the current file we are reading
 * @buff  The userspace buffer that we must copy from the user
 * @count The number of bytes we can read from the user (i.e., size of buff)
 * @offp  The offset into the file we are writing to 
 *
 * @return number of bytes written, 0 if none, and a negative error code on failure. 
 */
static ssize_t
csse332_write(struct file *filp, const char __user *buff,
    size_t count, loff_t *offp)
{
        /* Your write code goes here ... */
        return count; // please don't remove this line until you are ready to write your own!
}

static const struct proc_ops csse332_fops = {
        .read  = csse332_read,
        .write = csse332_write,
};
```


Now you know that you can create your `procfs` entry by passing the address of `csse332_fops` as the
last parameter to your `proc_create` function. 

Finally, recall that anything you create or allocate, you must destroy or free. So make sure that
you appropriate delete your `procfs` entry and directory before your module exits. Again, failure to
do will stop you from inserting your module again for testing!

Go ahead and create the entry, then verify that you can read and write to it (Of course, nothing
will happen at this point, but make sure that no errors show up). After inserting your module, you
can test the `write` handler using
```
echo "hello" > /dev/csse332/status
```
and you can test your `read` handler using
```
cat /dev/csse332/status
```
If no errors show up, you are good to go to the next step!

## Learning to write: The write handler
First let's busy ourselves with the `write` handler. In this milestone, we would want the user to
give us two parameters as a command: (1) the index at which we would want to write into our array,
and (2) the value they would like to write at the position. Your job is to parse the user's command
and then write the value that the user requested at the specific index. Of course, if the user sends
an invalid index, you must notify them appropriately (by returning an appropriate error code).

To make things easier, you may assume that the user always send in valid commands. In other words,
all inputs from the user is simply two space-separated numbers, the first of which is the index they
wish to write at, and the second is the value they would like to write to the array. Assume that all
commands from the user are correctly formatted. 

But before we start writing code, did you see that `__user` attribute before the `buff` parameters
to the `csse332_write` function? What does that mean?

Simply, this means that the memory area that `buff` occupies is in user land. But where is your
module running? Right, in kernel space! So can you module directly access memory from the user
space? Heck no! All hell would break loose if you do (try it! bad things will happen, and it will be
ugly -- well I exaggerate a little, but **NEVER** read or write directly to user land from the kernel). 
Okay, so what do we do? Well, luckily, the kernel provides us with a mechanism to copy data from the
user space to the kernel space. That function is surprisingly called, `copy_from_user` (you can
check out its documentation
[here](https://www.cs.bham.ac.uk/~exr/teaching/lectures/opsys/13_14/docs/kernelAPI/r4081.html)).
Therefore, before you do anything, you must first copy the message from the user space to the kernel
space appropriately. 

Here are the steps I suggest you do when implementing this step:
1. Allocate memory in kernel space to hold the user's message.
1. Using `copy_from_user`, copy the data from the user buffer to your kernel buffer.
   Make sure you do appropriate error checking and return error codes if your code fails. 
   > Note: Recall that C strings should always be terminated with a `\0` character. So make sure to make
   enough room in your kernel buffer to hold that character as well. `copy_from_user` is not
   guaranteed to copy that character for you so make sure you add it manually. 

   > Note 2: you might find it useful to remove any `\n` characters from your kernel buffer, as it
   can simplify parsing the command. 
1. Parse the user's command. You might find the function `sscanf` from `linux/string.h` useful for
   you. 
1. After parsing the command from the user, you will end up with two integers, the index and the
   value to write. 
1. Check that the index that the user passed to you is valid (i.e., is not negative and is not out
   of bounds). If the index is not valid, return an appropriate error code to the user and cleanup
   after yourself. You can find a list of useful error code
   [here](https://www.kernel.org/doc/html/v4.15/media/uapi/gen-errors.html). Note that you must
   always return a negative integer on error, i.e., you must use `return -ENOMEM` to indicate
   insufficient memory. 
1. If the index is valid, write the user's value at that specific index into the array of integers. 
1. Exit from the function after cleanup everything up. Make sure you do appropriate cleanup every
   single time, a single leaky pointer or buffer overflow can crash your machine :( 

You do not need to worry about the offset `offp` in writing, you will take care of that in the
`read` handler in the next step!

Test your code by inserting your module into the kernel and then trying to write to it. For example,
to write the value 10 at index 5, use
```
echo "5 10" > /dev/csse332/status
```
Then check `dmesg` to see if your code is behaving correctly. Also, make sure to test for out of
bounds input commands and make sure your code is correctly handling those!

## Learning to read: The read handler
Finally, it is time to teach our module to read (it is actually the user who is reading from the
module's proc entry, but let's just use the same terminology so we don't get confused). When the
user tries to read your module's `procfs` entry, your module should return the content of the
integer array stored in the module. However, it **MUST** return the elements one entry at a time. In
other words, as long as there are elements in the array, the user should keep asking you to read
until your return 0  (Recall that the `read` handler returns 0 **only** where there are no more
things to read from your module's `procfs` entry). 

But how do we know which entry did the user read last? Well this is where the `offp` pointer comes
in handy. In this project, we will not allow the user to skip entries when reading, so we can always
assume that the first time the user calls `read`, the offset is 0. (note that `loff_t` is nothing
but a `long long int` in most modern machines). 

After opening your `procfs` entry, the `offp` pointer will persist across multiple read operations
(again, we will not allow users to change that, though in real life, they can using the `seek`
calls, but let's ignore those for now). So think about how you can use the `offp` pointer to track
how many entries has the user read so far, and then to figure out when you should return 0 to
indicate that there are no more elements to read. Could `offp` serves as a pointer to the index you
are currently reading from? 

Similarly to the `write` handler, it is not a good idea to directly write to the user's buffer.
Instead, we will make use of `copy_to_user` to copy data from the kernel space to the user space. 
Here's how I would suggest your approach this step:
1. Allocate space in the kernel's memory to hold your message
1. Try to write a simple "Hello from module read" into that kernel buffer
1. Copy the kernel buffer to the user space buffer using `copy_to_user` and check for any errors
   (check if the user's buffer has enough room for your message, i.e., if the `count` parameter
   indicates that the user has enough room to hold your message). 
1. Return 0 to tell the user to stop reading
1. Test your code using `cat /dev/csse332/status` and make sure you can see the message from your
   module. 

Once you have the above working, now is the time to implement your array reading logic. Note that
`cat` will keep reading from your `procfs` entry until you return 0, so using `cat` to test the
correctness is not going to cut it for this step. You can use the user code below the test your
code:
```
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int
main(int argc, char **argv)
{
        int fd, i;
        size_t nbytes;
        char buffer[1000];

        fd = open("/proc/csse332/status", O_RDONLY);
        if (fd < 0) {
                fprintf(stderr, "Could not open your the procfs entry for reading: %s\n",
                        strerror(errno));
                exit(EXIT_FAILURE);
        }

        for (i=0;i<12;++i) {
                nbytes = read(fd, buffer, 1000);
                if (nbytes > 0) {
                        printf("Read %lu bytes from procfs entry\n", nbytes);
                        printf("%s", buffer);
                } else {
                        printf("No more stuff to read at index %d\n", i);
                }
        }
}
```
You can change the number of iterations in the loop to make sure that you are reading only the
correct number of entries, and not all at once!


# Rubric

| Part                                                                                   | Point Value |
|:---------------------------------------------------------------------------------------|-------------|
| Kernel module load successfully (prints hello message)                                 |     5       |
| Kernel module exits successfully (prints exit message)                                 |     5       |
| Allocate/free memory when appropriate                                                  |    20       |
| Create `/proc/csse332/status` directory                                                |    10       |
| Module write handler                                                                   |    40       |
| Module read handler                                                                    |    40       |
