---
layout: post
title: Project Milestone 2
readtime: true
date: Mon Aug 12 2024
---

# Introduction

In this project, we would like to add a new feature to the xv6 operating
system. Specifically, the current implementation of xv6 provides support only
for processes, each with its own separate address space. That is enough to
implement basic features like a simple shell, however, to make use of the
available processors, we'd definitely need to share memory between lightweight
threads. Therefore, we will make it our task in this project to add support for
`pthread`-like threads in xv6.

In our context, two threads will live in the same process address space but
each will have the following:

1. Its separate set of registers, including its own program counter. This means
   that each thread can be executing in a different spot in the program's
   source code.
2. Its separate stack (the stack will live in the same address space, they
   will just be at separate locations).
3. Each thread is a separate schedulable entity. In other words, if there
   are two processors available, then both threads can be actively running at
   the same time.

To support creating and manipulating threads, we would also like to provide an
API for our users to create and join threads, in a way similar to what the
`pthreads` library provides. For the base project, you do not need to worry
about synchronization.

# Final Project

For your final project, we require the following features in the multi-threaded
version of `xv6` that you have developed.

1. Support for an API to create a thread that can start at an given function
   pointer and has its own stack.

2. Support for joining threads, depending on how you decide to represent a
   thread. Joining should wait a specific thread, _not any thread_.

3. Support for an API to create a thread that takes one (or more) arguments.
   The requirement is only for one argument, but feel free to add more if you'd
   like.

4. The created threads share the address space when reading & writing. In other
   words, the same address in all threads will read the same value. If one
   thread modifies a memory location, all other threads will see the impacts of
   that change. No need to worry about synchronization here.

5. _If one thread allocated and maps a new page, all other threads must be able
   to access that page._

6. If a thread finishes, its state is cleared up without causing any memory
   leaks and/or memory corruption. (A good implementation of step 2 above will
   give you this one for free).

## Additional features

In addition to the above features, anything you add from the list below will
give you extra points:

1. If the main process dies, then all threads associated with it will die,
   i.e., we do not do any reparenting.

2. Thread functions are indirectly redirected to an exit function (that you
   defined) if they do not call return (or even if they do).

3. Support for more than one argument for thread creation.

4. Creative implementation of requirement (4) above.

5. Good documentation, making it easy for me to understand what you have done
   in the project.

6. Any other feature that you think is interesting, as long as it is well
   documented will earn you extra credits.

# Deliverables

At the end of the project, please submit the following:

1. Your modified `xv6` source code. **Please run `make clean` before you
   compress your source code, to avoid pushing any object files or binaries.
   Gradescope will reject any of those.** 

2. An updated design document (see `design.txt` from milestone 2) documenting
   all of your decisions.

   **As usual, I do not read .docx documents, so please submit plaintext or
   pdf or markdown.** 

3. A file called `grade.txt` where you argue for which _letter_ grade you
   should receive for this project. The better argument you make, the more
   convincing it is going to be for more to honor it.

   Generally, I have found out that students are more often than not accurate
   in representing their efforts and do provide good estimates of where their
   grades should lie.

4. Instructions for how to run and test your code, which test cases test which
   features, etc.

## Additional deliverables

If you'd like, please provide a link to a quick 5 minutes video (place it in a
file called `video.txt`) where you discuss your design, your features, and
provide a brief demo.

The video is completely optional, but is a good way to argue for your project
in case you were not able to complete everything. You can discuss the
challenges you faced, and what features are working and which are missing.

# Submission

Please submit one `zip` or `tar` file of your project to Gradescope.

