---
layout: post
title: Exam 2 Preparation
readtime: true
date: Tue Jan 24 2023 
---

# Mohammad's Unreliable Guide to Exam 2

This is heavily based on Buffalo's guide for Exam 2 from previous quarters,
modified a bit to reflect some changes for this quarter.


| Last Modified | Winter 22-23 |
| :------------ | ------------ |

# Exam Format and Structure

This exam will be a timed take home exam where you will have to choose a 3 hours
block of time to take the exam anywhere that is most convenient to you. Similar
to Exam 1, you will be asked to clone a password-protected git repository. You
will get the password from Moodle when you open the submission box.

**Please note that once you complete the exam, you should submit your code to
BOTH Moodle and Gradescope**. It would make our life a lot easier to grade if we
have access to your code in both places. If you skip submitting to gradescope,
you risk us not seeing your solution.

In general, there will be 4 problems in this exam:

1. 3 condition variables problems worth 70 points.

2. 1 thread creation problem worth 30 points. This problem might be split into
   two parts if needed.

At the end of the exam, you should submit **all** of the `.c` files that you
have changed. Feel free to include your thought process in comments in the code
to help us better understand your approach and give you partial credit whenever
applicable.

# General Remarks

- Time management is part of the exam, so keep an eye on your time.

- The problem should generally be independent though of varying difficulty. Feel
  free to attempt a different problem if you get stuck on a particular one.

- Keep backups of your code. If you have a partially working solution and would
  like to attempt something new, it might be worth it to backup the old one
  before making new changes.

- Think in terms of patterns, and try to remember if we solved something similar
  in class. You have full access to the course website and any code that we have
  solved together in class.

- It helps me personally to imagine a real-world analogy of the concurrency
  problem I am trying to solve. I tend to ask myself questions like: "If I were
  to solve this problem in real-life, how would I organize things?"

## A Note on External Resources

- Generally, problems you will have to solve are novel so Googling them is not a
  good idea as it often can be misleading. If you have questions, your
  instructor and the guidance sheet are your best source of information.

- [New in 2023] I know that you might be tempted to use ChatGPT to solve your
  problem. My advice is simply to stay away from it. Here is my reasoning:
  - The goal of you taking the class is to learn how to think about these kinds
    of problems, and not actually to just write a solution. You are not being
    tested on whether you can use the Internet (or the AI or whatever) to copy
    and paste a solution.
  - You are doing yourself a disservice on the long term. Being an engineer is
    about problem solving and not about solution copying.
  - I did experiment with ChatGPT a bit and so far, it has been 0/4 on the
    concurrency problems I have asked it to solve. So even if it spits out a
    solution, there are no guarantees that it will actually work.
  - Finally, it is generally very easy for us to tell whether you really
    understand your attempted solution (even if it doesn't work) compared to
    when you just copy it from somewhere else.

# Required Skills

## Thread Creation

- You will be asked to parallelize a sequential program by creating threads at
  the appropriate time, and joining them.

- Remember how you can pass parameters to the threads:
  - Thread functions accept `void *` arguments, so pretty much you can pass
    anything in there assuming you properly cast it afterwards.
  - If you need more than one argument, you can use a `struct`.

- Recall what we talked about in class. The iterator variable in a `for` loop
  will disappear after the loop terminates, similarly for variables created
  inside the body of the loop, so passing those as parameters to thread
  functions is a pretty bad idea.

- If you need synchronization at this point, a mutex lock is often enough, but a
  condition variable might be needed.

## Concurrency Problems


