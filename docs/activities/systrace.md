---
layout: post
title: Syscall Tracing Activity
readtime: true
date: Sat Jan 06 2024
last-updated: Sat Jan 06 2024
---

# Introduction

The goal of this activity is to trace the system calls that are being executed
in the xv6 system. Whenever a system call is initiated, we would like to print
the process id and the name of the process that initiated the call, the address
of the `ecall` instruction that was executed, and the name of the system call
that is being initiated. This will require us to interface with the trap
processing mechanism in xv6.

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

Then, make sure that the branch `systrace` shows up

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
  remotes/upstream/systrace
```

Then, get the `systrace` branch as follows:

```sh
$ git checkout -b systrace_sol upstream/systrace
branch 'systrace_sol' set up to track 'upstream/systrace'.
Switched to a new branch 'systrace'
```

Finally, push the empty stub to your own repo using:

```sh
$ git push --set-upstream origin systrace_sol
Enumerating objects: 99, done.
Counting objects: 100% (99/99), done.
Delta compression using up to 56 threads
Compressing objects: 100% (80/80), done.
Writing objects: 100% (88/88), 3.15 MiB | 11.39 MiB/s, done.
Total 88 (delta 15), reused 19 (delta 2), pack-reused 0
remote: Resolving deltas: 100% (15/15), completed with 11 local objects.
remote:
remote: Create a pull request for 'systrace_sol' on GitHub by visiting:
remote:      https://github.com/user/csse332-labs-user/pull/new/systrace_sol
remote:
To github.com:user/csse332-labs-noureddi.git
 * [new branch]      systrace_sol -> systrace_sol
branch 'systrace_sol' set up to track 'origin/systrace_sol'.
```

# The Activity

After our discussion about traps in class, it should be obvious to you where
the code should be added. Here are a few things to help you out.

## Getting process information

To get the information about a process, you can access its process control
block. You can check the content of a process control block in xv6 by looking
at the content of `struct proc` structure in `kernel/proc.h`. All of the
information about a process will be contained in the PCB.

## Getting instruction information

After our discussion in class, it should be very easy for you to access the
address of the instruction that caused the trap. The code in `usertrap` will
prove to be very helpful.

## Getting system call information

In RISC-V, when a process needs to make a system call, it will place the number
of that system call in the `a7` register and then issue the `ecall`
instruction.

> How can you access the `a7` register in the `usertrap` function?

Once you figure out the answer to that question, you can use the following
function to get the system call name. This is just a helper function that you
can use, you can also check the system call numbers in `kernel/syscall.h`.
You will need to add `#include "syscall.h"` at the top of the trap file
to access those constants.

```c
static const char *sysnum_to_str(int sysnum) {
  switch(sysnum) {
    case SYS_fork:
      return "fork";
      break;
    case SYS_exit:
      return "exit";
      break;
    case SYS_wait:  
      return "wait";
      break;
    case SYS_pipe:
      return "pipe";
      break;
    case SYS_read:
      return "read";
      break;
    case SYS_kill:
      return "kill";
      break;
    case SYS_exec:
      return "exec";
      break;
    case SYS_fstat:
      return "fstat";
      break;
    case SYS_chdir:
      return "chdir";
      break;
    case SYS_dup:
      return "dup";
      break;
    case SYS_getpid:
      return "getpid";
      break;
    case SYS_sbrk:
      return "sbrk";
      break;
    case SYS_sleep:
      return "sleep";
      break;
    case SYS_uptime:
      return "uptime";
      break;
    case SYS_open:
      return "open";
      break;
    case SYS_write:
      return "write";
      break;
    case SYS_mknod:
      return "mknod";
      break;
    case SYS_unlink:
      return "unlink";
      break;
    case SYS_link:
      return "link";
      break;
    case SYS_mkdir:
      return "mkdir";
      break;
    case SYS_close :
      return "close";
      break;
    default:
      return "unknown";
      break;
  }
}
```

You can use this function to obtain a string representing the system call given
a system call number.

## Testing

Once you are ready, compile your xv6 kernel using `make qemu` and then issue
any command, you should see your tracing line being printed out as the process
you launched is executing.

> Note that due to buffering issues, your printouts will show up on top and/or
  intermingled with printouts from the processes in the kernel, that is totally
  okay.

Here' an example of running it on my machine:

```sh
xv6 kernel is booting

hart 1 starting
hart 2 starting
Process 1:initcode called system call: exec from address 0x0000000000000014
Process 1:init called system call: open from address 0x00000000000003c0
Process 1:init called system call: dup from address 0x00000000000003f8
Process 1:init called system call: dup from address 0x00000000000003f8
Process 1:init called system call: write from address 0x00000000000003a0
iProcess 1:init called system call: write from address 0x00000000000003a0
nProcess 1:init called system call: write from address 0x00000000000003a0
iProcess 1:init called system call: write from address 0x00000000000003a0
tProcess 1:init called system call: write from address 0x00000000000003a0
:Process 1:init called system call: write from address 0x00000000000003a0
 Process 1:init called system call: write from address 0x00000000000003a0
sProcess 1:init called system call: write from address 0x00000000000003a0
tProcess 1:init called system call: write from address 0x00000000000003a0
aProcess 1:init called system call: write from address 0x00000000000003a0
rProcess 1:init called system call: write from address 0x00000000000003a0
tProcess 1:init called system call: write from address 0x00000000000003a0
iProcess 1:init called system call: write from address 0x00000000000003a0
nProcess 1:init called system call: write from address 0x00000000000003a0
gProcess 1:init called system call: write from address 0x00000000000003a0
 Process 1:init called system call: write from address 0x00000000000003a0
sProcess 1:init called system call: write from address 0x00000000000003a0
hProcess 1:init called system call: write from address 0x00000000000003a0

Process 1:init called system call: fork from address 0x0000000000000378
Process 2:init called system call: exec from address 0x00000000000003b8
Process 1:init called system call: wait from address 0x0000000000000388
Process 2:sh called system call: open from address 0x0000000000000e38
Process 2:sh called system call: close from address 0x0000000000000e20
Process 2:sh called system call: write from address 0x0000000000000e18
$ Process 2:sh called system call: read from address 0x0000000000000e10
```

When issuing `echo 'hello'`, we get the following:

```sh
$
echo 'hello'
Process 5:sh called system call: read from address 0x0000000000000e10
Process 5:sh called system call: read from address 0x0000000000000e10
Process 5:sh called system call: read from address 0x0000000000000e10
Process 5:sh called system call: read from address 0x0000000000000e10
Process 5:sh called system call: read from address 0x0000000000000e10
Process 5:sh called system call: read from address 0x0000000000000e10
Process 5:sh called system call: read from address 0x0000000000000e10
Process 5:sh called system call: read from address 0x0000000000000e10
Process 5:sh called system call: read from address 0x0000000000000e10
Process 5:sh called system call: read from address 0x0000000000000e10
Process 5:sh called system call: read from address 0x0000000000000e10
Process 5:sh called system call: read from address 0x0000000000000e10
Process 5:sh called system call: fork from address 0x0000000000000df0
Process 5:sh called system call: wait from address 0x0000000000000e00
Process 7:sh called system call: sbrk from address 0x0000000000000e80
Process 7:sh called system call: exec from address 0x0000000000000e30
Process 7:echo called system call: write from address 0x0000000000000336
'hello'Process 7:echo called system call: write from address 0x0000000000000336

Process 7:echo called system call: exit from address 0x0000000000000316
Process 5:sh called system call: write from address 0x0000000000000e18
$ Process 5:sh called system call: read from address 0x0000000000000e10
```

