---
title: Paging Activity
layout: post
readtime: true
date: Mon Dec 11 2023 
last-updated: Mon Dec 11 2023 
---

# Introduction

In this activity, we will explore some of the early stages of implementing
paging in xv6. Unfortunately, paging is dictated by the architecture (RISC-V) in
this case, so we cannot really change how it is structured, but nothing is
stopping us from playing around with it.

## Getting the Source Code

For this acitivity, we will need to mess around with the code for the xv6
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

Then, make sure that the branch `paging_act` shows up

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
```

Then, get the `paging_act` branch as follows:

```sh
$ git checkout -b paging_act_sol upstream/paging_act
branch 'paging_act_sol' set up to track 'upstream/paging_act'.
Switched to a new branch 'paging_act_sol'
```

Finally, push the empty stub to your own repo using:

```sh
$ git push --set-upstream origin paging_act_sol
Enumerating objects: 99, done.
Counting objects: 100% (99/99), done.
Delta compression using up to 56 threads
Compressing objects: 100% (80/80), done.
Writing objects: 100% (88/88), 3.15 MiB | 11.39 MiB/s, done.
Total 88 (delta 15), reused 19 (delta 2), pack-reused 0
remote: Resolving deltas: 100% (15/15), completed with 11 local objects.
remote:
remote: Create a pull request for 'paging_act_sol' on GitHub by visiting:
remote:      https://github.com/user/csse332-labs-user/pull/new/paging_act_sol
remote:
To github.com:user/csse332-labs-noureddi.git
 * [new branch]      paging_act_sol -> paging_act_sol
branch 'paging_act_sol' set up to track 'origin/paging_act_sol'.
```

# The Activity

What we would like to do in this activity is to examine the mappings from
virtual addresses to physical addresses in xv6. Normally, a process cannot
access its phyical address (think about why?) but we will expose those in this
activity so we could learn more.

The goal of this activity is to implement a system call that, given a virtual
address, returns the address of the physical frame of memory that corresponds to
the page that the address belongs to. The system call we are implementing is
called `getmaping` and it has the following signature:

```c
int getmaping(uint64 va, uint64 pa_addr);
```

where `va` is the virtual address we are looking up, and `pa_addr` is a pointer
to a pointer where we would like to store the physical frame address.

❓ Why do we have to do this level of indirection to write a value back to the
user?

To use it, the system, you could do something like:

```c
// the physical address we map to
uint64 pa;

// allocate some virtual address
char *p = malloc(4);

// make the system call, pa will contain the physical frame address
getmaping((uint64)p, (uint64)&pa);
```

To help you out, we have provided you with a userspace program that makes
several calls to the `getmaping` system call and prints out some valid and
invalid mappings. You can find the source code for the program under
`user/map.c`, you can run from the xv6 shell using

```shell
[xv6 shell] $ map 1024
############### CHILD (4) ###############
 Virtual Address                  Physical Address
------------------               -------------------
0x0000000000013C00               0x0000000000000000

Checking for invalid address 0x0000000000014C00 -->  0x0000000000000000
############### CHILD (4) ###############

############### PARENT (3) ###############
 Virtual Address                  Physical Address
------------------               -------------------
0x0000000000013C00               0x0000000000000000

Checking for invalid address 0x0000000000014C00 -->  0x0000000000000000
############### PARENT (3) ###############
```

Currently, all addresses will show up as zeros, since we have not implemented
the system call completely. Your job in this activity is to implement the system
call.

## Implementing `getmaping`

Systems calls are called from userspace, but they cause a trap into the kernel
so that the kernel can execute some service on behalf of the user process. We
have set everything up for you to add the `getmaping` system call. Your job is
to implement the functionality of translating the virtual address into a
physical address.

In `kernel/sysproc.c`, at the end of the file, you will find a function called
`sys_getmaping(void)`. This is the function that the kernel executes when the
user calls the `getmaping` system call.

Your job now is to add your code to line 104 so that you can translate the
virtual address `va` into its corresponding physical frame address `pa`.

_Hint 1_: To get a reference to the process that issues the system call, you can
use the `myproc()` call. It returns `struct proc *` pointer that represents the
PCB of the process that issued the system call.

_Hint 2_: You will find the functions defined in `kernel/vm.c` very helpful for
this activity.

_Hint 3_: Read the comments for those functions!

---

This page was last edited by Mohammad Noureddine on {{ page.last-updated }}. If
you notice any typos or inaccuracies, please open a GitHub issue on this
[repository]({{site.gh_repository_url}}).

