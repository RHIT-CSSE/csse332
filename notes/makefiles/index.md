---
layout: post
title: Makefile and Debugging
readtime: true
date: Wed 15 Mar 2023 04:24:07 PM EDT 
---

# Learning Objectives

At the end of this lecture, you should be able to:

- Define the steps involved in building an executable starting from your source
  code.

- Write a `makefile` that automates the build of your projects.


# Topics

In this lecture, we will cover the following topics:

- Makefiles and the build process.

---

# Notes

<!-- vim-markdown-toc GFM -->

* [Motivation](#motivation)
* [Interlude: Building Targets](#interlude-building-targets)
* [Incremental Builds and Makefiles](#incremental-builds-and-makefiles)
  * [Makefile Syntax](#makefile-syntax)
  * [Example](#example)

<!-- vim-markdown-toc -->

# Motivation

- When writing large projects, we generally do not dump all of the source code
  in one file.
  - We (hopefully) intelligently separate our code base into logical
    compartments the implement different functionalities.
  - Our compartments will depend on each other.
  - We will finally put everything together to have final deliverable or
    executable.
- So our projects are spread out across several source files and folders that
  have internal dependencies.
- When building those projects, it is essential to the following:
  - Keep track of the dependencies.
  - On every change, it is essential to minimize the amount of work we have to
    do by only rebuilding the components of the project that have changed.
- In a sense, this is very similar to us trying to cook a meal, which has
  several components, and each component requires a recipe and a certain set of
  ingredients.

- __Definition__: `make` is a _generic_ build tool that allows you to:
  1. Define targets and dependencies between them.
  2. Write a recipe that defines how each target is built.
  3. Execute the recipe and produce the ultimate (and intermediate) targets.

# Interlude: Building Targets

- So far, we have seen how to compile a source file and produce an executable
  from the command line, using something like:
  ```shell
  $ gcc -o a.bin a.c
  ```
  - But what are the steps involved in producing `a.bin`?

- First, we do a preprocessing step, in which a intermediary file (typically
  with a `.i` extension) is produced.
- Second, we compile `a.i` into assembly language for the appropriate
  architecture, producing a file with `.S` extension.
- Third, we build an object file (typically with a `.o` extension) that contains
  (among other thing) the machine representation of our assembly instructions.
- Finally, since we normally have dependencies to resolve, the __linker__ will
  take care of bringing object files from different places to produce a
  self-contained executable that you can run using `./a.bin`.
- We can try this and see all those files using
  ```shell
  $ gcc --save-temps -o a.bin a.c
  ```
- We can then individually inspect each of these files.

# Incremental Builds and Makefiles

- So now we know the steps of a build process, we would like to optimize
  building larger projects.
- Generally, our implementation will be spread out across several `.c` source
  files.
- So if we change one of those files, do we really want to recompile and redo
  all of the steps for all of our source files?
  - Not really, we can make use of the presence of those intermediary steps in
    the building process.

- Knowing that, we can make our building process more efficient:
  - If I change a source file, I can simply recompile that file into its
    corresponding object file.
  - All other object files remain unchanged.
  - Then we can link again to produce our final executable.
  - Unchanged file do not have to be recompiled.

- Note, to simply recompile a source file into an object file without linking,
  we can use the `-c` flag with `gcc`. For example:
  ```shell
  $ gcc -c a.c
  ```
  - This will generate the object file `a.o` without linking.

## Makefile Syntax

- So now we can talk about writing makefiles that would help us build things
  incrementally.
- You can think about a makefile as a bunch of rules that will be executed to
  produce a given target.
- A makefile rule looks something like the following:
  ```make
  target: prerequisites
    command
    command
  ```
  - Note the following:
    1. `make` expects that your goal is to produce a file called `target`.
    2. The commands will execute in order.
    3. Every command must be preceded by a `<tab>` character, `make` will
       complain if you do not have those characters in (manually adding space
       will not work).

- You can build several rules for several targets and create dependencies
  between them, something like the following:
  ```make
  big_target: medium_target little_target1
    command1
    command2

  medium_target: little_target2 little_target3
    command1
    command2

  little_target1: file1 file2
    command1

  little_target2: file3
    command1

  little_target3:
    command1
  ```

## Example

- Consider an example project where we have the following files:
  - `a.c` and `a.h` contains a bunch of function definitions and
    implementations.
  - `b.c` that contains our `main` function, but it externs a function
    implemented in `c.c`.
    - For a good discussion of using header files vs forward declarations, check
      out [this
      link](https://google.github.io/styleguide/cppguide.html#Forward_Declarations).
  - `c.c` implements a function needed from `b.c`.

- So, here's how we can write our `Makefile`:
  ```make
  b.bin: b.o a.o c.o
    gcc -o b.in b.o a.o c.o

  a.o: a.c a.h
    gcc -c a.c a.h

  b.o: b.c
    gcc -c b.c

  c.o: c.c
    gcc -c c.c
  ```


