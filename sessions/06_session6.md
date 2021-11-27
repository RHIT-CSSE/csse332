---
layout: post
title: Session 6 Fork & Exec
readtime: true
date: Wed Mar 17 18:36:42 2021
gh-repo: rhit-csse332/csse332
gh-badge: [star,watch,follow]
---

# Introduction and aim
In this session, we take a look at the process of creating new processes. What happens behind the
scenes when we launch an executable (either from the terminal or the GUI)? What are the ways to
launch new processes? We explore two function calls: the `exec` family that replaces a process with
a new one, and the `fork` system call which creates a child process. Today's session features
Professor Lixing Song from our sister sections in the class. 

## Logistics
* [Slides](https://rosehulman-my.sharepoint.com/:p:/g/personal/noureddi_rose-hulman_edu/EcCd3G_NmWNFhYw2-KQ4OYoBezGgHu7X2FIutTAnMrTTlw?e=LiFxf4)
* [Source Code]({{ site.gh_repository_url }}/tree/main/activities/06_fork_exec)
* [Activity submission box](https://moodle.rose-hulman.edu/mod/assign/view.php?id=2708133)
* [Clear/Muddy questions](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2762514)

### Outline
* [The exec family of calls](#the-exec-family-of-system-calls)
* [Using fork to create children](#using-fork-to-create-children)
* [In-class activity](#in-class-activity)

# The exec family of system calls
We will first learn about the first way to launch a new process, the `exec` family of system calls.
`exec` system calls request that the operating system replaces the current process by a new process
whose binary is specified in its first parameter. We discuss several version of `exec` and the
differences between them. 

Recall that the `exec` calls do not return unless they fail. I.e., if a call to `exec` is
successful, the current process is replaced by a new one, and thus any code that is after the `exec`
call will be unreachable. 

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=c0db77d1-fc55-4c3b-a118-ac7d0133deff&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# Using fork to create children
In this video, we introduce the `fork` approach to creating new processes. `fork` creates a new
child process that is a duplicate of its parent. Unlike `exec`, `fork` can be thought of as
returning twice: once in the parent process (with a return value equal to the child's PID) and
another in the child process (with a return value of 0). Thus, depending on the return value from
`fork`, you can tell in which process you are executing, and thus run different code. Here is an
example
```C
int pid;

pid = fork();
if (pid == -1) {
  // error, exit with failure
  exit(EXIT_FAILURE);
}

/* both parent and child return here. this means that they will both
 * print the following line:
 */
printf("Hello world!\n");

if (pid == 0) {
  // pid 0 means this is the child!
  do_child_stuff();
} else {
  // this is the parent portion
  do_parent_stuff();
}
```

We then introduce the `wait` system call that a parent process can use to wait for its children to
complete execution and read their exit status. Given that there error can occur, two things might
happen and children process can end up either as orphans or as zombies.
* __Orphan process__: A child who's parent has died (or exited) without waiting for it to finish. In
this case, the operating system automatically assigns the `init` process as the new parent of that
child (i.e., the OS adopts the orphans!)
* __Zombie process__: A child that has exited but its parent did not execute a `wait` call for it.
In that case, even though the child has completed execution (i.e., terminated), the operating system
needed to retain the information about it to allow its parent to `wait` on it. Since the parent will
not execute a `wait` for the child process (either due to a bug or a crash), the child will remain a
zombie forever. Zombie processes are bad since they waste the OS resources.

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=6d4c6b05-d1c5-414c-8676-ac7d013fdbb5&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

# In-class activity
Finally, we take a look at our in-class activity. Please do not forget to submit your activity
[here](https://moodle.rose-hulman.edu/mod/assign/view.php?id=2708133) and your clear/muddy survey
[here](https://moodle.rose-hulman.edu/mod/quiz/view.php?id=2762514).

<iframe src="https://rose-hulman.hosted.panopto.com/Panopto/Pages/Embed.aspx?id=d50486f3-1744-4c88-9f89-ac7d015e6424&autoplay=false&offerviewer=true&showtitle=true&showbrand=false&start=0&interactivity=all" height="360" width="640" style="border: 1px solid #464646;" allowfullscreen allow="autoplay"></iframe>

