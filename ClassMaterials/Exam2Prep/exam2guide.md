---
layout: togit
---

# Exam 2 Preparation

Last updated: Winter 21-22

Note that exam contents can change term to term.

# Sample Exams

You exam will be:

+ 3 semaphore questions (60 points)
+ 1 2-part thread creation question (40 points)

I'll update this a bit with the correct sample exams to look at.

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
        
## Condition Variables & Mutexes

* Mutex Lock & Unlock
* CVs Wait Signal Broadcast
* Concurrent algorithms we've talked about like producer consumer,
  reader/writer

### CV specific advice

* Always feel free to ask if you feel you do not understand what is
  correct and incorrect behavior
* Note that it's frequent that the output looks correct but there are
  still concurrency bugs
* You can use sleeps to adjust timing and try and evidence weird
  concurrency conditions BUT never rely on sleeps to make your code work
* Simple problems can be solved sometimes with multiple mutexes and no
  CVS.  However more complex problems almost always require CVs.
* If you have CVs there's usually no reason to have more than one 1
  mutex, and the ability to "lock the world" is one of the main
  benefits of CVs
* Don't be scared to have more than 1 CV - have 1 CV for each kind of
  waiting.  But always share the same mutex.
* Your CVs must always be in while loops.  If they are not they are
  buggy.
* Think very carefully about what state you will track.  A thread must
  always be able to know if it safe for it to proceed when it awakens.
* Think hard about edge cases and deadlocks - usually your code will
  not evidence these kinds of problems in testing
* Never allow two concurrent threads to modify a variable or a thread
  to read a variable while in an inconsistent state
* Busywaits and weird CV functions are never needed to solve
  the problem
* BUT if you are stuck and can't figure out how to solve a problem the
  normal way, busywaits are worth a lot more than nothing
  
### How we will grade

* You can get 25% points if your semaphore-based solution reliably
  produces correct output with the given data (and small
  modifications).  0 points if doing this requires weird sleeps or
  other hacky non-general purpose code.
* You can get 50% points you did above and your solution is
  plausible and does not do something obviously bad.  It's impossible
  to enumerate every obviously bad thing but here's a big one : having
  multiple concurrent threads modify a global variable.  Basically
  this is for solutions that basically are OK but miss some edge
  cases.
* You can get 75% points if you have a fully working solution but
  you utilize busywaits or non-standard CVs functions
* A fully correct solution, even if it's not the one we envisioned, is
  20/20
