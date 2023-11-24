---
title: Syllabus
layout: post
readtime: true
date: Fri 24 Nov 2023
---

## Table of Contents

<!-- vim-markdown-toc GFM -->

* [Course Description](#course-description)
* [Prerequisite Topics](#prerequisite-topics)
* [Learning Objectives](#learning-objectives)
* [Textbook](#textbook)
  * [Recommended textbook](#recommended-textbook)
  * [Other great references](#other-great-references)
  * [Other References](#other-references)
  * [Electronic Course Materials](#electronic-course-materials)
* [Deadlines and Late Policy](#deadlines-and-late-policy)
* [Special Arrangements](#special-arrangements)
* [Homework Assignments/Labs](#homework-assignmentslabs)
* [Exams](#exams)
* [Grading](#grading)
* [Handling Grading Issues](#handling-grading-issues)
* [Collaboration, Cheating and Professional Behavior](#collaboration-cheating-and-professional-behavior)
  * [Plagiarism and General Misconduct](#plagiarism-and-general-misconduct)
  * [Self-plagiarism](#self-plagiarism)
  * [Generative Artificial Intelligence](#generative-artificial-intelligence)
  * [Collaboration](#collaboration)
  * [Summary](#summary)
* [A statement on mental health](#a-statement-on-mental-health)
* [Miscellaneous](#miscellaneous)
  * [Syllabus changes](#syllabus-changes)
  * [Attendance policy](#attendance-policy)
  * [Illness](#illness)
  * [Laptop policy](#laptop-policy)
* [Acknowledgments](#acknowledgments)

<!-- vim-markdown-toc -->

## Course Description

In this course, we will dive deep into the mechanisms by which computer systems
manage and distribute resources using the services of an _operating system_. You
will learn how programs are turned into system processes and then are scheduled
to run on the CPU. We will explore topics related to memory and file-system
management as well as security and privacy from the system's administrator point
of view. Finally, in this class, you will have the opportunity to interact with
a real-world, yet small-scale, operating system. You will implement several
operating system features that will solidify the concepts that we discuss in
class.

If you are wondering why you are required to take this class, please take a
moment to read the [unofficial syllabus]({{ site.baseurl }}/docs/unofficial)
where we try to convince you of the importance of knowing the concepts in this
class.

## Prerequisite Topics

At the start of this class, we assume that you are familiar with the C
programming language and are well on your way to become an independent
programmer. Officially, this means that you should have passed CSSE-132 and
CSSE-220.

<!--
However, to make the best out of this class, we strongly recommend
that you have taken, or are familiar with the topics presented in CSSE 232
(Computer Architecture I). A lot of the topics we cover will make more sense if
you have the background knowledge discussed in an architecture class.
-->

## Learning Objectives

After completing this class, you should be able to:

1. Leverage operating system services to create, manage, terminate, and schedule
   processes.
2. Define the main memory abstraction and implement operating system features to
   achieve memory virtualization using paging.
3. Describe different approaches to file-system organization and build a
   working system that can read, write, and manage files.
4. Identify the need for synchronization in multi-threaded environments, and
   write code that properly uses synchronization primitives.
5. Implement operating system features that satisfy a requirements
   specification using an appropriate programming language.

## Textbook

### Recommended textbook

* Operating Systems: Three Easy Pieces, version 1.00, Remzi H. Arpaci-Dusseau
  and Andrea C. Apraci-Dusseau, August 2018.
  * You can obtain a free copy of the textbook at: [Operating Systems: Three
    Easy Pieces](https://pages.cs.wisc.edu/~remzi/OSTEP/)

> Unless otherwise specified, all Chapters and Sections listed on the schedule
  refer to the above textbook and chapters.

### Other great references

* Modern Operating Systems, 4th Edition, Andrew S. Tanenbaum, Herbert Bos,
  Pearson Education, 2015.

* Linux kernel development, Robert Love, Pearson Education, 2010.

* Operating System Concepts, 10th Edition, Abraham Silberschatz, Greg Gagne, and
  Peter B. Galvin, Wiley, 2018.

### Other References

* C Programming Language, 2nd Edition, Brian W. Kernighan and Dennis M. Ritchie,
  Prentice Hall, 1988.

* C: The Complete Reference, 4th Edition, Herbert Schildt, McGraw-Hill Osborne
  Media, 2000.

* Advanced Programming in the UNIX Environment, 2nd Edition, W. Richard Stevens
  and Stephen A. Rago, Addison-Wesley Professional, 2008.

### Electronic Course Materials

* Electronic course materials, including the schedule, may be accessed directly
  at the [course website]({{ site.baseurl }}).

* All grades and assignment feedback will be posted on [Gradescope]({{
  site.gradescope_url }}).

* Posting questions, discussions, and answers to questions asked are best done
  on the course’s [Moodle forum]({{ site.moodle_url }}).


## Deadlines and Late Policy

The deadlines of all homeworks, labs, and project milestones are listed on
the [schedule]({{ site.baseurl }}/schedule/) page.

In general, you should plan ahead and submit assignments on time. This course
has no ’late day’ policy and late assignments are generally worth 0 points
unless you have made arrangements. This class is fast-paced, if you started
delaying work, things will snowball very quickly, so plan ahead and **start
early**.

However, we understand that situations out of control may arise. We handle all
situations on a case-by-case basis.  Here are some general guidelines:

* **You should make arrangements with your instructor ahead of time**, minimum
  24 hours before the assigned deadline.
* The usual extension is 24 hours upon request or for extreme circumstances 48
  hours
* We keep track of extensions per student and if you seem to be overusing the
  privilege we will stop allowing extensions.
* **If the reason you are late is because you are having trouble completing the
  assignment (e.g. a bug you can’t fix or just difficulty understanding) get
  help from an instructor. Don’t assume that more time will fix the problem;
  plan to get help.**

## Special Arrangements

If circumstances have caused you to miss/fail several assignments OR if your
current situation in OS seems overwhelming, you should contact your instructor
right away.  Sometimes it is possible to make up missed work for partial credit,
these situations are always handled on a case-by-case basis.

The best chance of success is when these problems are addressed timely - but it
never hurts to have a discussion, however unsalvageable the situation may feel.
Please talk to your professor if you are having problems!

## Homework Assignments/Labs

Homework assignments, including Labs, are listed on the class schedule. Each
assignment includes instructions on obtaining the code and submitting it, please
follow those instructions carefully, especially when it comes to what files you
should be submitting; submitting object files and executables will cause all
sorts of issues when grading. The instructions on which files to submit are
there for a reason!

Assignments are split into two major types:

There are regular programming assignments, plus assorted quizzes and minor
assignments (you can see the exact weight of everything in Moodle).  These
assignments are integral to your understanding of OS - missing any of them can
make passing OS quite hard.  All these assignments are done individually.

There are also challenge assignments.  Challenge assignments generally involve
trickier programming but explore more interesting topics.  Despite the fact that
they are harder, the challenge assignments are actually weighted less than
regular assignments.  You can pass the class without doing all challenge
assignments, but your grade will be capped at a C, leaving you little room to
make mistakes on labs and exams.  BUT it's our hope everyone will attempt at
least some of the challenge assignments, and those looking for an A will need to
complete all of them.  Challenge assignments can be done in groups of 2, or
individually.

## Exams

There will be 2 skill exams and a final exam. Please make note of the following:

- You **must** earn a passing (60%) average on the exams to pass the course.
- You **must** earn at least a C (70%) average on the exams to earn a C or
  better in the course.


## Grading

| Item                   | Weight |
| :--------------------  | :----- |
| Assignments/Labs       | 40%    |
| Challenge  Assignments | 15%    |
| Project                | 10%    |
| Exams                  | 35%    |


I generally follow the registrar's interpretation of letter grades, which you
can find
[here](https://www.rose-hulman.edu/campus-life/student-services/registrar/rules-and-procedures/grades.html)
under the grades description section.

## Handling Grading Issues

Thankfully, Teaching Assistants will be helping us grade most of the homework in
this class. On the schedule page, you should be able to see who will be grading
a particular assignment. When you have a grading issue, please contact the
grading TA first, particularly for issues like:

+ The code you submitted works on your computer but not the TA's.
+ The TA didn't notice but you actually did do the thing requested, it was just
  in a different place etc.
+ You submitted the wrong files.

If you talk to the grading TA and you disagree with their analysis of your code
or you feel the grading standards were unfair (or unfairly applied) please
contact your instructor.  We want to help you but everything goes smoother if
logistical issues come to the TAs first.

## Collaboration, Cheating and Professional Behavior

The sections below provide specifics on academic misconduct and its
consequences. Please understand that these policies are rooted in the **fact**
that we are here to teach, and you are here to learn!

### Plagiarism and General Misconduct

If you **plagiarize any part of your work or take part in any form of academic
misconduct (including hindering the work of other students), you will
automatically receive a grade of F** for the class and the incident will be
reported for further disciplinary action.

### Self-plagiarism

Students who turn in work derived from a past attempt at the course shall
receive an F in the course.

Self-plagiarism is copying and reusing all or some of your previous work for
another course attempt. There are several issues with reusing your own work,
including:
  
    1. It probably wasn't of high quality (since you're here retaking the
       course).
    2. It gives you a time advantage over other students.
    3. You are not learning the material by resubmitting existing work.

While all of these are significant issues, the last one is our biggest concern.
If you are retaking the class, you should be redoing the work, as the practice
helps reinforce course concepts.

**Self-plagiarism will be treated as any other form of academic misconduct**.

### Generative Artificial Intelligence

**Work for this course must be your own**! This means that you wrote or designed
it on your own, without the use of generative AI platforms. **Work wholly or
partially created through generative AI will be treated as any other form of
academic misconduct**.

**What is considered generative AI**?

The term generative AI is not necessarily restricted to large language models
accessible through online platforms, such as ChatGPT or Bard. Plugins that use
generative AI to complete code fragments (mainly GitHub Copilot) also fall under
this policy and will be treated similarly. Using any plugin in your IDE that
interacts with these generative AI models will be considered as academic
misconduct.

On exams, you will be asked to refrain from using an IDE, and are restricted to
using a terminal-based editor on a remote server. It is therefore in your best
interest to practice using that platform on the lab assignments, in preparation
for the exam. In addition, using any resources not specifically allowed during
the exam, such as communication platforms (MS Teams, Slack, Discord), will be
considered academic misconduct. At the start of the exam, you will be asked to
quit all these platforms and turn off all notifications they might generate.
Failure to do so will be reported as a case of academic misconduct.

**Why no generative AI?**

Basically, because _we want you to learn how to do it_. Yes, generative AI will
be a part of our future, but it may not always be available to you, and if you
do have access, you need to understand if the work you receive from it is any
good. In the first case, generative AI platforms can store and repeat what
they're given. Imagine working with confidential company information or in
classified government work? You cannot feed it the information on what you want,
because that information will become part of its network. Beyond that, you
cannot simply assume that an AI did good work. Even if you end up in a job that
is AI-assisted, a human still needs to be able to evaluate if the work was done
properly or not, you won't be able to do that unless you have experience doing
the work yourself.

### Collaboration

We generally welcome and encourage discussions on homework and lab assignments.
Such discussions about ideas are not considered cheating, and can be very
beneficial for your learning experience. However, the exchange of code or
written answers is cheating!

In such discussions of ideas, you should distinguish between helping or hurting
yourself and the other student(s). In brief, you can help each other by teaching
yourselves, but you can hurt each other by giving each other answers, such as a
key idea needed to work through a problem. The same applies to tutoring and
getting help from the instructor.

If you use reference materials (other the course texts and resources) to solve a
problem, please cite the references material. Similarly, if you discuss a
solution with another student, _give credit where credit is due_ by making a
note such as "the following idea was developed jointly with Alyssia P. Hacker",
or "the following idea is due to Ben Bittwiddle". You cannot be charged with
plagiarism if you cite in this way. However, copying and reusing code, even with
such a citation, is still considered cheating. Cooperation refers to the
exchange of ideas, not code or written answers. **Collaboration** refers to
helping one another _understand course material_, not providing shortcuts to
completing course work.

### Summary

The instructors take plagiarism seriously. **Students caught plagiarizing, or
cheating in any way, will be reported to the dean of students and will receive
an automatic F in the class**.


<!--
With code assignments - it is acceptable to discuss your solution with other
students.  It is acceptable to get some occasional help with a particular
section or with debugging.  It is not acceptable, nor in your best interest,  to
pair-program the assignment together (except for challenge assignments where
pairing is allowed).  It is not acceptable to co-write the assignment by
discussing the solution line-by-line as you write it.  It is definitely not
acceptable to get code from someone else.

Students found to have cheated will be penalized up to -150% of the value of the
assignment (yes, that’s a negative grade, beyond a simple 0). This is in
addition to cheating being reported and added to your permanent file.

Students are expected to behave professionally. A lack of professional behavior
in a student’s interactions within a team, with an instructor, teaching
assistants or during in-class activities can result in a penalty grade.
Depending on the nature and the egregiousness of the behavior, the penalty grade
might be a reduction in letter grade or an F in the class.
-->

## A statement on mental health

We know that as students, you are dealing with a lot of stress, often a lack of
sleep, and sometimes social isolation. It is easy to neglect your mental (and
physical) health and lose yourself in the vortex of pressure. Spoken from
experience, it is important for you to *make time* for yourself, both mentally
and physically. Therefore, if at any point during the quarter, you feel that you
are in need of help, please reach out to the [office of health
services](https://www.rose-hulman.edu/campus-life/student-services/wellness-and-health-services/health-services/index.html)
or [the student counseling
center](https://www.rose-hulman.edu/campus-life/student-services/wellness-and-health-services/counseling-services/index.html).
Also, if you feel comfortable, you can reach out to me so I can help you devise
a plan to tackle your coursework for CSSE 332.

## Miscellaneous

### Syllabus changes

Please note that the syllabus is a living document. We reserve the right to
change the syllabus at any time, but you will notified of any changes.

### Attendance policy

It is in your best interest to attend all class sessions, as we will often be
doing coding activities that will feed directly into your homework or lab
assignments. 

Therefore, **up to two unexcused absences are allowed**. Any additional
unexcused absence may result in you receiving a failing grade for the course.
You will be responsible for making up any missed work during those unexcused
absences.

### Illness

We understand illnesses may arise and you will need to take some time to
recover, resulting in missing class sessions. In such case, please do inform
your instructor as soon as possible about your situation; the details of your
situation that you wish to share are totally up to you. To protect your
classmates and yourself, please do not come to class ill, especially if it is
COVID-related.

### Laptop policy

You may need to use your laptops during a portion of class periods. Please be
sure to bring your laptop and a power brick to class.

**During class discussion, please do not use your laptops**. Studies have found
that using your device during class time (non work time) hinders your learning
as well as that of your classmates. Class time is not the time to catch up on
anime or to make that big push on league of legends!

However, if you typically use your laptop for note taking, please talk with your
instructor so they can make an exception.

## Acknowledgments

This syllabus was last modified by Mohammad Noureddine on Fri 24 Nov 2023. It is
built on the contribution of various instructors, including Mike Hewner, Lixing
Song, and Mark Hays.

