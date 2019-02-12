---
layout: togit
---

# Exam 2 Preparation

Last updated: Winter 18-19

Note that exam contents can change term to term.

# Sample Exams

You should plan to practice using the [sample exams available here](../../SampleExams/).

Be sure to time yourself (160 minutes to do computer part + 50 minutes to do paper part)

# Paper Part

~40 points

You can have a 1 page front and back note sheet for this part,
otherwise closed book closed notes.

The 201830 sample exam should give you a pretty accurate view of what
to expect, paper part wise.  But there are a few variations:

1.  Expect a banker's algorithm problem
2.  Don't expect a disk scheduling problem
3.  You should know about all page table structures: inverted,
    hierarchical, with and without TLBs
  
# Computer part

~60 points, also you must have a passing average (65%) on the 2 exam
computer parts to pass the course

* Being timed is part of the exam, monitor your time
* Realize that within a particular question, the parts tend to be of
  increasing difficulty.  So if you get stuck on one part, go on to
  the other questions and then return to it.
* Backup your working solution as you go, so if you start to get
  segfaults or whatever you can go back to a working codebase
* Open book, open course website, you can use all the sample code
  we've given you plus any code you have written
* No google/other websites but you can use manpages and the professors
  (maybe) can be your google
* We will expect you to solve novel problems on the exam, not simply
  regurgitate previous code we've give you

## Semaphores

~30 points

These problems will be most similar to the Semaphores 1 and 2 homework
assignments in that you will be given (mostly) functional code and
asked to add semaphores to accomplish specific goals.

General advice:

* None of these problems involves writing more that a couple
  semaphores and a couple variables, so if you find yourself writing a
  lot you're likely overcomplicating the solution.
* All waiting will be done with semaphores, not by busy-waiting using
  while loops.  In addition to being inefficient, most busy waits I
  see in the code mask concurrency problems.
* More semaphores means more interactions which means more
  possibilities for deadlocks and bugs.  Every semaphore you add
  should have a purpose.
* Practically speaking, these problems should take you less time than
  the OS problem


## Bare Metal OS Problem

~30 points in multiple parts

This problem will be most similar to the Practice Exam 2 problem we
did in class.

* Use print statements to your advantage - understand your problem
  before you try and fix your problem
* For filesystem stuff, remember also you can look at the disk image
  in a hex editor
* You'll have working code that already does part of the thing you
  need - make sure you understand it and use it as a starting point
* The steps will increase in difficulty as they go.  It will probably
  be the case that the last (and hardest) problem will be worth
  relatively few points for the amount of time required -- do it last
