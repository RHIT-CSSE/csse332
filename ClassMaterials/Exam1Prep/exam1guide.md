---
layout: togit
---

# Exam 1 Preparation

Last updated: Winter 18-19

Note that exam contents can change term to term.

# Sample Exams

You should plan to practice using the [sample exams available here](../../SampleExams/).

Be sure to time yourself (170 minutes to do paper part + computer part)

# Paper Part

~35 points

While it's good to review the material and (especially) the types of
problems you'll see, be cognizant that it's not worth memorizing huge
chunks of the textbook for only a few points.

1 note sheet front and back allowed, no other materials

Should take ~20-30 minutes or so, up to 50 if you're a slow writer

We may ask you some text questions but the answer is always
expressible in 1 sentence.  We don't mind if you use 2 but if you feel
like you have to write a paragraph you're probably wrong.

## The five state process model

~10 points

* See textbook section 3.1 & 3.2 and especially figure 3.2.
* Know what the states mean and why transitions occur
* Expect a question like the quiz where you have processes in various
  states and then events happen and you need to know what state
  everything is in after the fact (see sample exams for what I mean by
  that)
  
## CPU Scheduling

~10 points

* Simulate the various scheduling algorithms we discussed in class
* Similar to CPU Scheduling homework assignment
* See the CPU Scheduling slides, and also

http://pages.cs.wisc.edu/~remzi/OSTEP/cpu-sched.pdf

## Userspace Threading

~8 points

* Frequently as compared to kernel threads
* See Userspace Threading slides and Kernel Threads Slides
* Also 4.1 - 4.3 in your textbook

## Signals

~7 points

* We'll ask you to look at some signal code and explain what it does
* Be sure to know classic unix functions signal, alarm, ualarm,
  sigprocmask, sleep
* Signal tutorial from Lab 2 is a good resource http://titania.ctie.monash.edu.au/signals/
  
# Computer part

~65 points, also you must have a passing average (65%) on the 2 exam
computer parts to pass the course

* Being timed is part of the exam, monitor your time
* Realize that within a particular question, the parts tend to be of
  increasing difficulty.  So if you get stuck on one part, go on to
  the other questions and then return to it.
* Backup your working solution as you go, so if you start to get
  segfaults or whatever you can go back to a working codebase
* Know how to use gdb to debug a problem like a segfault and at least
  figure out where you're erroring
* Open book, open course website, you can use all the sample code
  we've given you plus any code you have written
* No google/other websites but you can use manpages and the professors
  (maybe) can be your google
* We try to note the fundamental skills we are looking for in each of
  the parts...be sure you can confidently do those things
* We will expect you to solve novel problems on the exam, not simply
  regurgitate previous code we've give you

## Subprocesses

a.k.a. Forking/Waiting/Execing

~30 points in multiple parts

Fundamental skills

* How to fork, how to tell if you're the parent/child, how to combine
  multiple forks in loops or other structures to produce children that
  act a specific way (e.g. serially, parallel)
* How to get data to the children so they know what they need to do
* How to wait, how to communicate using exit codes, how to know the
  pids of your children
* How to exec (e.g. execl, execv), how to pass parameters to exec
* Not included: Inter-process communication (you're welcome)

## Threading

Using pthreads

~30 points in multiple parts

Fundamental skills

* How to use threads to parallelize code, creating them, joining them
* How to pass parameters to threads (and how to do it so that all your
  threads don't end up sharing the same parameters)
* How to create/wait/post on semaphores, how to use semaphores to
  prevent concurrency problems while still allowing parallelism
  
## Makefile

~5 points

An easy way to get a few points - it's silly to lose these.

Makefiles can do a lot of things but the only thing we care about are:

1. To build an ordinary file, put its name then it's dependencies,
   then (tabbed in) its build commands
   
        ordinaryExecutable: ordinary.c ordinary.h
            gcc -ggdb -o ordinaryExecutable ordinary.c ordinary.h

2. Oftentimes we want to save on compile times but building .o files
   and then linking them in a separate step.  The -c flag produces an
   intermediate file for each .c that then can be combined into a
   single final output.
   
        library.o: library.c library.h
            gcc -c -ggdb -o library.o library.c library.h
        
        libraryUser.o: library.h libraryUser.c
            gcc -c -ggdb -o libraryUser.o library.c library.h
            
        libraryUser: library.o libraryUser.o
            gcc -o libraryUser -ggdb library.o libraryUser.o
    
        
