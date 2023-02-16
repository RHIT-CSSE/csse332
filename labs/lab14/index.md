---
layout: post
title: Challenge Mini-Project Step 2
readtime: true
date: Wed Feb 15 2023 
---

# Introduction

For the final step of this project, you should submit your xv6 source code as
well as a two-page report that documents your design decisions, your
assumptions, and some details of your implementation. Below, you will find the
details of what you should be submitting.

# Source code submission

To submit your xv6 code, run the following from the top level of your xv6 source
tree:

- First, create a file, call it `exclude.txt` and add the following to it:
  ```plain
  *.out.*
  *.out
  __pycache__
  .git
  *.tar
  ```

- Then, clean up your compilation and all object files using:
  ```shell
  $ make clean
  ```

- Create a tarball of your source code as follows:
  ```shell
  $ tar --exclude-from="exclude.txt" -cvf project.tar .
  ```

- You should end up with a file called `project.tar` that contains your source
  code. Please submit that along with your report to Gradescope.

# The project report

Your project report is the vessel through which you would convey your design 
decisisons, your assumptions, your implementation plan, your test cases, and
what worked well and what didn't work well. Here is a section by section
breakdown of what you need to include in your report. This breakdown will
constitute the rubric by which we will be grading your reports.

**Please submit your report in markdown or pdf format only. I do not open MS
Word documents.**

## Introduction

The introuction section should provide an overview of your project and a preview
of your results. You should include reasons why your chose the project you did
and why it is relevant or important for that feature to be part of the xv6
kernel. A bullet list of the highlights of your project would be great to
include as part of the introduction.

At the end of the introduction section, please include, according to your own
assessment, which letter grade do you think you deserve on this project, along
with a short reasoning for it.

## Design

This section should provide a in-depth description of the design of your
feature. For example, if you are implementing signals in xv6, you should
describe what happens when a process sends a signal to another process, how is
that signal delivered, what happens when a signal is delivered, and when does a
process know that is has received a signal (etc.). In other words, describe the
architecture of your added feature and how it interacts with the already
existing parts and services in the xv6 kernel.

In this section, make sure to include the design decision that you had to make,
and the resons why you chose to make those decisions.

Also part of this section, make sure to include any challenges you faced and
how/if you overcame them.

## Assumptions

As this is a mini-project, it is very important that you convey any assumptions
that you made during your design and implementation. For each simplifying
assumption that you made, please include a short discussion of how you would
approach relaxing that assumption and providing support for more features.

## Implementation

In this section, provide a description of how you approached implementing your
design. You should mention any data structures you used/modified, any function
calls you intercepted or added, and any system calls that you added to provide
an interface for users to access your feature. Code snippets would be great here
to help you explain your implementation, especially if you were not able to get
it fully functional.

## Test Cases

In this section, describe the test case(s) you wrote to verify that your
implementation satisfies the requirements that you set out to support.
Specifically, we are looking at test cases that reveal edge cases in your design
and how you handled those. Feel free to include code snippets if those are
useful.

## References

Finally, include any and all references you used in your project, including
documentation, source code, papers, etc.

