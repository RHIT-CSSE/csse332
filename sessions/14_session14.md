---
layout: post
title: Session 14 Scheduling II
readtime: true
date: Thu Apr  1 17:42:27 2021
gh-repo: rhit-csse332/csse332-202130
gh-badge: [star,watch,follow]
---

# Introduction and aim
In this session, we will continue our discussion of scheduling policies and discuss the
_Multi-Level Feedback Queue_ (MLFQ). We will build up to our final results by considering simpler
designs and then building up more sophisticated solutions.

## Logistics
* [Slides](https://rosehulman-my.sharepoint.com/:p:/g/personal/noureddi_rose-hulman_edu/EVof4XRz43xPu_ZLvAbbDd8B66m7KGXsFfJuVfPPUOu82g?e=WYxcTN)
* [Class notebook
section](https://rosehulman-my.sharepoint.com/personal/noureddi_rose-hulman_edu/_layouts/OneNote.aspx?id=%2Fpersonal%2Fnoureddi_rose-hulman_edu%2FDocuments%2FClass%20Notebooks%2FCSSE%20332%20Operating%20Systems&wd=target%28_Content%20Library%2FSession%2014.one%7C7A4EA5D6-BBAA-40A5-B36E-FBDECE6349D6%2F%29)
* [Activity submission](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2708151)
* [Clear/Muddy survey](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2776320)

__Note__ that the activity and the clear/muddy survey are due by the end of the day on Saturday
April 3, 2021. 

This session has about 43 minutes of lecture time and an additional 9 minutes if you'd like me to
walk through the activity with you. 

# Outline
* [Motivation](#motivation) ~ 9 minutes
* [Introduction](#introduction) ~ 16 minutes
* [Activity walkthrough](#activity-walkthrough) ~ 9 minutes
* [Challenges](#challenges) ~ 6 minutes
* [Priority boosting](#priority-boosting) ~ 5 minutes
* [Improved accountability](#improved-accountability) ~ 7 minutes
* [Conclusion](#conclusion)


# Motivation
In this video, we go through a quick recap of the STCF and Round Robin scheduling policies and we
take a look at their shortcomings. We use that to start thinking about MLFQ as a solution that
brings about the best of the both worlds. 

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=82a89d1b-8693-47b2-bb11-acfd018b2386&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# Introduction
In this video, we go through a brief introduction to MLFQ and list our first set of rules for
managing the scheduling of processes on the CPU. The rules so far are the following:
1. If `priority(A)` > `priority(B)`, then `A` runs.
1. If `priority(A)` == `priority(B)`, then `A` and `B` run in Round Robin fashion
1. Initially, all jobs go to the highest priority level
1. If a job uses up its entire time slice, reduce its priority (i.e., move it down one queue)
1. If a job voluntarily yields before a time slice ends, it stays in the queue. 

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=4da74c19-e409-4a36-8f5d-acfe00039866&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# Activity walkthrough
In this video, I will walk you through the activity for this session. This video is optional, you
can skip it until you are ready to go over the activity. 

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=212b216d-7ca4-4978-8340-acfe0013cbdc&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# Challenges
In this video, we present three challenges that might make our MLFQ system go wrong. Those
challenges are:
1. Starvation
1. Process changes its behavior from CPU intensive to I/O intensive. 
1. Gaming the system (i.e., malicious process)

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=210f2fc1-3dda-4ddc-85f5-acfe000947f9&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# Priority boosting
In this video, we introduce priority boosting as a solution to challenges (1) and (3) above. 

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=6a5f4f56-c6ba-4627-bbda-acfe000b58ea&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# Improved accountability
In this video, we introduce the idea of accountability to prevent malicious processes from
manipulating our scheduling policy. Our goal is to maintain fairness and best serve I/O bound
processes. 

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=cec712be-f890-401f-b502-acfe000d6f01&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# Conclusion
In this session, we introduce the MLFQ scheduling policy as an integration of the benefits of RR
and STCF. We went through the stages of building our MLFQ scheduler and then presented the
challenges that come with doing that. We then introduce priority boosting and accountability as a
solution to address those challenges. 

Please make sure you submit the in-class activity as a
[quiz](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2708151) on Moodle (you are allowed
infinite resubmissions) and your [clear/muddy
questions](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2776320).
