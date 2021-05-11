---
layout: post
title: Session 34: Paging Issues
readtime: true
date: Mon May 10 19:55:33 2021
gh-repo: rhit-csse332/csse332-202130
gh-badge: [star,watch,follow]
---

# Introduction and aim

In this lecture, we will further explore the issues associated with implementing
virtual memory using fixed-sized pages, i.e., paging. We will look at the two
problems that we have identified in the previous lecture:
1. Each process needs a 4MB page table.
1. Each memory access now requires two accesses to memory, one for the page
   table and another to get the actual data from memory.

We will propose the Translation Lookaside Buffer (TLB) as a solution to the
second problem. We will also take a look at multi-level paging as a possible
solution to the first problem. We will use these two approaches as examples to
highlight the tradeoffs between memory and efficiency.

## Logistics

- [Class notebook section](https://rosehulman-my.sharepoint.com/personal/noureddi_rose-hulman_edu/_layouts/OneNote.aspx?id=%2Fpersonal%2Fnoureddi_rose-hulman_edu%2FDocuments%2FClass%20Notebooks%2FCSSE%20332%20Operating%20Systems&wd=target%28_Content%20Library%2FSession%2034%20Paging%20Issues.one%7C274BDF45-92B2-2148-A809-30C498D55ED8%2F%29)
- [Activity submission box](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2708181)
- [Clear/muddy survey](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2817945)

# Video recordings

Coming soon...
