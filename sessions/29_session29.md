---
layout: post
title: Session 29 Deadlock Detection
readtime: true
date: Tue May  4 08:35:24 2021
gh-repo: rhit-csse332/csse332
gh-badge: [star,watch,follow]
---

# Introduction and aim

In this session, we will take a look at the ways in which an operation system
can handle the problem of deadlocks occurring in a user application. 
There are three main techniques that we will discuss in this lecture:
1. The ostrich algorithm: basically do nothing about deadlocks.
1. Deadlock avoidance: using the Banker's algorithm
1. Deadlock detection and recovery

## Logistics

- [Slides](https://rosehulman-my.sharepoint.com/:p:/g/personal/noureddi_rose-hulman_edu/ERizt1RQuf5Ou6HKR0MumtMBCK5HWjiaQPbGgx5kBiEHQg?e=YDmJXa)
- [Class notebook section](https://rosehulman-my.sharepoint.com/personal/noureddi_rose-hulman_edu/_layouts/OneNote.aspx?id=%2Fpersonal%2Fnoureddi_rose-hulman_edu%2FDocuments%2FClass%20Notebooks%2FCSSE%20332%20Operating%20Systems&wd=target%28_Content%20Library%2FSession%2029.one%7C7C7F817C-2A1A-43A3-B430-D3EC306516CF%2F%29)
- [Activity source code]({{ site.gh_activity_url }}/29_deadlocks)
- [Activity submission box](https://moodle.rose-hulman.edu/mod/assign/view.php?id=2809848)
- [Clear/muddy survey](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2809842)

# Video recordings

<iframe width="560" height="315" src="https://www.youtube.com/embed/REuQsBhjNxo" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

<iframe width="560" height="315" src="https://www.youtube.com/embed/2qK-AQJXqIk" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
