---
layout: post
title: Exam 2 Guide
readtime: true
date: Tues April 27
gh-repo: rhit-csse332/csse332-202130
gh-badge: [star,watch,follow]
---

# Exam 2 Preparation

Last updated: Spring 20-21

Note that exam contents can change term to term.

# Sample Exams

You exam will be:

+ 3 semaphore questions (60 points)
+ 1 2-part thread creation question (40 points)

The best preparation for this exam 2 is
[Exam2-202020](https://github.com/rhit-csse332/csse332-202130/tree/main/sample_exams/Exam2-202020),
which is the only released exam in the newer
exam format (where thread-creation questions and semaphore questions were both
on the same exam).

For older exams, here's what I recommend.

For thread creation questions:

+ exam2-201930 (mergesort) longer than the threading question on your exam, but
  basic idea is good
+ exam1-201830 (producer-consumer)
+ exam2-201830 computer/backups.c
+ exam1-201630 ThreadCommander shorter than the threading question on your exam
(see csse332\_exam1\_computer.pdf for instructions)

For semaphore questions:

+ exam3-201930 more semaphore questions than on your exam, but more is better
  practice right?
+ exam2-201630 maybe on the easier side
+ exam2-201830 para\_seri\_para definitely easier


# General Guidance


* Remember that you must get at least a 65% on this exam to pass the
  course (though there is a makeup)
* Being timed is part of the exam, monitor your time
* The parts tend to be of increasing difficulty, but if you get stuck
  sometimes it is possible to try the parts out of order
* Backup your working solution as you go, so if you start to get
  segfaults or whatever you can go back to a working codebase
* Know how to use gdb to debug a problem like a segfault and at least
  figure out where you're erroring
* Open book, open course website, you can use all the sample code
  we've given you plus any code you have written
* No google/other websites but you can use manpages and the professors
  (maybe) can be your google
* We will expect you to solve novel problems on the exam, not simply
  regurgitate previous code we've give you

# Fundamental skills

## Thread Creation

* How to use threads to parallelize code, creating them, joining them
* How to pass parameters to threads, how to use structs to pass more
  than one, and how to do it so that all your threads don't end up
  sharing the same parameters
* How to create/wait/post on semaphores, how to use semaphores to
  prevent concurrency problems while still allowing parallelism
        
## Semaphores

* Semaphores, including waits and posts
* Concurrent algorithms we've talked about like producer consumer,
  reader/writer, the scoreboard problems, and dining philosophers

### Semaphore specific advice

* Always feel free to ask if you feel you do not understand what is
  correct and incorrect behavior
* Note that it's frequent that the output looks correct but there are
  still concurrency bugs
* You can use sleeps to adjust timing and try and evidence weird
  concurrency conditions BUT never rely on sleeps to make your code work
* Don't decide up-front how many semaphores you need - step through the
  various cases adding semaphores as they are required
* Think hard about edge cases and deadlocks - usually your code will
  not evidence these kinds of problems in testing
* Never allow two concurrent threads to modify a variable or a thread
  to read a variable while in an inconsistent state
* Busywaits and weird semaphore functions are never needed to solve
  the problem
* BUT if you are stuck and can't figure out how to solve a problem the
  normal way, busywaits and weird semaphore functions are worth a lot
  more than nothing
  
### How we will grade

Each semaphore problem will be worth 20 points

* You can get 5/20 points if your semaphore-based solution reliably
  produces correct output with the given data (and small
  modifications).  0 points if doing this requires weird sleeps or
  other hacky non-general purpose code.
* You can get 10/20 points you did above and your solution is
  plausible and does not do something obviously bad.  It's impossible
  to enumerate every obviously bad thing but here's a big one : having
  multiple concurrent threads modify a global variable.  Basically
  this is for solutions that basically are OK but miss some edge
  cases.
* You can get 15/20 points if you have a fully working solution but
  you utilize busywaits or non-standard semaphore functions (mutexes and
  condition variables are okay).
* A fully correct solution, even if it's not the one we envisioned, is
  20/20

