---
layout: post
title: Session 13 Scheduling I
readtime: true
date: Wed 31 Mar 2021 01:03:43 PM EDT
gh-repo: rhit-csse332/csse332-202130
gh-badge: [star,watch,follow]
---

# Introduction and aim
In this session, we will start our discussion of scheduling and scheduling policies. We will first
introduce the need for scheduling, and then present some criteria for comparing different scheduling
policies. We then discuss 4 policies:
1. First-In-First-Out (FIFO)
2. Shortest Job Fist (SJF)
3. Shortest Time to Completion First (STCF)
4. Round Robin (RR)
We will also present the main challenge when it comes to FIO and SJF, which is the convoy effect,
where a process with long runtime can monopolize the CPU and prevent other processes from running. 

__Note__ that this session turned out to be longer than expected, we had a lot to cover so I will
compensate for that on Monday since we will not have a session or an activity due. 

## Logistics
* [Slides](https://rosehulman-my.sharepoint.com/:p:/g/personal/noureddi_rose-hulman_edu/Ee-B7saLFd9BgEagElQ3Ep8BeUwNrWACD_XFk5S_4ygBzw?e=IUzTSW)
* [Class notebook section](https://rosehulman-my.sharepoint.com/personal/noureddi_rose-hulman_edu/_layouts/15/Doc.aspx?sourcedoc={8fbe2227-3fab-4305-b588-a6b50b37e367}&action=edit&wd=target%28_Content%20Library%2FScheduling%20I.one%7Cda254590-7b5e-4455-8da9-22ef40849e9f%2F%29&wdorigin=717) 
* [Activity submission](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2708148)
* [Clear/Muddy questions](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2776317)

### Outline

* [Motivation](#motivation) ~ 11 minutes
* [Introduction](#introduction) ~ 13 minutes
* [FIFO scheduling](#fifo-scheduling) ~ 12 minutes
* [Shortest job first](#shortest-job-first) ~ 10 minutes
* [Activity walkthrough I](#activity-walkthrough-i) ~ 8 minutes
* [Shortest time to completion first](#shortest-time-to-completion-first) ~ 9 minutes
* [Round robin](#round-robin) ~ 11 minutes
* [Activity walkthrough II](#activity-walkthrough-ii) ~ 10 minutes
* [Conclusion](#conclusion)

# Motivation
In this first video, we go back to our lame motivation stories and motivate the need for good
scheduling policies.

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=0aa0c310-7cd2-4f15-930c-acfb0155f6fa&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# Introduction
In this video, we make out formal introduction of scheduling and scheduling policies. We also
introduce criteria for comparing different scheduling policies, namely:
* Turnaround time: Completion time - arrival time: How long does the process need to complete its
  activities from the time if arrives into our system.
* Response time: Time for first run - arrival time: How soon after arriving does a process get to
start executing.

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=eaa359be-b888-4ea6-bd08-acfc00d0da93&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# FIFO scheduling
In this video, we take our first steps into the world of scheduling policies and discuss the
_First-In-First-Out_ policy. We also discuss one problem with FIFO scheduling, namely the __convoy
effect__.

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=4bb94808-8360-4774-bed3-acfc00d80c20&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# Shortest job first
In this video, we present the _shortest job first_ (SJF) policy as a solution to the convoy problem
presented in the previous video. We also show how SJF is also susceptible to the convoy effect even
long running processes are "smart".

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=91fed370-29a6-4d8b-8328-acfc00de6d44&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# Activity walkthrough I
In this video, I walk you through the solution of the first half of our activity (which is a quiz on
Moodle). 

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=81391546-04df-4507-978e-acfc00fcf971&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# Shortest time to completion first
Next, we introduce the _shortest time to completion first_ (STCF) scheduling policy as a potential
solution to the convoy effect we witnessed in the first two policies. STCF is a preemptive
scheduling policy, i.e., the scheduler can yank a running process off of the CPU and replace it with
another process using a context switch.

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=4fa4d3c1-6615-4a1d-8385-acfc010005a8&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# Round robin
As an alternative to STCF, a _round robin_ (RR) scheduler runs each process for a fixed amount of
time (referred to as the __quantum__ of the policy). RR is a _fair_ scheduling policy that allocates
each process a fixed share of the CPU. However, if not careful of your choice of the quantum, the
overhead of doing frequent context switches can impede the performance of an RR scheduler. 

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=83f2c94a-6118-4cab-b3cb-acfc01037a4f&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# Activity walkthrough II
Finally, we walk through the second half of our activity and apply a RR scheduling policy to two
examples with different quanta.

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=39cbf533-46de-4c7e-9b60-acfc01089478&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# Conclusion
In this session, we introduced the concept of a scheduler and a scheduling policy, and explored
different criteria for comparing them. We introduce 4 policies: FIFO, SJF, STCF, and RR. 

Please make sure you submit the in-class activity as a
[quiz](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2708148) on Moodle (you are allowed
infinite resubmissions) and your [clear/muddy
questions](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2776317).
