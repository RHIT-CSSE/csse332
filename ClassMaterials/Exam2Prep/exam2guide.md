---
layout: togit
---

# Exam 2 Preparation

Last updated: Spring 18-19

Note that exam contents can change term to term.

# Sample Exams

You should plan to practice using the [sample exams available here](../../SampleExams/).

But be aware the format of the exam has changed quite a bit from
previous years.  For this term you'll want to look at the Threading
question from Exam 1s of the past (i.e. thread commander and
producer-consumer).

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

* How to use threads to parallelize code, creating them, joining them
* How to pass parameters to threads (and how to do it so that all your
  threads don't end up sharing the same parameters)
* How to create/wait/post on semaphores, how to use semaphores to
  prevent concurrency problems while still allowing parallelism
        
This particular question will also involve merge sort (in a pretty
different way than the one you did on your lab) so if you don't recall
how that algorithm worked from 220 you might get a quick refresher.
