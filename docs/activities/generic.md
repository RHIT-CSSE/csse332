---
layout: post
title: Lazy Paging Activity
readtime: true
date: Wed Jan 10 21:23:27 2024
last-updated: Wed Jan 10 21:23:33 2024
---

# Getting the source code of an activity

The source code of this activity will reside in the acitivities directory in
the labs repository. Here are the steps needed to access the updated
activities.

## Back up your lab code 

If you are currently working in another lab, make sure to back up your progress
before switching branches.

```sh
$ git commit -am 'saving progress'
$ git push
```

## Getting on the right branch

Then, we need to switch to the `main` branch using

```sh
$ git checkout main
```

Next, fetch the updates and pull them into your `main` branch:

```sh
$ git fetch upstream
$ git pull upstream main
```

If you are asked for a merge message, enter any appropriate message and exit
the editor, if any.

## Do the activity and push

After you are done with the activity, make sure to commit and push your changes
using:

```sh
$ git commit -am 'acitivity done'
$ git push origin main
```

