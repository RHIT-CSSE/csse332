---
layout: post
title: Session 35 Page Replacement Policies
readtime: true
date: Thu May 13 09:49:49 2021
gh-repo: rhit-csse332/csse332-202130
gh-badge: [star,watch,follow]
---

# Introduction and aim

In this final session on memory management, we will discuss the hole that the
operating system has dug itself into. By giving each process the illusion that
it owns the entirety of the address space, the OS could get into a situation
where there is more demand on memory that it can actually serve, i.e., the
processes are requesting more pages (and therefore page frames) than there
actually are in physical memory. So what do we do? How do you get out of a lie?
Well, you lie some more.

To get out of this pickle, the OS will write current pages from memory to disk
and then replace those with new pages that are currently needed. But which pages
is it going to replace? That's what we're going to answer in today's lecture.



## Logistics

- [Class notebook section](https://rosehulman-my.sharepoint.com/personal/noureddi_rose-hulman_edu/_layouts/OneNote.aspx?id=%2Fpersonal%2Fnoureddi_rose-hulman_edu%2FDocuments%2FClass%20Notebooks%2FCSSE%20332%20Operating%20Systems&wd=target%28_Content%20Library%2FSession%2035%20Page%20Replacement%20Policies.one%7CC8750C19-3AA2-BF4F-A142-AF8D501C5644%2F%29)
- [Activity excel sheet]( {{ site.base_url }}/activities/35_activity.xlsx)
- [Activity submission box](https://moodle.rose-hulman.edu/mod/assign/view.php?id=2708178)
- [Clear/muddy survey](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2819250)

# Video recordings

Coming soon...

