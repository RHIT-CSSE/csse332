---
layout: post
title: The Process Abstraction
readtime: true
date: Wed Nov 30 22:25:14 2022 
---

# Learning Objectives

At the end of this lecture, you should be able to:
- Define a computer process and distinguish it from a static program.
- Identify the content of a process's memory address space.
- Explain how the operating system keeps track of running and sleeping
  processes. 

---

# Topics

In this lecture, we will cover the following topics:
- Processes and programs.
- Memory address spaces.
- The process control block.

---

# Notes


<!-- vim-markdown-toc GFM -->

* [Definition of a Process](#definition-of-a-process)
* [Multiple Processes in the System (Context Switching)](#multiple-processes-in-the-system-context-switching)
* [The Process Control Block](#the-process-control-block)
* [Threads and Multi-Threading](#threads-and-multi-threading)

<!-- vim-markdown-toc -->

# Definition of a Process

- A process is nothing but a program in execution
- And what is a program?
  - A program is a bunch of machine instructions.
- When you run a program, like doing `./glue` as we did in the previous case,
  what happens in that case?
- Well the shell (or your GUI server) is going to call on the operating system
  and ask it to **fork** a new process, which is then going to **execute** the
  program `glue`
- Each process will have a memory address region allocated for it, referred to
  as the **process address space**.
- In each process address space, we have sections of memory that contain
  different things:
  - The code region that contains the program's code as machine instructions
  - The global region that contains global variables
  - The heap which is a region for dynamic memory allocation
  - The stack which,
    - Grows **downward**
    - Hosts local variable and statically allocated variables

# Multiple Processes in the System (Context Switching)

- The operating system is in charge of asking the CPU to execute multiple
  different processes, including itself
- so it must have a way to switch between different processes
- how do you know where the process is executing from?
  - The program counter register
- So the OS will save the process's program counter, its registers along with
  all its information before switching it out and then load another one.


# The Process Control Block

- The PCB contains all of the information necessary to know about a process, its
  ID, its state, its memory regions, page tables, children, etc.
- Show the `task_struct` in Linux

# Threads and Multi-Threading

- Threads are lightweight processes, they share the data, code, and open files
- But they have each their own registers and stack
- Switching between processes is easier than switching between threads
- In Linux, everything is a task!


