---
layout: post
title: Introduction to Memory Safety
readtime: true
date: Sun Nov 27 18:13:13 2022 
---

# Learning Objectives

At the end of this lecture, you should be able to:

- Define the goals of computer security.
- Identify the roles that the OS plays in achieving those goals.
- Define how integrity can be broken through control-flow alteration.

---

# Topics

In this lecture, we will cover the following topics:

- Computer security.
- OS security.
- Control-flow integrity.
- Stack smashing.

---

# Notes

# Prerequisites

- To follow up on this lab, make sure you are running an x86 machine (ARM works
  but would require a lot of changes).
- In most of these programs, we will be compiling things as 32 bits programs
  rather 64 bits. 
  - Things are simpler and easier to visualize when dealing with 32 bit
    compilations.
  - The same concepts can be easily extended to 64-bit machines.

- You can easily check which architecture you are running on using:
  
  ```shell
  $ uname -m
  x86_64
  ```
  - In my case, I am running the Intel x86_64 architecture.

## Required Software

- You are most likely running a 64-bit machine, so we will need to cross-compile
  our code to run a 32-bit version that is easier to understand.
- To do so, we must install the 32-bit versions of the C compiler and the
  debugger.

- From your Linux terminal, run the following command (assuming your are running
  a flavor of Ubuntu or Debian).
  
  ```shell
  $ sudo apt install -y gdb gdb-multiarch gcc-multilib
  ```

## Recommended Software

- This activity and later on lab will require heavy use of a debugger.
- `gdb` is your goto debugger for C programs on Linux, and it gladly supports
  multiple architectures.

- However, I recommend that you augment `gdb` with `gef`, a set of `gdb`
  configuration options and scripts that make the kind of analysis we do in this
  class easier and more visual.
- To install `gef`, from your Linux terminal, run the following:
  
  ```shell
  $ bash -c "$(curl -fsSL https://gef.blah.cat/sh)"
  ```

- If `curl` is not installed, you can install it using `sudo apt intall -y
  curl`.
