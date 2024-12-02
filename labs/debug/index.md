---
layout: post
title: Lab 04 -- Debug Lab
readtime: true
date: 2024-06-07 20:45
---

# Introduction

In this lab assignment, we will practice some of the `gdb` and `valgrind`
skills that we built and examined during the lecture. Your task is to find and
fix bugs in three exercises. While you can debug the problems in different ways
(including printouts), you **must** use `gdb` and/or `valgrind` to find the
bugs and fix them. You will be asked to write a report detailing how you found
the bugs and the steps you took to do so.

# Leaning Objectives

At the end of the assignment, you should be able to:
- Use `gdb` to find memory allocation bugs in C program.
- Use `gdb` to find logic bugs in C programs.
- Use `valgrind` to find memory leaks in C programs.

# Getting the Source Code

Unlike the first few labs, __we will not do this lab in the xv6 operating
system__, we will rather use your native Linux virtual machine (or baremetal
machine if you are running one). As it stands, `xv6` lacks some of the features
that make this assignment doable and manageable given our leaning objectives.

# Getting the Source Code

We will do this lab in the `main` branch of your labs repository. To make sure
you are on the right branch, check it out using:

  ```sh
  $ git branch
  ```
The branch you are currently on will be highlighted for you (with a \* next to
its name).

If you are working on the `main` or `master` branch, then follow these
instructions:

  ```sh
  $ git fetch upstream
  $ git pull upstream main
  ```
At this stage, you should have the latest copy of the code, and you are good to
get started. The starter code is contained under the `debug/` directory.

If you are currently on a different branch (say you are still on `clab_solution`
from the last lab), then we need to switch to `main` or `master` (depending on
your default's name).

First, add, commit, and push your changes to the `clab_solution` to make sure
you do not lose any progress you did on the last lab. To check the status of
your current branch, you can use:
  ```sh
  $ git status
  ```
This will show you all the files you have modified and have not yet committed
and pushed. Make sure you `add` those files, then `commit` your changes, and
`push` them.

If `git push` complains about not knowing where to push, you'd want to push the
current branch you are on. So for example, if I am working on `clab_solution`,
then I'd want to do `git push origin clab_solution`.

Now, you are ready to swap back into `main` (or `master`).

```sh
$ git checkout main
```

Then, grab the latest changes using:

```sh
$ git fetch upstream
$ git pull upstream main
```

At this stage, you should have the latest copy of the code, and you are good to
get started. The starter code is contained under the `debug/` directory.

# Deliverables: Logs and Code

In addition to fixing the source code, you will be asked to submit your `gdb`
and/or `valgrind` logs. To do so, you must enable command tracing and logging
from `gdb` as follows.

## Generating logs for `gdb`

Say I am debugging `al.bin`. First, launch `gdb` using `gdb ./al.bin`, then
when in `gdb` **before you start your session**, issue the following commands:

```
(gdb) set trace-commands on
(gdb) set logging file exercise1.log
+set logging file exercise1.log
(gdb) set logging enabled
+set logging enabled
Copying output to exercise1.log.
Copying debug output to exercise1.log.
```

When you are done with your debug session, this will generate a file called
`exercise1.log` that contains all of your commands and the output from those
commands. Please **submit** a file for each problem that reflects how you used
`gdb` to find the bug.

### Engagement pint: Config or script `gdb`

To make the process above easier, `gdb` support configuration options and/or
scripting so that you don't have to type these commands every time. For an
opportunity to gain multiple engagement points, suggest a way for us to use
those config options and post it on the class forum.

## Generating logs for `valgrind`

For `valgrind`, simple save the output of running `valgrind` in a text file and
submit that along with your solution.

# Compiling and running the code

As always, you can use `make` to compile all the code in the lab directory. You
can run each file separately depending on which exercise you are working on.

# Exercise 1: Array list

The first exercise is concerned with the program in `al.c`. This program
contains two bugs: (1) that leads to a segmentation fault and (2) another that
leads to memory leaking. Use your debugging tool-set and find and fix those
steps.

Always remember to log your `gdb` session to gain credit for solving this
exercise.

# Exercise 2: Selection sort and Binary Search

The second exercise is concerned with the program in `selection.c`. This
program contains three bugs:

1. The first is memory access bug that causes the program to segfault when run.

2. The second is a logic bug in the selection sort algorithm. It is okay if
   your fix for problem (1) above also fixes this one, just make sure that the
   array is sorted when you run your code.

3. The third is a logic bug in the binary search algorithm. This bug causes the
   algorithm to go into an infinite loop.

To help you out, here is the expected output when your code works correctly:

```shell
./selection.bin
unsorted list:
         [  100   45   89   27  317   17 ]
list in order:
         [   17   27   45   89  100  317 ]
test if 42 is in the array...
        NO
```

# Exercise 3: Singly linked list

The third exercise is concerned with the program contained in `student.c`,
`sllist.c` and `sllist.h`. You should only be needing to look into `sllist.c`
and `student.c`

`student.c` contains the `main` function that this program runs. It uses the
linked list API defined `sllist.h` and implemented in `sllist.c`.

## Running the code

To run this program, compile it first using `make` and then you can run in
multiple ways. It has several test cases that are attempt to isolate the bugs
as much as possible.

To run the first test case, you can use the `-t` switch in the code as follows:

```sh
./student.bin -t 1
Test 1 (simple add_head): FAILED (list is still empty after adding)
```

This shows that we are attempting to run test 1 and that it has failed.

There are four tests cases in `student.c`, use those to guide you to find the
four bugs in the code. Note that running the program without the `-t` switch
will run all the test cases.

Here's an example of running the tests 2 and 3:

```sh
./student.bin -t 2
Test 2 (simple add_tail): FAILED (list is still empty after add_tail)
```

```sh
./student.bin -t 3
Test 3 (general add and delete): FAILED (list still empty after insertion).
```

## Your task

Your job in this exercises is to find and fix all the four bugs in this code,
you only need to use `gdb`, but **please make sure to log your session for
submission**.

Here is the expected output when your code has completed correctly:

```sh
./student.bin
Test 1 (simple add_head): OK.
Test 2 (simple add_tail): OK.
Test 3 (general add and delete): OK.
```

and for test 0,

```sh
./student.bin -t 0
{
  Mohammad, [ 0, 2, 4 ]
  Robert  , [ 2, 4, 6 ]
  Sid     , [ 4, 6, 8 ]
  Sriram  , [ 6, 8, 10 ]
  Buffalo , [ 8, 10, 12 ]
}
```

# Submitting your solution

From your Linux terminal, issue the following command (make sure your are in
the `debug` directory):

```sh
./create_submission.sh <username>
```
and replace <username> with your RHIT username (without the < and >). For
example, for me, that would be:

```sh
./create_submission.sh noureddi
```

If you get a message saying that you don’t have permission to run
`./create_submission.sh`, then issue the following command first:

```sh
chmod +x ./create_submission.sh
```

Here's the output it shows on my end:

```sh
Process started: writing temporaries to /tmp/dab9bfedf8d508c2a1c3f1c95e6ba1fc.txt
Found the following modified files:
./al.c
./selection.c
./sllist.c
./student.c
./ex1.log
./ex2.log
./ex3.log
Creating the submission zip file.
  adding: al.c (deflated 53%)
  adding: selection.c (deflated 55%)
  adding: sllist.c (deflated 56%)
  adding: student.c (deflated 67%)
  adding: ex1.log (deflated 52%)
  adding: ex2.log (deflated 34%)
  adding: ex3.log (deflated 54%)
Done...
################################################################
        submission_noureddi.zip has been created.
   Please submit THIS FILE AND THIS FILE ONLY to Gradescope.
################################################################
```

As you can see, my files contain the modified `.c` files as well as the log
files I generated from running the `gdb` sessions.

This will generate a single file called `submission-username.zip` (for me, it
would be `submission-noureddi.zip`). That is all you need to upload to
[Gradescope]({{site.gradescope_url}}).


## Submission Checklist

- [ ] My code compiles and generates the right executables.
- [ ] I generate the `gdb` and `valgrind` logs as needed.
- [ ] I ran the submission script to generate my `zip` file.
- [ ] I submitted the `.zip` file to [Gradescope]({{ site.gradescope_url }}).

