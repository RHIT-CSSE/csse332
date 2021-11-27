---
layout: post
title: Session 10 FileSystems II
readtime: true
date: Wed Mar 24 12:20:39 2021
gh-repo: rhit-csse332/csse332
gh-badge: [star,watch,follow]
---

# Introduction and aim

## Logistics
* [Slides](https://rosehulman-my.sharepoint.com/:p:/g/personal/noureddi_rose-hulman_edu/ESEc1aGYvTpKglJwcf8PVU8Bb92Y7nadyaXnwbIuRI1vYQ?e=cLCosC)
* [Activity Files]({{ site.gh_repository_url }}/tree/main/activities/10_filesystems_ii)
* [Activity submission](https://moodle.rose-hulman.edu/mod/assign/view.php?id=2708142)
* [Clear/Muddy questions](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2769153)

## Outline
* [Inodes and directories](#inodes-and-directories) ~ 26 minutes
* [Implementation of a file system](#implementation-of-a-filesystem) ~ 11 minutes
* [Activity discussion](#activity-discussion) ~ 13 minutes

---

# Inodes and directories
In this video, we will review the basics of filesystems and further discuss inodes and directories
in a inode-based filesystems.

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=9be294c3-519b-4270-bf9b-ac84017b3db6&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

---

# Implementation of a filesystem
In this video, we complete the picture and go on discussing the details of how we can implement
indexing and the metada we need preserve in each data block so we can easily create/retrieve/remove
files and directories in our systems.

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=cdcdfb1f-fe2a-4415-8b33-ac84018855b2&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

---

# Activity discussion
Finally, we concretize our understanding by going through parts of our in-class activity.

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=6a88a0c9-2529-4f76-be95-ac8a012bc298&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

---

# Conclusion
In this session, we took a deeper dive into the details and implementation of index-based (or
inode-based) filesystems. We see how we organize things into multiple levels of indirection so we
can store larger and larger files. We also discuss how we can provide support for creating new files
into our system. We finally go walk parts of our activity together so we can further solidify our
understanding of filesystems.

We have an activity due for this session, please make sure you submit the activity
[here](https://moodle.rose-hulman.edu/mod/assign/view.php?id=2708142). And as usual, please submit
your clear/muddy survey [here](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2769153).
