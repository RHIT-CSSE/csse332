---
layout: togit
---

# Exam 1 Preparation

Last updated: Fall 19-20

Note that exam contents can change term to term.

# Sample Exams

You should plan to practice using the [sample exams available here](../../SampleExams/).

But be aware the format of the exam has changed quite a bit from previous years.

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
* We try to note the fundamental skills we are looking for in each of
  the parts...be sure you can confidently do those things
* We will expect you to solve novel problems on the exam, not simply
  regurgitate previous code we've give you

# Fundamental skills

* All your basic C, pointers etc.
* Command line parameters in c (argc, argv)

## Fork

* How to fork, how to tell if you're the parent/child, how to combine
  multiple forks in loops or other structures to produce children that
  act a specific way (e.g. serially, parallel)
* How to get data to the children so they know what they need to do
* How to wait, how to communicate using exit status
* how to know the pids of your children


## Exec

* The two flavors of exec (execlp, execvp) and when you want to use
  one or the other
* How to pass parameters to exec functions - don't forget the final null
* How to combine with fork/wait to run things in background
  foreground, or just convert one process to another
* Be sure to check for exec failures - can lead to some weird hard to debug stuff!

## Signals

* Registering handlers
* Masks
* Passing extra data to signal handlers (hint globals)
* Could be fair game but not actually on this exam

## IPC

* Pipes (exam1)
* Shared memory using mmap (exam1 makeup)


## Makefile

~15 points

An easy way to get a few points - it's silly to lose these.

Makefiles can do a lot of things but the only thing we care about are:

1. To build an ordinary file, put its name then it's dependencies,
   then (tabbed in) its build commands
   
        ordinaryExecutable: ordinary.c ordinary.h
            gcc -ggdb -o ordinaryExecutable ordinary.c

2. Oftentimes we want to save on compile times but building .o files
   and then linking them in a separate step.  The -c flag produces an
   intermediate file for each .c that then can be combined into a
   single final output.
   
        library.o: library.c library.h
            gcc -c -ggdb -o library.o library.c
        
        libraryUser.o: library.h libraryUser.c
            gcc -c -ggdb -o libraryUser.o libraryUser.c
            
        libraryUser: library.o libraryUser.o
            gcc -o libraryUser -ggdb library.o libraryUser.o
    
        
