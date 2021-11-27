---
layout: post
title: Session 9 FileSystems I
readtime: true
date: Wed Mar 24 12:20:39 2021
gh-repo: rhit-csse332/csse332
gh-badge: [star,watch,follow]
---

# Introduction and aim
In this session, we are again visited by Professor Lixing Song from Sections 1/2 this quarter. We
will now start to take a look at how the OS plays the role of a glue by looking at filesystems and
how are files and data are organized on hard drives.

## Logistics
* [Slides](https://rosehulman-my.sharepoint.com/:p:/g/personal/noureddi_rose-hulman_edu/Ee0ubGqNnf1MhINAFuo-YJMBSGKh3JwRfp5zBWUkaUD-Kw?e=k7MWcx)
* [Activity Files]({{ site.gh_repository_url }}/tree/main/activities/09_filesystems_i)
* [Activity submission](https://moodle.rose-hulman.edu/mod/assign/view.php?id=2708139&forceview=1)
* [Clear/Muddy questions](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2769156&forceview=1)

### Outline
* [Introduction](#introduction) ~ 21 minutes
* [FAT filesystems](#fat-filesystems) ~ 12 minutes
* [Inode-based systems](#inode-based-systems) ~ 22 minutes

# Introduction
In this video, we start off by discussing the needs for filesystems and their objectives. We also
present some criteria for comparing different filesystem implementation. The criteria are:
1. How easy it is to create new files when space is available?
1. Are there any limits on the size of files we create?
1. How fast can we get data from a specific slot in a file?
1. How much space are wasting in storing metadata (i.e., bookkeeping information)?

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=d425c96e-a50a-4026-bd49-ac83012ad0cd&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

---

# FAT filesystems
In this video, we dive deeper into FAT (File Allocation Table) filesystems, the early filesystem
used for MS-DOS and what you typically see when you plug in a USB device. We then answer the above
questions for the FAT filesystems and see how they fare out.

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=3a98da92-f177-44ca-a1b1-ac830135c4f3&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

---

# Inode-based systems
In this video, we take a deeper dive into inode-based filesystems (pointer-based filesystem), the
default for modern Unix-based operating systems. We also see how inode-based filesystems fare out
with respect to our comparison criteria.

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=8c5c20a0-a60f-4c01-81d5-ac8301687222&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

---

# Conclusion
In this session, we introduced filesystems and the need for them in modern operating systems.
Filesystems are a great example of how an OS acts as  a glue between users and the hardware they are
using. We discuss two approaches to managing files and data: FAT and inode-based systems, and then
compare the two based on a set of criteria that we defined in our introduction.

We have an activity due for this session, please make sure you submit the activity
[here](https://moodle.rose-hulman.edu/mod/assign/view.php?id=2708139&forceview=1). And as usual,
please submit your clear/muddy survey
[here](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2769156&forceview=1).
