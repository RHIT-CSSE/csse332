---
title: Semaphores Homework I
layout: post
date: Wed Apr 14
readtime: true
gh-repo: rhit-csse332/csse332-202130
gh-badge: [star,watch,follow]
---

This assignment has 4 problems. They go from easy to hard, so I recommend you do them in order. The instructions for each are in the comments of the code itself.

Some guidance on how we grade:

* You can get 25% of points if your semaphore-based solution reliably
  produces correct output with the given data (and small
  modifications).  0 points if doing this requires weird sleeps or
  other hacky non-general purpose code.
* You can get 50% points you did above and your solution is
  plausible and does not do something obviously bad.  It's impossible
  to enumerate every obviously bad thing but here's a big one : having
  multiple concurrent threads modify a global variable.  Basically
  this is for solutions that basically are OK but miss some edge
  cases.
* You can get 50% points if you have a fully working solution but
  you utilize busywaits or non-standard semaphore functions
  (i.e. stuff other than init, post, wait, and destroy)
* A fully correct solution, even if it's not the one we envisioned, is
  worth 100%


1. (10 points) threeAndTwo

2. (10 points) abcd

3. (20 points) producerConsumer

4. (20 points) littleRedHen


## Logistics
* [Source code]({{ site.gh_homeworks_url }}/08_semaphores_homework1)
* [Submission box](https://moodle.rose-hulman.edu/mod/assign/view.php?id=2708067)
* [Submission instructions](https://rhit-csse332.github.io/csse332-202130/docs/submission_instructions/)

