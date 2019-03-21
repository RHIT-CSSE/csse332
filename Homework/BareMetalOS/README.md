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
* A command line shell with a minimum of necessary commands (directory listing, type, copy, execute)
* Multiprocessing and basic memory management


This project was designed by Michael Black at American University. An
overview of the project is given in his [SIGCSE
 paper](OS_paper_for_sigcse_2009_final.pdf). You should read the
paper soon to get a general idea of the scope and complexity of the
project. Although this is a relatively simple "Bare Metal" project,
you only have five weeks to complete it and there are many components
that you must complete. You are encouraged to work incrementally and
consistently in order to be able to complete the project on time.

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

This project will be on teams on 2-3 students.  Your will evaluate
your teammates at the end of the project.

You will setup a repository on the first day of the project.

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
  - make commands that should be used to build and run
  - any bugs or deficiencies
