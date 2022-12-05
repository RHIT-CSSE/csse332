---
layout: post
title: Operations on Processes
readtime: true
date: Wed Nov 30 22:25:14 2022 
---

# Learning Objectives

At the end of this lecture, you should be able to:
- Identify how a process is created using `fork` and `exec` family of calls.
- Implement code that uses `fork` and `wait` to create a child process.
- Define the genealogical relationship between all processes in a running
  operating system. 

---

# Topics

In this lecture, we will cover the following topics:
- Process creation using `fork` and `exec`.
- The `wait` routine. 
- Patterns for creating child processes. 

---

# Notes


<!-- vim-markdown-toc GFM -->

* [Introduction](#introduction)
* [Creating a child process using `fork`](#creating-a-child-process-using-fork)
  * [`fork` returning](#fork-returning)
* [Being a good parent using `wait`](#being-a-good-parent-using-wait)
  * [Example](#example)
  * [Orphans](#orphans)
  * [Zombies](#zombies)
* [Replacing a child using `exec`](#replacing-a-child-using-exec)
  * [The versions of `exec`](#the-versions-of-exec)
  * [The line after `exec`](#the-line-after-exec)

<!-- vim-markdown-toc -->

# Introduction

- Imagine you are in a spot where you have so much to do, so many chores, and so
  much homework.
  - ❓ What would be a great (even if unrealistic) solution to this
    problem?
  - Does this picture help ring any bells?
  
    ![meeseeks](./meeseeks.png)

- What if you had the ability to create clones of yourself, with all your
  knowledge and abilities, and then task those clones with specific tasks to do?
- That would be amazing, right?
- But what if, you want those clones to have another set of knowledge and
  skills, maybe give them some autonomy?
  - What would you want to do with those clones?
- ❓ Now that we can do all of that, let's be a bit pessimists, what can
  go wrong with those clones?

# Creating a child process using `fork`

- Recall that each process has a unique ID that we refer to as the PID.
  - You can use the `getpid()` call to obtain a process's PID.
    ```c
    pid_t pid = getpid();
    ```
- `fork` is a system call that allows a user process to create a copy of itself.
  - The calling process is referred to as the __parent__.
  - The created process is referred to as the __child__.
  - For documentation, from you terminal, you can use `man fork`.

- All processes in your system are children (or grandchildren) of the same
  initialization process.
  - That process is often referred to as `init` (or in Ubuntu, it is `systemd`).

- Here's an example of running the `pstree` command on my system:
```shell
$ pstree --color=age
systemd─┬─ModemManager───2*[{ModemManager}]
            ├─NetworkManager───2*[{NetworkManager}]
            ├─accounts-daemon───2*[{accounts-daemon}]
            ├─agetty
            ├─atd
            ├─2*[cpptools-srv───11*[{cpptools-srv}]]
            ├─cron
            ├─dbus-daemon
            ├─fwupd───4*[{fwupd}]
            ├─in.tftpd
            ├─irqbalance───{irqbalance}
            ├─multipathd───6*[{multipathd}]
            ├─networkd-dispat
            ├─nrpe
            ├─oddjobd
            ├─polkitd───2*[{polkitd}]
            ├─rsyslogd───3*[{rsyslogd}]
            ├─snapd───15*[{snapd}]
            ├─sshd─┬─sshd───sshd───zsh
            │          ├─sshd───sshd───bash─┬─sh───node─┬─node───10*[{node}]
            │          │                                │                   ├─node─┬─cpptools───24*[{cpptools}]
            │          │                                │                   │          ├─node───7*[{node}]
            │          │                                │                   │          ├─python3───{python3}
            │          │                                │                   │          └─11*[{node}]
            │          │                                │                   ├─node───12*[{node}]
            │          │                                │                   └─10*[{node}]
            │          │                                └─sleep
            │          ├─sshd───sshd───bash───sleep
            │          └─sshd───sshd───zsh───pstree
            ├─sssd─┬─sssd_be
            │          ├─sssd_nss
            │          └─sssd_pam
            ├─systemd─┬─(sd-pam)
            │             ├─dbus-daemon
            │             └─gnome-keyring-d───3*[{gnome-keyring-d}]
            ├─systemd-journal
            ├─systemd-logind
            ├─systemd-network
            ├─systemd-resolve
            ├─systemd-timesyn───{systemd-timesyn}
            ├─systemd-udevd
            ├─thermald───{thermald}
            ├─tmux: server───12*[zsh]
            ├─udisksd───4*[{udisksd}]
            ├─unattended-upgr───{unattended-upgr}
            ├─upowerd───2*[{upowerd}]
            └─wpa_supplicant
```

- `fork` creates a clone, or a __copy__, of the parent process.
  - In other words, the child process contains a snapshot of the parent
    process's state. 
- The child process will duplicate its parent's memory address space, which
  means that the child process contains copies of 
  - all variables,
  - all structures,
  - all global variables,
  - all pointers,
  - all dynamically allocated memory, etc.
- The child process will also contain its own copy of its parent's file
  descriptors, __however__ these file descriptors reference the same underlying
  objects. 
  - In other words, if you open a file in the parent using `fd = open(...)` and
    then fork a child process, then the child process will contain a copy of
    `fd` but it points to the same file.
    - So if the child changes the file content, the parent will also see those
      changes. 

## `fork` returning

- You must keep in mind that `fork` virtually _returns twice_:
  1. Once in the parent process, with a return value __that is equal to the
     child's PID__.
  2. Once in the child process, with a return value of 0.

- ❓ So how would we differentiate between the code that is in the
  parent and that is in the child?
  - Remember that we have copied the entire `.text` section of the process's
    code, so the code has to be exactly the same.
    ```c
    int pid = fork();
    if(pid == 0) {
      printf("I am the child\n");
      exit(0);
    }

    printf("I am the parent\n");
    exit(0);
    ```

- 🏃 What does this code do?
    ```c
    for(int i=0; i < 10; i++)
      fork();
    ```

# Being a good parent using `wait`

- Forking a child is very much like giving birth to a child and then letting
  them go to play in the park.
- ❓ That is great and all, but as a good parent, what do you need to
  do?

- For a parent to reclaim its child, it needs to call the `wait(int *)` system
  call.
  - As with all relevant function, you can check `man 2 wait` for more
    information.

- Here's the breakdown of the `wait(&status)` call:
  1. The parent will stop execution and wait until the child is done executing.
  2. The parent will store the child's exit status in the `status` pointer
     passed to the `wait` system call.
     - The `status` integer will then contain the child's exit status (i.e., the
       one passed to the `exit` call or through main's `return` statement).

## Example

```c
int status = -1;
pid_t pid;

pid = fork();
if(pid == 0) {
	printf("I am the child and I am done playing\n");
	exit(5);
} else {
	printf("I am the parent and I am waiting for my child\n");
	/* This will block until the child returns */
	wait(&status);
  /* Use the WEXITSTATUS macro to read the child's return code. */
	printf("The child is back and it returned %d\n", WEXITSTATUS(status));
}
```

## Orphans

- Unfortunately, not all parents are great.
- Sometimes, a parent might return (or maybe crash) without waiting for its
  children.
- In that case, the children are referred to as _orphan_ children.
- Luckily, the operating system will adopt all orphan children and they will
  become direct children of `init` (or `systemd`).

## Zombies

- Sometimes, parents might get busy and forget to wait for their children.
- In that case, if a child is done playing but its parent is not waiting for it,
  that child is referred to as a __zombie__ child.
- In OS terminology, a zombie process is one that is done (and is ready to die),
  but cannot really die because it still has some information to pass to its
  parent.
  - Thus the name zombie.
  - The process would have terminated but it would still occupy OS resources
    until its parent waits for it.

# Replacing a child using `exec`

- Sometimes, we want our children not to be exact copies of their parents.
- We would like to replace a child's code section with another.
- To achieve that, we can use the `exec` family of system calls.
- `exec` replaces the process with a completely different process with a
  different address space.
  - Note that open file descriptors are preserved in the new process.
  - Becomes relevant when we talk about `pipe`.

## The versions of `exec`

- There are two main versions of the `exec` call that we care about in this
  class:
  - `execv` where `v` stands for vector.
  - `execl` where `l` stands for ellipsis, which means a variable number of
    arguments.

- The syntax that corresponds to each is as follows:
  - `execv(<absolute path>, array of arguments, NULL);`
  - `execl(<absolute path>, argument0, argument1, argument2, ..., NULL);`

- Another variant that is sometimes more useful:
  - `execvp(<relative path>, array of arguments, NULL);`
  - `execlp(<relative path>, argument0, argument1, argument2, ..., NULL);`

- Remember that in both cases, the first argument in the vector (or list) of
  arguments is __always__ the name of the program being executed. 

## The line after `exec`

- One important thing to note about the `exec` call, is that it __does not
  return__, except when there is an error in the call.
  - In other words, if `exec` returns, then something bad has happened.

