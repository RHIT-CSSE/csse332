---
layout: post
title: System Call Implementation Activity
readtime: true
date: 2024-12-15 20:06
last-updated: 2024-12-15 21:26
---

# Introduction

In this activity, you will be adding a new system call to the xv6 kernel to
facilitate the implementation of the miniature version of Unix utility `ps`.
`ps` is a utility that allows you to get information about the processes
currently running in your system (you can try it out on your own terminal
window and check its `man` page). However, process information usually require
inquiring with the operating system about the name, process id, and state of
each running process - thus we'd need to add a system call to support it
(that's not exactly how `ps` works in real-life, but we'll just assume so).

## Learning objectives

At the end of this activity, you should be able to:

1. Add a system call that would read process information and store it in a data
   structure.

2. Read arguments to a system call in xv6.

3. Have the kernel write to a user's piece of memory through a system call.

## Getting the source code

For this activity, we will need to mess around with the code for the xv6
kernel. So before you start this activity, please make sure that all of your
code for your labs is pushed to your own private repo, otherwise, we might
risk mixing things up and losing your progress.

### Backing up your lab code 

First commit and push all of your changes to your repo. You can do so using:

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

Then, make sure that the branch `syscall` shows up

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
  remotes/upstream/syscall
```

Then, get the `syscall` branch as follows:

```sh
$ git checkout -b syscall_sol upstream/syscall
branch 'syscall_sol' set up to track 'upstream/syscall'.
Switched to a new branch 'syscall_sol'
```

Finally, push the empty stub to your own repo using:

```sh
$ git push --set-upstream origin syscall_sol
Enumerating objects: 99, done.
Counting objects: 100% (99/99), done.
Delta compression using up to 56 threads
Compressing objects: 100% (80/80), done.
Writing objects: 100% (88/88), 3.15 MiB | 11.39 MiB/s, done.
Total 88 (delta 15), reused 19 (delta 2), pack-reused 0
remote: Resolving deltas: 100% (15/15), completed with 11 local objects.
remote:
remote: Create a pull request for 'syscall_sol' on GitHub by visiting:
remote:      https://github.com/user/csse332-labs-user/pull/new/syscall_sol
remote:
To github.com:user/csse332-labs-noureddi.git
 * [new branch]      syscall_sol -> syscall_sol
branch 'syscall_sol' set up to track 'origin/syscall_sol'.
```

# The Data Structure 

First, open the file `kernel/ps.h` and check out the `psstat` structure. I have
reproduced it here for reference:

```c
struct psstat {
  int pid;
  int ppid;

  int state;
  int pstate;

  char name[MAX_PROC_NAME];
  char pname[MAX_PROC_NAME];
};
```

This structure is the main data structure that we'd like the kernel to help us
fill out about a specific process. It contains the following fields:

1. `pid` and `ppid`: the process and the parent process's pids, respectively.

2. `state` and `pstate`: the process and the parent process' states,
   respectively.

3. `name` and `pname`: the process and the parent process' names, respectively.

Your task is then is to write a **system call** that given a process id, will
find the information needed and fill out a `struct psstat` for that process, if
any.

# Usage

Before we begin, check out `user/ps.c` for usage information of the `ps` system
call and `struct psstat` structure. Specifically, look for 56 which looks like
the following (with some definitions pulled in):

```c
struct psstat my_ps;
int pid;

/* ... */

if(ps(pid, &my_ps)) {
  printf("[ERROR] Problem running ps for pid = %d\n", pid);
  exit(1);
}
```

You can see that the `ps` system call accepts two arguments:

1. An `int` that is the process id to fetch information for.

2. A `struct psstat*` pointer to a `struct psstat` that the kernel will fill
   for us with the appropriate information.

Feel free to skim the code before and after the system call to see the `struct
psstat` in use.

## Compilation

Note that the code currently is set to ignore any use of the system call
because it is not yet implemented (thus all the `#ifdef` and `#ifndef` you
see). If you run it now, it will error out and do nothing.

Once you are done with the implementation, add the line:

```c
#define PS_SYSCALL_DONE 1
```

right after the `#include`s in `user/ps.c` and then recompile xv6.


# Adding the System Call

Let's start adding the infrastructure for the system call. There are many
places you should edit, so follow closely.

1. Add the system call's user API prototype. In `user/user.h`, you will need to
   add two things:

   - Add the line `struct psstat;` to the top of the file (right after the
     `struct stat` line). This is a forward declaration of the `psstat`
     structure since we are not including the file were it is defined here.

   - Add a prototype for the system call at the end of the system calls list:

     ```c
     int ps(int, struct psstat*);
     ```

2. Add a stub for the system call in `user/usys.pl` as follows:

   ```pl
   entry(ps);
   ```

3. Give the system call a number under `kernel/syscall.h` as follows:

   ```c
   #define SYS_ps 22
   ```

   Note that `SYS` **must be capitalized**.

4. Append your system call to the list of calls in `kernel/syscall.c` as follows:

   ```c
   static uint64 (*syscalls[])(void) = {
     /* ... Other stuff here */
     [SYS_ps]  sys_ps,
   };
   ```

    Note that `sys_ps` is **not** capitalized!

5. Also in `kernel/syscall.c`, add the signature of your system call to the
   list of externs:

   ```c
   extern uint64 sys_ps(void);
   ```

6. Next, move to `kernel/defs.h` and add the function call signature under the
   `proc.c` section of definitions (right around where `fork` and `exec` are):

   ```c
   int ps(int, uint64);
   ```

## Implementing the system call wrapper

All what we have done so far is to set up the following:

1. The user issues the `ps` system call (thus calls `ecall`) and a trap to the
   kernel occurs.

2. The kernel figures out that this is a system call trap, and then gets the
   system call number from the `a7` register.

3. The number in `a7` will correspond to `SYS_ps` (or 22) that we have defined
   earlier.

4. The kernel will lookup `SYS_ps` in the `syscalls` table and figure out it
   must call the `sys_ps` wrapper function.

We are now at the stage where we should implement the `sys_ps` wrapper, let's
go ahead and do that.

In `kernel/sysproc.c`, add the following function:

```c
uint64 sys_ps(void)
{
  // TODO: add you code here...
}
```

What this wrapper should do is the following:

1. Extract the arguments to the system call (where are they stored?).

2. Call the actual implementation of the system call with the correct
   arguments.

Usually, the wrapper is only there to parse arguments. Logic and functionality
is delegated to more specialized functions like the one we'll implement soon.

**Follow along with your instructor to complete this wrapper.**

## Implementing the `ps` logic

Finally, navigate to `kernel/proc.c` and implement the logic of fetching the
process information. Here is a skeleton with some helper functions for you:

```c
int
ps(int pid, uint64 addr)
{
  // get a pointer to the process that made the system call using myproc()
  struct proc *self = myproc();

  // a pointer to a process for iteration.
  struct proc *p;

  // the psstat struct to fill out
  struct psstat pinfo;

  // ind the process with pid = given pid
  // Use this loop to iterator over all processes
  for(p = proc; p < &proc[NPROC]; p++) {
    // skip over ones that are unused
    if(p->state == UNUSED)
      continue;

    // TODO:
    //   
    //  Step 1: Find the process who's pid is the input pid.
    //  Step 2: Fill out the information about the process into the pinfo
    //          structure.

    //  Hint: Recall that you cannot assign strings to copy them in C.
    //  Hint: You can use safestrcpy to copy one string into another.
    //        safestrcpy(destination, source, length);

    //  Step 3: We do this for you so don't change this code.
    //          We need to copy the pinfo structure we fill out back to the
    //          user. We need to take extra care because we are the kernel.
    if(copyout(self->pagetable, addr, (char*)&pinfo, sizeof pinfo) < 0) {
      printf("Woaaaaa something very terrible happened!\n");
      return -1;
    }

    //  Step 4: Return zero for success.

  }

  // Step 5: What happens if we go through the entire loop and reach this line?
  //         Should this ever happen?
}
```

## Testing

Once you are done with the implementation, you can use the `ps` utility we
wrote initially (under `user/ps.c`) to test it out.

First, make sure you enable the `ps` calls by adding the line `#define
PS_SYSCALL_DONE 1` right after the `#include`s in `user/ps.c`.

Then compile `xv6` using `make qemu` and once the shell drops, try to issue the
`ps` command. Without any arguments, `ps` will pass the ID of the current
process (i.e., the pid of `ps` itself) as the argument to the system call.

```shell
$ ps
***** Looking for process information for pid = 4
Process ps_test (4)
================================================
Process id:    4
Process name:  ps
Process state: RUNNING

Parent process id:    2
Parent process name:  sh
Parent process state: SLEEPING
================================================
```

We can see that the process `ps` has pid 4, it is currently `RUNNING`, and its
parent is `sh` (the shell).

Let's investigate the shell as well:

```shell
$ ps 2
***** Looking for process information for pid = 2
Process sh (2)
================================================
Process id:    2
Process name:  sh
Process state: SLEEPING

Parent process id:    1
Parent process name:  init
Parent process state: SLEEPING
================================================
```

We can see the `sh` is `SLEEPING` (think about why?) and that its parent is non
other than `init`.

## Fixing `init`

Now if you try to get information about `init`, you will see that things break
with a kernel panic. Consider it your last task to fix `ps` to work for `init`
(i.e., the process with pid 1) as well. The first question you ought to ask
yourself, what's unique about `init` that makes it so different that any other
process? And why does that break the current version of `ps`?

