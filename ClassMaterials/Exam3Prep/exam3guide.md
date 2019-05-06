---
layout: togit
---

# Exam 3 Preparation

Last updated: Spring 18-19

Note that exam contents can change term to term.

# Sample Exams

You should plan to practice using the [sample exams available here](../../SampleExams/).

But be aware the format of the exam has changed quite a bit from
previous years.  For this term you'll want to look at the Semaphores
questions from Exam 2s of the past (i.e. backups for 201830 and all
the computer questions in 201630).  

The questions will be most similar to Semaphores 1 and 2 homework
assignments - that is to say small individual problems where most of
the code is written but you need to add semaphores to produce the
correct concurrency behavior.

# General Guidance


* Remember that you must get at least a 65% on this exam to pass the
  course (though there is a makeup)
* Being timed is part of the exam, monitor your time
* The parts tend to be of increasing difficulty, but if you get stuck
  sometimes it is possible to try the parts out of order
* Segfaults and other craziness are less likely here, but it's still a
  good idea to backup your code
* Open book, open course website, you can use all the sample code
  we've given you plus any code you have written
* No google/other websites but you can use manpages and the professors
  (maybe) can be your google
* We will expect you to solve novel problems on the exam, not simply
  regurgitate previous code we've give you

# Fundamental skills

* Semaphores, including waits and posts
* Concurrent algorithms we've talked about like producer consumer,
  reader/writer, and dining philosophers
  
# Semaphore specific advice

* Always feel free to ask if you feel you do not understand what is
  correct and incorrect behavior
* Note that it's frequent that the output looks correct but there are
  still concurrency bugs
* You can use sleeps to adjust timing and try and evidence weird
  concurrency conditions BUT never rely on sleeps to make your code work
* Don't try and think of everything up front - step through the
  various cases adding semaphores as you need
* Think hard about edge cases and deadlocks - usually your code will
  not evidence these kinds of problems in testing
* Never allow two concurrent threads to modify a variable or a thread
  to read a variable while in an inconsistent state
* Busywaits and weird semaphore functions are never needed to solve
  the problem
  
# How we will grade

There will be 5 problems each worth 20 points

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
  you utilize busywaits or non-standard semaphore functions
  (i.e. stuff other than init, post, wait, and destroy)
* A fully correct solution, even if it's not the one we envisioned, is
  20/20
