---
layout: post
title: Session 25 Deadlocks
readtime: true
date: Monday April 26
gh-repo: rhit-csse332/csse332-202130
gh-badge: [star,watch,follow]
---

# Introduction and aim

In this session, we will dive deeper into concurrency bugs and look at the most
prominent form of such bugs, the deadlock. We will see that a deadlock can occur
when the following four conditions are met:
1. Mutual exclusion
1. Hold-and-wait
1. No preemption
1. Circular wait

We will define each of these conditions and then look at how we can prevent
deadlocks by tackling each of those four conditions. 

## Logistics

- [Slides](https://rosehulman-my.sharepoint.com/:p:/g/personal/noureddi_rose-hulman_edu/EV3_fhFJS99Pvwcmq2qIupMBNxA3R5qaVwaxm_QWCIkp_A?e=8DDFQZ)
- [Class notebook
section](https://rosehulman-my.sharepoint.com/personal/noureddi_rose-hulman_edu/_layouts/OneNote.aspx?id=%2Fpersonal%2Fnoureddi_rose-hulman_edu%2FDocuments%2FClass%20Notebooks%2FCSSE%20332%20Operating%20Systems&wd=target%28_Content%20Library%2FSession%2025%20Deadlocks.one%7C3271C056-18EA-6140-A10B-34CA1BE57524%2F%29)
- [Activity source code]({{ site.gh_activity_url }}/25_deadlock_starvation)
- [Activity submission box](https://moodle.rose-hulman.edu/mod/assign/view.php?id=2708169)
- [Clear/muddy survery](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2799426)

# Video recording

<iframe width="560" height="315" src="https://www.youtube.com/embed/F0oTCS3c2Bs" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
