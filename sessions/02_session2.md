---
layout: post
title: Session 2 Intro to OS
readtime: true
date: Sat Mar 13 22:36:14 2021
gh-repo: rhit-csse332/csse332-202130
gh-badge: [star,watch,follow]
---

# Introduction and aim
In this session, we will have our first basic introduction to what an operating system is, and what
its main roles are. **Note** that this section does not have an activity that you must submit, but
there is a clear/muddy survey at the end of the session, as usual. 

## Logistics
* [Slides](https://rosehulman-my.sharepoint.com/:p:/g/personal/noureddi_rose-hulman_edu/Ed5xz_-IYohJkY181epr6f4BGXWhlPajqlUpqVpbMKwBvg?e=oPqhZr)
* [Class Notebook Section](https://rosehulman-my.sharepoint.com/personal/noureddi_rose-hulman_edu/_layouts/OneNote.aspx?id=%2Fpersonal%2Fnoureddi_rose-hulman_edu%2FDocuments%2FClass%20Notebooks%2FCSSE%20332%20Operating%20Systems&wd=target%28_Content%20Library%2FSession%202.one%7C6A2FB6B2-2514-4B0E-A5D7-DD9D110A158D%2F%29 onenote:https://rosehulman-my.sharepoint.com/personal/noureddi_rose-hulman_edu/Documents/Class%20Notebooks/CSSE%20332%20Operating%20Systems/_Content%20Library/Session%202.one#section-id={6A2FB6B2-2514-4B0E-A5D7-DD9D110A158D}&end)
* [Demo Resources Directory]({{ site.gh_repository_url }}/tree/main/activities/02_intro_demo/)
* [Clear/Muddy Questions](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2758953)
* __Due date__: Monday, Mar 15, 2021, 23:55

### Outline
* [Motivation](#motivation) ~ 8 minutes
* [The Roles of an OS](#the-roles-of-an-operating-system) ~ 7 minutes
  * [Example](#example) ~ 7 minutes
* [Demo: OS as Glue](#demo-os-as-glue) ~ 5 minutes
* [Demo: OS as Referee](#demo-os-as-referee) ~ 5 minutes
* [Demo: OS as Illusionist](#demo-os-as-illusionist) ~ 9 minutes
* [Conclusion](#conclusion)

# Motivation
We kick off our session with the story of Heidi, a successful female of the Pac-people species, who
finds herself in a sticky situation. Heidi must take care of three kids while having only a limited
number of resources. Find out how she attempts to face this challenge.

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=345bdbf3-cb86-4855-89a4-acea0183cff5&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# The Roles of an Operating System
Based on Heidi's story, in this video, we introduce an _operating system_ as a piece of software
that provides everything you **need** to operate a computer. In our definition, the operating system
performs three basic roles:
1. Referee
2. Illusionist
3. Glue

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=85ee6753-b756-4c87-906b-acea0186a4f7&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

## Example
Next, we apply the above concepts to the Chrome web browser application and reason about how the OS
performs the roles of referee, illusionist, and glue for that application. 

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=b0b09e66-7085-48d2-a9bf-aceb00002472&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

Question to check your understanding:
> Pick any application you like, think about the tasks that the application performs, and then think
about how the OS acts as a referee, illusionit, and glue for your application. It can as simple as a
text editor and as complicated as a video game. 

# Demo: OS as Glue
In this demo, we see what is means for the OS to act as a glue in action. We examine a simple code
that writes one line into a file and examine all the various system calls that the operating system
performs to make that happen!

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=42efc261-bfa6-40dc-b392-aceb00028c17&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

For better understanding, do the following:
> Modify the code in `glue.c` to perform any action you would like. Then compile the program and use
the `strace` program to inspect what the operating system performs in order to service your
program's request.

# Demo: OS as Referee
In this demo, we examine what happens when two or more processes (or application) get into
contention about the same resource. We see how the OS acts an arbiter and how that is not always in
sync with what we would expect. 

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=8c42841f-c700-47ec-9ca2-aceb00046a27&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

For better understanding, do the following:
> Modify the code in `referee.c` to do more iterations and/or run it more than twice concurrently.
What do expect would happen then? Does the output you see fit your expectations? Is it possible to
predict how things will go?


# Demo: OS as Illusionist
In this demo, we see how the OS acts as an illusionist. We run two processes that on the surface of
it, appear to be reading and writing to the same memory location. However, we see from the output
that each process is actually reading and writing to its own memory space. How does that happen? The
OS is putting each process under the illusion that it owns the entire memory space!

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=fe517b26-6412-403a-b795-aceb00061c52&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

**Note**: If you would like to run this code yourself, you will need to turn off some security
protections in gcc. I provide the instructions to do so in the `README.md` file in the activity demo
directory.

For better understanding, do the following:
> Modify the code in `illusionit.c` to allocate more and more memory and see what happens when you
do that. For example, allocate more than one integer and examine their addresses, do they seem to be
next to each other in our illusion?

# Conclusion
There is no activity to submit for this application. However, there is a short clear/muddy quiz to
help me answer any questions you might have. You can find the quiz [here](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2758953).
