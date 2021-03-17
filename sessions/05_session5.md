---
layout: post
title: Session 5 Signals
readtime: true
date: Wed Mar 17 02:27:54 2021
gh-repo: rhit-csse332/csse332-202130
gh-badge: [star,watch,follow]
---

# Introduction and aim
In this session, we introduce the concept of a signal, a light-weight form of _Inter-Process
Communication_ (IPC). We will define signal and examine their life-cycle, from generation to
delivery. We then discuss how processes can define custom handlers to handle receiving certain
signals. Finally, we examine when and how we would like to block and then unblock signals.

## Logistics
* [Slides](https://rosehulman-my.sharepoint.com/:p:/g/personal/noureddi_rose-hulman_edu/EXbapWVhr95AuCwpFZbBss0Bvk0vLyJXY6O18azbxDsI1A?e=zDThBw)
* [Class Notebook Section](https://rosehulman-my.sharepoint.com/personal/noureddi_rose-hulman_edu/_layouts/OneNote.aspx?id=%2Fpersonal%2Fnoureddi_rose-hulman_edu%2FDocuments%2FClass%20Notebooks%2FCSSE%20332%20Operating%20Systems&wd=target%28_Content%20Library%2FSession%205.one%7C26AC2F19-86F8-48D4-B644-B857A409D5D5%2F%29 onenote:https://rosehulman-my.sharepoint.com/personal/noureddi_rose-hulman_edu/Documents/Class%20Notebooks/CSSE%20332%20Operating%20Systems/_Content%20Library/Session%205.one#section-id={26AC2F19-86F8-48D4-B644-B857A409D5D5}&end)
* [Source code]({{ site.gh_repository_url }}/tree/main/activities/05_signals)
* [Activity submission box](https://moodle.rose-hulman.edu/mod/assign/view.php?id=2708130)
* [Clear/Muddy Questions](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2762511)

### Outline
* [Motivation and definition](#motivation-and-definition)
* [Life of a signal](#life-of-a-signal)
* [Signal Handlers](#signal-handlers)
* [Signal Masking](#signal-masking)
* [Conclusion](#conclusion)

# Motivation and definition
In this video, we take a look at a quick motivating example on when we would like to use signals and
then go into defining what a signal is. 

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=d9abd2e4-8d7b-435d-b45e-acee00448380&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# Life of a signal
In this video, we examine the life of a signal, from the moment it is generated to the moment it is
delivered. We see that three things can happen when a process receives a signal:
1. The process ignores the signal
2. The process is interrupted and the signal's default action is executed
3. The process is interrupted and a custom signal handler is executed

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=0d85b039-10fa-4364-84dc-acee004dfeec&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# Signal handlers
In this video, we take a look at how we can define custom signal handlers that will run upon
receiving a signal. We see how to use the `signal` function call to register a handler with the OS
to receive our signals. Finally, we examine the first part of our in-class activity.

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=53168e81-633e-45cd-9e1d-acee0051dd27&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# Signal masking
In this video, we note that some functions are not "signal-friendly", i.e., in other words, bad
things would happen if they are interrupted (e.g., `printf`). To protect such functions from a such
jumps cause by signals, we can block certain signals from occurring and then unblock them later on.
We discuss the difference between blocking and ignoring signals and then move on to discussing part
(2) of our in-class activity.

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=4e8a3b48-9edd-4bdd-87e0-acee00561900&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# Conclusion and final thoughts
Finally, we discuss some final caveats and thoughts to be aware off and summarize what we have
learned in this session. Don't forget to submit your activity
[here](https://moodle.rose-hulman.edu/mod/assign/view.php?id=2708130) and fill in your clear/muddy
questions [here](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2762511)).

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=05f8d316-295c-4fe9-90f1-acee005a81d2&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>
