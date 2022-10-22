---
title: Syllabus
layout: post
readtime: true
date: Thursday Oct 20, 2022
---

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
CSSE-220. However, to make the best out of this class, we strongly recommend
that you have taken, or are familiar with the topics presented in CSSE 232
(Computer Architecture I). A lot of the topics we cover will make more sense if
you have the background knowledge discussed in an architecture class. 

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

## Instructor(s)

<!-- TODO: Update these each term. -->

- [Mohammad Noureddine](https://mnoureddine.com) 
  - Office: Moench F214.
  - Email: noureddi `at` rose-hulman `dot` edu
  - [Get on my calendar](https://calendly.com/mnoureddine/office-hours)

### Mohammad's Welcome Statement

I am very excited to have the opportunity to offer this class and can’t wait to
meet you and get to know you. I hope that throughout the class, you will see
me as part of your support team. I strive to offer you a welcoming environment
where you can get help on course content, homework, or anything else really.
Studies conducted by me in my office show that coming to office hours
increases your chances of success.

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

<!-- TODO: update link to the quarter's gradescope -->
* All grades and assignment feedback will be posted on [Gradescope]({{
  site.gradescope_url }}).

<!-- TODO: update link to the quarter's campuswire -->
* Posting questions, discussions, and answers to questions asked are best done
  on the course’s [CampusWire]({{ site.campuswire_url }}).


## Deadlines and Late Policy

The deadlines of all homeworks, labs, and project milestones are listed on on
the [schedule]({{ site.baseurl }}/schedule/) page.

In general, you should plan ahead and submit assignments on time. This course
has no ’late day’ policy and late assignments are generally worth 0 points
unless you have made arrangements.

However we understand that situations can arise. We handle all situations on a
case-by-case basis.  Here are some general guidelines:

* The usual extension is 24 hours upon request or for extreme circumstances 48
  hours
* We expect to be notified as soon as is feasible. If you are very ill, we
  understand that that might be slightly after the assignment due date. For
  other situations, it’s usually possible to notify us in advance.
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

Homework assignments, including Labs, are listed on the class schedule. 
Homework assignments that require code solutions and submission instructions can
be found [here]({{ site.baseurl }}/docs/submission_instructions/).

<!-- TODO: FIX ME -->
There are "regular" programming assignments, plus assorted quizzes and
minor assignments (you can see the exact weight of everything in Moodle).  These
assignments are integral to your understanding of OS - missing any of them can
make passing OS quite hard.  All these assignments are done individually.

<!-- TODO: FIX ME -->
There are also ADVANCED assignments.  Advanced assignments generally involve
trickier programming but explore more interesting topics.  Despite the fact that
they are harder, the advanced assignments are actually weighted less than
regular assignments.  If you're willing to accept a C in the course, you don't
have to do any advanced assignments.  BUT it's our hope everyone will attempt at
least some of the advanced assignments, and those looking for an A will need to
complete nearly all of them.  Advanced assignments can be done in groups of 2,
or individually.

## Exams

There will be 2 skill exams.  Passing *each* exam is required to pass the course
(unless otherwise specified, passing means getting 65% or better).  Each exam
will have a makeup and the higher of the two exam grades will be used.  However,
if you fail both the exam and the makeup, you fail the course.

## Grading

| Item                  | Weight |
| :-------------------- | :----- |
| Assignments/Labs      | 40%    |
| ADVANCED  Assignments | 30%    |
| Exams                 | 30%    |


I generally follow the registrar's interpretation of letter grades, which you
can find
[here](https://www.rose-hulman.edu/campus-life/student-services/registrar/rules-and-procedures/grades.html)
under the grades description section.

* NOTE: You must pass each skill exam (usually 65% or better) to pass the course 

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

We generally welcome and encourage collaboration on homework and laboratory
assignments. However, it is in your best interest if you write your own code and
appropriately acknowledge any sources of help you receive to complete your
assignment. This will no doubt help you perform better on exams and design
projects. 

With code assignments - it is acceptable to discuss your solution with other
students.  It is acceptable to get some occasional help with a particular
section or with debugging.  It is not acceptable, nor in your best interest,  to
pair-program the assignment together (except for advanced assignments where
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

## A statement on mental health
I know that as students, you are dealing with a lot of stress, often a lack of
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

