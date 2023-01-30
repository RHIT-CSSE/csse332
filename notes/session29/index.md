---
layout: post
title: Scheduling I
readtime: true
date: Sun Jan 29 2023
---

# Learning Objectives

At the end of this lecture, you should be able to:

- Define a scheduling policy and the requirements it must achieve.
- Evaluate a scheduling policy using turnaround and response times.
- Identify different scheduling policies, their advantages and their
  shortcomings.

---

# Topics

In this lecture, we will cover the following topics:

- CPU scheduling and scheduling policies.
- Turnaround and response times.
- First-In-First-Out, Shortest Job First, Shortest Time to Completion First, and
  Round Robin scheduling.

---

# Notes

<!-- vim-markdown-toc GFM -->

* [Motivating Analogy](#motivating-analogy)
* [Definitions](#definitions)
  * [Scheduling Policies](#scheduling-policies)
  * [Evaluating a Scheduling Policy](#evaluating-a-scheduling-policy)
  * [Assumptions](#assumptions)
* [FIFO Scheduling](#fifo-scheduling)
  * [The Convoy Effect in FIFO](#the-convoy-effect-in-fifo)
* [Shortest Job First (SJF)](#shortest-job-first-sjf)
  * [The Convoy Effect in SJF](#the-convoy-effect-in-sjf)
* [Shortest Time to Completion First (STCF)](#shortest-time-to-completion-first-stcf)
  * [Fairness](#fairness)
* [Round Robin (RR)](#round-robin-rr)

<!-- vim-markdown-toc -->

# Motivating Analogy

- Imagine you are running a kiosk for answering questions about the OS class
- How would you schedule the students that come in to ask questions?
- What if you had a very annoying student, call him Robert, that hogs your time,
  how would you handle that?
  - Hire a bouncer, sort students by their number of questions
- Well what if Robert arrives very early, before everyone else shows up? 
- Even worse, imagine if your crush arrives to ask question and Robert is still
  asking questions, what would you do?
  - Kick Robert out! —> preemption!
- We also might like to give priority to some students over others
  - Put them in a priority queue
- Finally, if you are a social justice warrior, then you want to give everyone
  an equal shot
  - Round Robin
- This is exactly the same questions that OS developers had to deal with when
  designing the scheduler for an operating system

# Definitions

- We have a shared limited resource that must be shared among several processes
  or threads (or jobs or tasks, etc.)

## Scheduling Policies

- Scheduling:
  - Figuring out how to share a limited resource among many tasks
- Scheduling policy:
  - Process of determining who gets to run when
- Factors that affect a policy:
  1. The number of tasks to run.
  2. The run time of each task.
  3. The arrival time of tasks.
  4. The I/O patterns of the tasks.
      1. Heavy on I/O operations.
      2. CPU intensive (`while(1)`).
  5. Are we allowed to kick tasks off of the CPU?
      1. Preemption

## Evaluating a Scheduling Policy

- We will use two metrics to evaluate a scheduling policy.
  1. The **turnaround** time:
    - How long does it take to complete the task?
    - T<sub>turnaround</sub> = T<sub>completion</sub> - T<sub>arrival</sub>

  2. The **response** time:
    - When is the task first ran given when it arrived?
    - T<sub>response</sub> = T<sub>first run</sub> - T<sub>arrival</sub>

## Assumptions

- When designing our scheduling policies, we will make (and then relax) the
  following assumptions:

  1. Each job runs for the **same amount of time**.
  2. All jobs **arrive at the same time**.
  3. Once started, **each job runs to completion**.
  4. All jobs **only use the CPU** (no I/O).
  5. The **runtime** of each is known beforehand.

# FIFO Scheduling

- We have three jobs: A, B, and C
  - A needs 3 time units
  - B needs 3 time units
  - C needs 3 time units
  
- Show the diagram and calculate the turnaround and response times.

## The Convoy Effect in FIFO

- Let’s relax the first assumption, what do you think can happen in that case?

  1. ~~Each job runs for the **same amount of time**.~~
  2. All jobs **arrive at the same time**.
  3. Once started, **each job runs to completion**.
  4. All jobs **only use the CPU** (no I/O).
  5. The **runtime** of each job is **known** beforehand.
 
- A long-running process can hog the CPU and prevent other processes form
  running, even if they have a short runtime.
- This is called the **convoy** effect:
  - Imagine the person that drives slowly on the left lane, yikes!

# Shortest Job First (SJF)

- ❓ To address the challenge of the FIFO scheduler, what would you suggest?
  - Run the job with the shortest amount of time first.
- So order the jobs by increasing runtime in a priority queue.
- Show diagram and compute the turnaround and response time.

## The Convoy Effect in SJF

- Let’s relax the second assumption, what do you think can happen in that case?

  1. ~~Each job runs for the **same amount of time**.~~
  2. ~~All jobs **arrive at the same time**.~~
  3. Once started, **each job runs to completion**.
  4. All jobs **only use the CPU** (no I/O).
  5. The **runtime** of each job is **known** beforehand.

- ❓ What could happen in this case?
  - Another convoy effect if the longest running process arrives before anyone
    else.

# Shortest Time to Completion First (STCF)

- To address the challenges of the SJF scheduling policy, we need to relax the
  third assumption

  1. ~~Each job runs for the **same amount of time**.~~
  2. ~~All jobs **arrive at the same time**.~~
  3. ~~Once started, **each job runs to completion**.~~
  4. All jobs **only use the CPU** (no I/O).
  5. The **runtime** of each job is **known** beforehand.

- We will introduce the idea of preemption to solve this challenge.
- We prioritize the jobs that have the least amount of time remaining to
  complete.
  - Thus the name shortest time to completion first
  
- If a new task arrives that needs less time to complete than the currently
  running one:
  - Preempt the currently running job and put it back in the queue.
  - Schedule the newly arriving job to run.

- Show the diagram and compute the turnaround and response time

## Fairness

- ❓ What if we a whole bunch of short tasks and a few long tasks?
  - What could happen to the long tasks?
  - This is not a fair system!

# Round Robin (RR)

- Run each job for a fixed time slice called a **quantum**.
- This can help us achieve fairness.

