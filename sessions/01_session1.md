---
layout: post
title: Session 1 Pointers Review
readtime: true
date: Wed Mar 10 21:15:18 2021
gh-repo: rhit-csse332/csse332
gh-badge: [star,watch,follow]
---

# Introduction and aim
In this session, we will do a quick review of some of the basic concepts in C, specifically
pointers, void pointers, memory management, and function pointers. 

### Logistics
* [Slides](https://rosehulman-my.sharepoint.com/:p:/g/personal/noureddi_rose-hulman_edu/EcmqJLbNxRNPuUiRgvmIEBEBVUyjWk4wvoDA8wdlhZw-xg?e=Xx2ofO)
* [Class Notebook
Section](https://rosehulman-my.sharepoint.com/personal/noureddi_rose-hulman_edu/_layouts/OneNote.aspx?id=%2Fpersonal%2Fnoureddi_rose-hulman_edu%2FDocuments%2FClass%20Notebooks%2FCSSE%20332%20Operating%20Systems&wd=target%28_Content%20Library%2FSession%201.one%7CB6A37146-6E41-4B80-8E9A-980A4685F9E5%2F%29 onenote:https://rosehulman-my.sharepoint.com/personal/noureddi_rose-hulman_edu/Documents/Class%20Notebooks/CSSE%20332%20Operating%20Systems/_Content%20Library/Session%201.one#section-id={B6A37146-6E41-4B80-8E9A-980A4685F9E5}&end)
* [Activity source directory](https://github.com/rhit-csse332/csse332/tree/main/activities/01_pointers)
* [Activity submission box](https://moodle.rose-hulman.edu/mod/assign/view.php?id=2708124)
* [Clear/Muddy Questions](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2752890)
* [Submission instructions]({{ site.baseurl }}/docs/submission_instructions/)
* __Due date__: Thursday, Mar 11, 2021, 23:55


### Outline
* [Introduction and announcements](#announcements) ~ 2:00 minutes
* [Pointers review and void pointers](#pointers-review) ~ 12:00 minutes + 5:00 minutes demo
* [Pointer arithmetic](#pointer-arithmetic) ~ 12:00 minutes + 5:00 minutes demo
* [Memory management](#memory-management) ~ 14:00 minutes
* [Function pointers](#function-pointers) ~ 12:00 minutes
* [Conclusion](#conclusion)

# Announcements
In this first short video, we will make a few announcements and set out our aim for this session.

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=4048b3b0-93f3-4b9a-930e-ace7014a55e5&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# Pointers review
In this video, we will go over C pointers and review some of their basic concepts. We will also talk
about void pointers, which are C pointers that can point to _anything_.

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=5f0a732f-dc33-4a9b-8188-ace70155a1d8&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

Below is a short demo that shows the above concepts in action. This demo is optional if you are
comfortable with your pointers.

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=17cdbb7a-a3be-40c6-a6c0-ace7016af3d8&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# Pointer arithmetic
In this video, we talk about pointer arithmetic and dive into the details of what happens when we
increment pointers, both typed pointers and void pointers.

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=13582d53-b225-47a6-ae8a-ace80005460d&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

There is also a short demo to accompany the pointer arithmetic concepts. Feel free to skip it if you
are comfortable with pointer arithmetic.

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=071c51fd-3971-44fb-ab0b-ace8000926c3&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

---

> Stop here and complete Step 1 of your in-class activity

---

# Memory management
In this video, we talk about `malloc` and `free`, the main ways to allocate and deallocate _dynamic_
memory in C.

_This video include a bonus voice appearance by Herman the cat who was not particularly happy with
the fact that I locked him out of the room. I guess he wanted to learn about memory management in C
as well_.

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=9a66f0c6-92b3-4ce2-8abe-ace8000d793e&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

---

> Stop here and complete Step 2 of your in-class activity

---

# Function pointers
Finally, we revisit the concept of function pointers that allow us to dynamically change which
function is called based on which function our pointer addresses. 

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=24d25ef7-8bf3-4655-9776-ace8001b3c02&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>


---

> Stop here and complete Step 3 of your in-class activity

---

# Conclusion
Once you have complete the activity for this session, please go ahead and submit your code to the
Moodle drop box [here](https://moodle.rose-hulman.edu/mod/assign/view.php?id=2708124). There is also
a short quiz for you to list your clear/muddy questions, you can submit that quiz
[here](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2752890).
