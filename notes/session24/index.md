---
layout: post
title: More Concurrency Exercises
readtime: true
date: Mon Jan 23 2023 
---

# Learning Objectives

At the end of this lecture, you should be able to:

- Use mutex locks and condition variables to solve more concurrency problems.

---

# Topics

In this lecture, we will cover the following topics:

- Mutex locks.
- Condition variables.
- The sleeping barber's problem.

---

# The Problem

- The sleeping barber is a famous concurrency problem that illustrates some of
  the issues when writing multi-threaded code, especially when dealing with
  multithreaded operating system.

- So here is the setup of the problem:
  1. We have one barber that owns a barber shop.

  2. There is one work chair where the barber can actively cut a customer's hair.

  3. There are `NUM_WAITING_CHAIRS` waiting chairs in the waiting area.

  4. If there are no customers in the shop, the barber will fall asleep on the
     work chair.

  5. When a customer arrives, if the barber is asleep, the customer will wake
     the barber up and get a haircut.

  6. Alternative, if the barber is busy, the customer will enter the waiting
     area. Two cases might arise as well:
     - If the waiting area is full, then the customer gets pissed and leaves.
     - If there is an empty chair in the waiting area, the customer will join
       the waiting queue.
