---
layout: post
title: Session 7 Makefiles
readtime: true
date: Fri Mar 19 21:04:44 2021
gh-repo: rhit-csse332/csse332-202130
gh-badge: [star,watch,follow]
---

# Introduction and aim
In this session, we will learn how to write our own makefiles to help in speeding up the process of
compiling our source code and generating our executables. 

## Logistics
* [Slides](https://rosehulman-my.sharepoint.com/:p:/g/personal/noureddi_rose-hulman_edu/ETSbGLHm3wNAp50Lr9mEeVEBrjSLvc9sd6-m6xCoVUdHOA?e=d6ASrl)
* [Class notebook section](https://rosehulman-my.sharepoint.com/personal/noureddi_rose-hulman_edu/_layouts/OneNote.aspx?id=%2Fpersonal%2Fnoureddi_rose-hulman_edu%2FDocuments%2FClass%20Notebooks%2FCSSE%20332%20Operating%20Systems&wd=target%28_Content%20Library%2FSession%207.one%7CE207A7DB-84D0-44CF-BDAD-A2142AC49AC8%2F%29 onenote:https://rosehulman-my.sharepoint.com/personal/noureddi_rose-hulman_edu/Documents/Class%20Notebooks/CSSE%20332%20Operating%20Systems/_Content%20Library/Session%207.one#section-id={E207A7DB-84D0-44CF-BDAD-A2142AC49AC8}&end)
* [Source code]({{ site.gh_repository_url }}/tree/main/activities/07_makefiles)
* [Activity submission box](https://moodle.rose-hulman.edu/mod/assign/view.php?id=2708136)
* [Clear/Muddy questions](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2767419)

### Outline
* [Motivation](#motivation) ~ 5 minutes
* [The basics](#the-basics) ~ 9 minutes
* [Multiple targets](#multiple-targets) ~ 11 minutes
* [From code to executable](#from-code-to-executable) ~ 10 minutes
* [Optimizing our targets](#optimizing-our-targets) ~ 9 minutes
* [Optional: Makefile tips and tricks](#makefile-tips-and-tricks) ~ 15 minutes
* [Conclusion](#conclusion)

# Motivation
As usual, we start off with our lame motivating example. We look at the process of making a pizza
and see how we can define our recipe through its different sub-components.

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=fd2ee2a6-f2d9-49f8-9379-acef015e75ed&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# The basics
Inspired by the requirements for our pizza making recipe, we take a basic look at makefiles. We
explore how we can define targets, dependencies, and rules. 

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=edfdeb8b-c1cf-4c69-9bed-acf00155aa4b&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# Multiple targets
Next, we extend our makefile to support multiple targets and cleaning up our kitchen after we finish
up making our pizza!

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=65e1df4e-c9f5-45bb-9e0a-acf0015c1b18&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

As usual, I forgot to clean everything up. So here is a quick addendum on cleaning up.

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=fb4b8b06-433b-4477-9836-acf0016042fd&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>


# From code to executable
In order to understand how we can further optimize our build environment, we need to first
understand the process of how we get from the source code to the executable. We introduce the
concepts of (1) pre-processing, (2) compilation, (3) assembling, (4) linking.  

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=904b1f82-8fc2-4e41-b348-acf00161d372&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# Optimizing our targets
Finally, to put things into context and further optimize our build, we make use of the different
stages of compilation and linking to only recompile any files we need. 

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=bcc3b01f-970b-4789-84ae-acf0016534a4&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# Makefile tips and tricks

This part of the session is optional but highly recommended. In this video, we learn a few tips and
tricks and help save on typing time by making use of several useful features of `make`. 

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=6e0de392-3b31-4d1c-ab22-acf0017299d1&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# Conclusion
In this session, we learned the basics of writing makefiles to speed up and optimize our build
process. We explored the stages of compilation to general separate object files in a way that allows
us to only recompile the files that are strictly necessary to regenerate a binary. As usual, please
don't forget to submit your activity
[here](https://moodle.rose-hulman.edu/mod/assign/view.php?id=2708136) and you clear/muddy survey
[here](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2767419).
