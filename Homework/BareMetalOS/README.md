---
title: Bare Metal Operating System Project
layout: togit
---

# Bare Metal Operating System Project

In this project you will build a 16-bit bare metal operating system
from scratch.  Your finished operating system will consist of the
following components:

* A boot loader to boot from a floppy disk image
* A set of system calls that are made using interrupts
* A file system
* The ability to execute a program from a file
* A command line shell with a minimum of necessary commands (directory listing, type, copy, delete, execute)
* Multiprocessing and basic memory management


This project was designed by Michael Black at American University. An
overview of the project is given in his [SIGCSE
 paper](OS_paper_for_sigcse_2009_final.pdf). You should read the
paper soon to get a general idea of the scope and complexity of the
project. Although this is a relatively simple "Bare Metal" project,
you only have five weeks to complete it and there are many components
that you must complete. You are encouraged to work incrementally and
consistently in order to be able to complete the project on time.

# Learning Objectives

In the process of completing the final project you will learn to:

* Develop a simple bare metal operating from scratch, in which
you explore the intricacies of
    - booting an operating
    - system calls
    - file System
    - process execution
    - command line shells
    - multiprocessing
    - basic memory management
* Exploit a variety of resources that will facilitate the
development of the operating system.
* Work in teams of two to three students to develop the
operating system.
* Evaluate your teammates at the end of the project.


# Coding Standards

You should adopt and adhere to a standard that results in readable
code. Remember, the easier your code is to read, the better the mood
of the grader grading it.

You must write ANSI C code that you will compile with the bcc compiler
and as86 assembler.

* Your comments should take the following form:

    /* This is a valid ANSI comment */

* Declare all your local variables in a declaration block at the start
  of each function, but do not initialize them yet.
* Initialize your variables outside the declaration block, after you
  declare them.
* Be sure that your main function returns an int and not void.


# Project Teams

We will setup a repository on the first day of the project.

Each member of your team is advised to checkout your team repository
at the start of the project.  Before you edit code, documents, and
other aspects of the project, you should update your local workspace,
edit and update, then commit your work back to your repository.  Using
this cycle will minimize conflicts.  Remember to provide appropriate
commit messages and to do git add if you want to add new files to your
repository.  Of course, don't forget to commit after you add new
files.


# Deliverables

This project is divided into five mini-projects and an evaluation of
your project team.  Each mini-project contains a rubric that will be
used to evaluate it.  You are encouraged to review the rubrics to
become familiar with the criteria for evaluating each milestone.

See the course schedule for all dates.

At the end of each milestone, you should commit updated versions of
each of the following to your team's repository:

* The source code
* A Makefile that builds everything
* A README file containing:
  - instructions for building your operating system,
  - instructions for testing and using the operating system,
  - any bugs or deficiencies

Additional features that you implemented should be described in a
separate section of your README file. They will be evaluated together
with the final milestone.

You should be using good process as you go.  Thus, for each milestone,
your code must run, have good style and complete documentation.  It
should also have no other warnings.


# Presentation

Your team will give a 10--15 minute presentation on your project
during week 9.  Your goals for this presentation
are:
* Confidently and professionally describe your operating system.
* Demonstrate how a user of your system could use it to accomplish a
  satisfactory set of tasks.
* Demonstrate a sampling of the required and optional features that
  you implemented.
* Show off cool features that you are proud of.
* Describe the basic design of your system and discuss the modularity
  in your design.

Every team member should play a significant role in the delivery of
your presentation.
