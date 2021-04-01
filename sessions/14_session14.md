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
* [Class notebook section]()
* [Activity submission](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2708151)
* [Clear/Muddy survey](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2776320)

__Note__ that the activity and the clear/muddy survey are due by the end of the day on Saturday
April 3, 2021. 

# Motivation
In this video, we go through a quick recap of the STCF and Round Robin scheduling policies and we
take a look at their shortcomings. We use that to start thinking about MLFQ as a solution that
brings about the best of the both worlds. 

# Introduction
In this video, we go through a brief introduction to MLFQ and list our first set of rules for
managing the scheduling of processes on the CPU. The rules so far are the following:
1. If `priority(A)` > `priority(B)`, then `A` runs.
1. If `priority(A)` == `priority(B)`, then `A` and `B` run in Round Robin fashion
1. Initially, all jobs go to the highest priority level
1. If a job uses up its entire time slice, reduce its priority (i.e., move it down one queue)
1. If a job voluntarily yields before a time slice ends, it stays in the queue. 

# Activity walkthrough
In this video, I will walk you through the activity for this session.

# Challenges
In this video, we present three challenges that might make our MLFQ system go wrong. Those
challenges are:
1. Starvation
1. Gaming the system (i.e., malicious process)
1. Process changes its behavior from CPU intensive to I/O intensive. 

# Priority boosting
In this video, we introduce priority boosting as a solution to challenges (1) and (3) above. 

# Improved accountability
In this video, we introduce the idea of accountability to prevent malicious processes from
manipulating our scheduling policy. Our goal is to maintain fairness and best serve I/O bound
processes. 


# Conclusion
In this session, we introduce the MLFQ scheduling policy as an integration of the benefits of RR
and STCF. We went through the stages of building our MLFQ scheduler and then presented the
challenges that come with doing that. We then introduce priority boosting and accountability as a
solution to address those challenges. 

Please make sure you submit the in-class activity as a
[quiz](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2708151) on Moodle (you are allowed
infinite resubmissions) and your [clear/muddy
questions](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2776320).
