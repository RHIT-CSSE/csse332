---
layout: post
title: Session 3 Process Basics
readtime: true
date: Sun Mar 14 21:39:03 2021
gh-repo: rhit-csse332/csse332-202130
gh-badge: [star,watch,follow]
---

# Introduction and aim
In this session, we will define what a process is and what a thread is. We will discuss how the
operating system represents processes using _process control blocks_ and examine how context
switching happens (i.e., what happens when we called `swap_context`). There is short quiz at the end
of the session in addition to our normal clear/muddy survey.

## Logistics
* [Slides](https://rosehulman-my.sharepoint.com/:p:/g/personal/noureddi_rose-hulman_edu/EdPw3KFKdB5MvhzLp7bUTq8BElTXlbO3eizdxldTsPXxUQ?e=Uj7e5F)
* [Class Notebook Section](https://rosehulman-my.sharepoint.com/personal/noureddi_rose-hulman_edu/_layouts/OneNote.aspx?id=%2Fpersonal%2Fnoureddi_rose-hulman_edu%2FDocuments%2FClass%20Notebooks%2FCSSE%20332%20Operating%20Systems&wd=target%28_Content%20Library%2FSession%203.one%7CCE286D05-5A0D-4302-9C95-64DD1309310B%2F%29 onenote:https://rosehulman-my.sharepoint.com/personal/noureddi_rose-hulman_edu/Documents/Class%20Notebooks/CSSE%20332%20Operating%20Systems/_Content%20Library/Session%203.one#section-id={CE286D05-5A0D-4302-9C95-64DD1309310B}&end)
* [Quiz](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2708127)
* [Clear/Muddy Questions](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2760012)

### Outline
1. [Motivation](#motivating-example) ~ 7 minutes
2. [Definition of a process](#processes-a-definition) ~ 11 minutes
3. [Definition of a thread](#threads-a-definition) ~ 9 minutes
4. [Context switching](#context-switching) ~ 8 minutes
5. [The PCB and process states](#the-pcb-and-process-states) ~ 10 minutes
6. [Conclusion](#conclusion) ~ 4 minutes

# Motivating example
We kick things off with a small motivating example: what happens when your instructor is trying to
do some work prepping for the CSSE332 sessions?

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=6325ace1-6535-4386-8932-aceb01764d44&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# Processes: A definition
In this video, we define what we do actually mean when we say process. In short, a process is
nothing but a program __in execution__. We examine how a process is loaded from a given program (or
binary) and then look at what memory segments are needed for executing a process.

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=bd32cc77-0f3f-40fa-b6e5-aceb017c32f9&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# Threads: A definition
After defining processes, we take a look at threads, a more ''light-weight'' units of execution that
share an address space with the process that spawns them, and withing themselves. We end the video
with a small demo in which we look at the memory segments of a process in action.

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=126a945c-df0e-4b92-b702-aceb0186183f&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# Context switching
After setting up our definitions, we examine what do we when we say context switching. What happens
when we must switch from one process or thread to another. 

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=343f6eef-d7c7-4de2-bb67-acec0002478b&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# The PCB and the process states
Finally, we introduce the process control block (PCB) that the operating system uses to maintain all
the information of a certain process. We examine Linux's `task_struct` as an example and check out
its contents. 

We then take a look at the states in which a process can be. We see that a process can be in one of
three states:
* `RUNNING`: When the process is actively running on the CPU
* `READY`: When the process is ready to executes but does not have ownership over the CPU
* `WAITING`: When the process is waiting for a certain even to happen
We finally see those states in action by examining a small demo.

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=e30b017b-3891-4d4e-910d-acec00083541&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# Conclusion
We finally conclude with a quick recap of what we discussed in this session. Please don't forget to
do the following:
* Take the short quiz about the session's contents [here](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2708127)
* Fill out the clear/muddy survey [here](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2760012)

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=516b2ca0-c57f-410c-9f18-acec000be909&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# Advanced Lab 1 Intro
Professor Song has prepared a short intro for you for our first advanced lab. You can find it
[here](https://rose-hulman.hosted.panopto.com/Panopto/Pages/Viewer.aspx?id=7b3e6a5d-d200-4ca9-8f24-ac52016e6c1b)
