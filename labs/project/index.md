---
layout: post
title: Project Description
readtime: true
date: Sun Jan 28 22:30:29 2024
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

<!--
; adding locks and/or condition variables would count as
an extra bonus feature. Your implementation will be finally judged on its
design, efficiency, usability, and any extra/added features.

## Interlude: Hardcore Linux project

For those of you who are really into operating system, you have a chance to opt
into a different, yet much harder project, implemented in the latest version of
the Linux kernel. In that project, you will write a Linux kernel module and
provide an interface to it using the virtual `procfs` filesystem. You have a
bit more freedom in choosing the objective of your module as long as you
satisfy a set of small requirements. If you are interested in this project,
please reach out to your instructor for input, a discussion, and some guidelines.

Please note that this project is by nature significantly harder than anything
you do in xv6. You will need to work on a virtual machine that you are able to
crash and reboot multiple times while developing your code. Expect that you
will need to spend twice to three times as much time on this one compared to
the xv6 multi-threading project.
-->

# Milestone 1: The project proposal

In order to implement a `pthreads`-like library in xv6, you must be able to
first design one. You will do all of your design on paper first before
attempting to write a single line of code; that is the goal of your first
milestone, in which you will submit your project proposal.

## The theory

To get started, you **must** read chapters 3 and 4 of the [xv6
book](https://pdos.csail.mit.edu/6.828/2023/xv6/book-riscv-rev3.pdf); those
chapters are essential for you to understand how to design and implement
lightweight threads in the xv6 kernel. Please do read those chapters carefully
before answering the below questions.

Based on the information in those chapters, please write a design document that
contains answer to all of the questions below:

1. Describe in detail the content of the page table of a process in xv6.

2. Each page table contains two "special" mappings, one called the `trapframe`
   and another called the `trampoline`. What are the uses of those two
   mappings? And where in the page table are they mapped.

3. Looking back at our interrupts lecture, and with insights from chapter 4,
   describe in-depth the process of context switching between one process and
   another in xv6. Make sure to include references to the kernel code, or code
   snippets to support your answer.

4. Given your answers to the above questions, what is the main challenge with
   sharing page tables between processes? In other words, why isn't it a good
   idea for creating threads to have them just share the exact same page table
   data structure?

5. Following up on question 4, can we use the function `uvmcopy` that we used
   in the copy-on-write lab to create a page table for a new thread? Why or why
   not?

Based on the above 5 questions, the most challenging part of this design is to
be able to share the address space between the threads, all the while allowing
each one to have its own separate thread. Additionally, when a thread allocates
new pages of memory, **all other threads** within the same process must be able
to see those new pages of memory.

On a separate level, you'd also need to answer the following questions to be
able to support all of the required features:

1. In RISC-V, how are arguments passed to a function? You can safely assume we
   have a limited number of arguments. You do not need to support a variable
   number of arguments.

2. What registers would you need to manipulate to impact where your thread is
   going to start and where your thread will return to (if it does return) once
   it has completed its execution.

## The design

Based on your answers to the above questions and your understanding of chapters
3 and 4 of the xv6 book, please describe your design of the threads in full
detail. You must be specific about any data structures you will create and/or
modify, any locks you need to add to account for the presence of multiple
processors, any API functions and system calls you add, and so on.

More concretely, you design document must specify the following:

1. How can a user create a new thread. Be specific about the function calls
   and/or input arguments needed to create a new thread.

2. How can a user _join_ (or _wait_ for) a specific thread.

3. Will you maintain any relationships between the threads of the same process?
   Specifically do you need your __main__ thread to know about any of the other
   created threads?

4. Describe how you will have each thread use its own separate stack. What
   happens if the stacks collide? How can you prevent that from happening (you
   do no have to implement this collision check).

5. Describe how to pass arguments to the your thread, be specific as to how
   those arguments are going to be moved from the create call to the thread
   once it starts execution.

6. Describe what happens when a thread terminates (either early or due to a
   normal return). Be specific about what happens to shared pages and how they
   would be maintained.

By reading the design section of your proposal, a reader should be able to come
up with an implementation plan that would satisfy the project's requirements
while adhering to your design guidelines. If you make any assumptions, you must
state those clearly in your design section. If you make such assumptions,
please provide a rationale for it and a suggested method to overcome it if you
had the time to do so.

## The implementation

In this section, you must present a plan to implement your design. We ask you
to adopt an incremental build and test process. In other words, do not attempt
to implement everything all at once. Start with smaller pieces, test those out,
make sure they are working, and then start adding more features.

Be specific about the data structures that you will be using and the functions
that you will be adding/editing to achieve your goals. The more specific you
can be here, the more feedback you can get from your instructor prior to
starting your implementation.

# Milestone 2: Implementing your design

For your second milestone, you are to implement your design. We will judge your
design based on the following criterion:

1. Support for an API to create a thread. The new thread should start at a
   specific location with its own thread.

2. Support for an API to create a thread that can start at an arbitrary
   user-specified code location and having its own stack.

3. Support for an API to create a thread that can take one argument.

4. Support for an API to create a thread that can take more arguments, upper
   bounded by a number that you choose.

5. The created threads share the address space when reading. In other words,
   the same address in all threads will read the same value.

6. The created threads share the address space when writing. In other words, if
   one thread modifies a memory location, all other threads will see the
   impacts of that change. No need to worry about synchronization here.

7. If one thread allocated and maps a new page, all other threads must be able
   to access that page.

   This one is challenging, you be judged by the creativity of your solution as
   well as its efficiency and impact on performance.

8. Each thread has its own separate stack.

9. The threads' stacks do not overlap.

10. If a thread dies, its state is cleared up without causing any memory leaks
    and/or memory corruption.

11. If the main process dies, then all threads associated with it will die,
    i.e., we do not do any reparenting.

12. Each feature has one or more test cases.

13. (EXTRA) Provide synchronization means using locks and/or condition
    variables.

# Deliverables

At the end of the project, please submit the following:

1. Your modified `xv6` source code.

2. Your updated design document including any changes from the first milestone,
   as well as documentation on how to use your code. Your design document must
   be in either **markdown** (.md) or **pdf** format.

   **Design document submitted as MS word documents will be ignored**.

3. A brief (5 minutes) video presentation of the features of your design along
   with a demo of your threads in action. Think of this as an elevator pitch
   for your project, highlighting where it stands strong and showcasing those
   in practice using the demo.
   
4. A file called `grade.txt` in which you state what grade you think you
   deserve on this project, arguing why you think so. Find the **Grade
   Descriptions** section of [Rose-Hulman - Rules and Procedures -
   Grades](https://www.rose-hulman.edu/campus-life/student-services/registrar/rules-and-procedures/grades.html)
   for guidance.

<!--
# Ideas out there

- Implement `pthreads` in the xv6 kernel.

Requirements:

- Threads must share all the address space, including newly added pages, if
  any.

- Sharing of pages must be as efficient as possible.

- Simply assigning the page table of the child to that of the parent will not
  work, ask to find out why?

- How do we set the start of the newly created thread to a new function? How do
  we pass arguments to that function?
  - Assume we only pass a single argument.

- Must add a new system call

- Must develop test cases

- Must show design document, pretty much answering these questions.

- Must develop plan of attack, including when test cases will be developed and
  how.
-->
