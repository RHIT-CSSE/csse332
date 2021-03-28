---
layout: post
title: Session 11 IPC
readtime: true
date: Sat Mar 27 22:44:15 2021
gh-repo: rhit-csse332/csse332-202130
gh-badge: [star,watch,follow]
---

# Introduction and aim 
In this session, we will spend more time with Professor Song talking about _Inter Process
Communication_ (IPC). We introduce two techniques for sharing information and data between two or
more processes: (1) shared memory and (2) message passing. Shared memory is generally managed by the
processes themselves, and thus provide faster means of communication while risking introduction
concurrency issues and race conditions (more on those later). Message passing (e.g., pipes) are
managed by the kernel and thus require less management skills from the processes themselves at the
cost of sacrificing some performance. 

## Logistics
* [Slides](https://rosehulman-my.sharepoint.com/:p:/g/personal/noureddi_rose-hulman_edu/ESN8gjpua5VEtGunxJO4r64BFF263ZnpIaBJwNuUl4UxTA?e=n1ATTO)
* [Activity files]({{ site.gh_repository_url }}/tree/main/activities/11_ipc)
* [Activity submission](https://moodle.rose-hulman.edu/mod/assign/view.php?id=2708145)
* [Clear/Muddy questions](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2772321)

### Outline
* [Overview and shared memory](#overview-and-shared-memory) ~ 27 mins
* [Shared memory activity](#shared-memory-activity) ~ 12 mins
* [Message passing and its activity](#message-passing-and-its-activity) ~ 19 mins
* [Conclusion](#conclusion)

# Overview and shared memory
In this video, we will introduce the need for inter process communication and describe two methods
for achieving that:
  - Shared memory, where the processes share and manage an allocated area of memory.
  - Message passing, where the processes ask the OS to provide them with a mechanism to send and
  receive messages (data or packets) between each other. 

We then dive further into the shared memory mechanism and describe the syntax for achieving it in
Unix systems. 

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=c2569fb5-5704-49f3-bec3-ac8b012834be&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# Shared memory activity
In this video, we provide you with an introduction and a walk through of the main activity for this
session, which is to shared memory between processes to compute a dummy application. 

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=8d02a518-2cd4-4d90-af61-ac8b0133e6ee&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# Message passing and its activity
In this video, we dive into message passing, and discuss pipes in more detail (we have already seen
those in our process lab homework assignment). We will then go through the activity where we would
have to change our implementation from one using shared memory to one using pipes. 

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=dc03c974-4e77-4fc1-a92a-ac8b014117df&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# Conclusion
In this session, we introduce two forms of inter process communication (IPC). Due to the fact that
process' memory regions are isolated, we need mechanisms to allow them to communicate and share
information. We introduce shared memory as well as message passing as two techniques to achieve that
goal. We also discuss the advantages and disadvantages of each mechanism and walk you through our
in-class activity. 

As usual, remember to submit your activity files
[here](https://moodle.rose-hulman.edu/mod/assign/view.php?id=2708145) and your clear/muddy survey
[here](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2772321).

