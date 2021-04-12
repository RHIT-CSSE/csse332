---
layout: post
title: Session 19 More Concurrency
readtime: true
date: Mon Apr 12 09:50:22 2021
gh-repo: rhit-csse332/csse332-202130
gh-badge: [star,watch,follow]
---

# Introduction and aim
In this session, we will explore two more concurrency constructs that are at the heart
of implementing semaphores: (1) mutex locks and (2) condition variables. We will look
at their usage and API as well as briefly talk about how to implement them in practice.

## Logistics
* [Slides](https://rosehulman-my.sharepoint.com/:p:/g/personal/noureddi_rose-hulman_edu/ESaiuG2DRclCu8Jxx9fLtmMBYV_rbAZUM_JyXnXmZwDmZg?e=2acTW1)
* [Class notebook section](https://rosehulman-my.sharepoint.com/personal/noureddi_rose-hulman_edu/_layouts/OneNote.aspx?id=%2Fpersonal%2Fnoureddi_rose-hulman_edu%2FDocuments%2FClass%20Notebooks%2FCSSE%20332%20Operating%20Systems&wd=target%28_Content%20Library%2FSession%2019%20Condition%20Variables.one%7C4E5A3D97-C6A3-9148-839E-31BE4823AFE9%2F%29 onenote:https://rosehulman-my.sharepoint.com/personal/noureddi_rose-hulman_edu/Documents/Class%20Notebooks/CSSE%20332%20Operating%20Systems/_Content%20Library/Session%2019%20Condition%20Variables.one#section-id={4E5A3D97-C6A3-9148-839E-31BE4823AFE9}&end)
* [Source code]({{ site.gh_activity_url }}/19_condition_variables)
* [Activity submission](https://moodle.rose-hulman.edu/mod/assign/view.php?id=2708157)
* [Clear/Muddy survey](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2788095)

In-class activity and clear/muddy survey will be due on Tuesday at 2:00 pm ET.

# Outline

|           Topic             |           Time              |
| :-------------------------- | --------------------------- |
| Announcements and recap     |  5 minutes |
| Mutex:binary semaphore      |  5 minutes |
| Condition variables + small demo | 15 minutes |
| Producer/consumer activity | 25 minutes |

# Video recording

<iframe width="560" height="315" src="https://www.youtube.com/embed/J0tdNA0PCiM" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
