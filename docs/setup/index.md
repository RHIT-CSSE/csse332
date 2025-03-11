---
title: Repository Setup
layout: post
readtime: true
date: Fri 24 Nov 2023
last-updated: Wed 22 May 2024
---

# Table of contents

<!-- vim-markdown-toc GFM -->

* [CSSE 332 labs](#csse-332-labs)
  * [1. Create a __private__ repository](#1-create-a-__private__-repository)
    * [Make sure your repository is private](#make-sure-your-repository-is-private)
  * [2. Clone the repository](#2-clone-the-repository)
  * [3. Adjust the repository's remote](#3-adjust-the-repositorys-remote)
  * [4. Fetch the changes from the class repository](#4-fetch-the-changes-from-the-class-repository)
  * [5. Push initial commit to your private repository](#5-push-initial-commit-to-your-private-repository)
  * [6. Making your own changes](#6-making-your-own-changes)
    * [Some hints](#some-hints)
  * [7. Obtaining changes from the class repository](#7-obtaining-changes-from-the-class-repository)
  * [8. Discarding changes to a file](#8-discarding-changes-to-a-file)

<!-- vim-markdown-toc -->

# CSSE 332 labs

All of the labs for this class will be maintained under one repository that you
can find on GitHub using this [link]({{ site.gh_labs_repo }}). We will be
updating this repository with new labs as we continue with the quarter, so it
is essential that you can keep track of it.

At the same time, you should also be able to maintain your own code in a
__private__ repository so that you never lose your progress, and can easily work
on several machines.

In what follows, we detail the steps necessary for you to maintain a private
course repository that can obtain updates from our labs.

## 1. Create a __private__ repository

Using your Github account, create a new, empty, and __private__ repository. Do
not initialize that repository with a README file, do not create a license, and
make sure that the repository is completely empty.

The repository you created will host all of your source code for submitting
labs, so we suggest you give it a meaningful name, such as
`csse332-labs-<username>` where `<username>` is your RHIT username. For example,
I would want to create a repository called `csse332-labs-noureddi`.

In what follows, we will assume that your repository is called
`csse332-labs-user`.

### Make sure your repository is private

Please make sure that your created repository is __private__. Posting your
solutions on a public repository is considered a form of academic misconduct,
and will be treated as such, as mentioned in the class
[syllabus]({{site.baseurl}}{{site.syllabuspath}}).

## 2. Clone the repository

First, copy the `ssh` URL to clone the repository from the GitHub webpage
corresponding to your newly created repository (make sure to toggle the `ssh`
option). The URL should start with `git@<your username>`. Then, in your
terminal window, clone the labs repository using the following command

  ```shell
  $ git clone <your_ssh_clone_repository_url_here>
  ...
  warning: You appear to have cloned an empty repository.
  ```

Note that the line `warning: You appear to have cloned an empty repository.`
**should** show up when you clone this repository. If it does not, then you
repository is not empty and you will need to delete it and recreate a new and
empty one.

### Authentication Issues?

If you get an authentication problem when trying to clone your private
repository, then please refer to [this
guide]({{site.baseurl}}/docs/setup/ssh_keys) for instructions on how to add
your SSH keys to you GitHub account.

## 3. Adjust the repository's remote

We would like to set up your repository in a way that you can pull from two
locations:

  1. `origin`: that would be your own repository (in case you need to move
     between devices).

  2. `upstream`: that would be the class's repository containing new labs.

However, when you finish a lab and commit your changes, we would like to `push`
those changes to your private repository, i.e., only to `origin`.

To do so, first navigate to your labs repository:

  ```shell
  $ cd csse332-labs-user/
  ```

Your directory should be empty (confirm that with `ls`). Next, let's add a new
`upstream` remote pointing to the class repository. To do so, use the following:

  ```shell
  $ git remote add upstream https://github.com/rhit-csse332/csse332-labs.git
  ```

Then, confirm that a new remote shows up in your repository, as follows:

  ```shell
  $ git remote -v
  origin  github.com:user/csse332-labs-user.git (fetch)
  origin  github.com:user/csse332-labs-user.git (push)
  upstream        https://github.com/rhit-csse332/csse332-labs.git (fetch)
  upstream        https://github.com/rhit-csse332/csse332-labs.git (push)
  ```

## 4. Fetch the changes from the class repository

Now, let's obtain the class content from the class repository using:

  ```shell
  $ git fetch upstream
  remote: Enumerating objects: 4, done.
  remote: Counting objects: 100% (4/4), done.
  remote: Compressing objects: 100% (4/4), done.
  remote: Total 4 (delta 0), reused 4 (delta 0), pack-reused 0
  Unpacking objects: 100% (4/4), 2.51 KiB | 366.00 KiB/s, done.
  From github.com:rhit-csse332/csse332-labs
   * [new branch]      main       -> upstream/main
  ```

Then, pull the changes:

  ```shell
  $ git pull upstream main
  From github.com:rhit-csse332/csse332-labs
   * branch            main       -> FETCH_HEAD
  ```

Now, if you check the content of your directory, the starter code for the labs
will show up in there. Confirm that using `ls`.

## 5. Push initial commit to your private repository

Now, we need to sync things up to your private repository, to do so, you should
use:

  ```shell
  $ git push origin main
  Enumerating objects: 4, done.
  Counting objects: 100% (4/4), done.
  Delta compression using up to 8 threads
  Compressing objects: 100% (4/4), done.
  Writing objects: 100% (4/4), 2.53 KiB | 2.53 MiB/s, done.
  Total 4 (delta 0), reused 0 (delta 0), pack-reused 0
  To github.com:user/csse332-labs-user.git
   * [new branch]      main -> main
  ```

Using your browser, verify that everything now shows up on your
`csse332-labs-user` private repository.

## 6. Making your own changes

When attempting the labs, you can treat this repository just like any other
private repository where you can maintain your code. Say you modified a file
called `solution.c`, then you can follow this workflow to push it to your
private repository.

Add the file to the change list:

  ```shell
  $ git add solution.c
  ```

Commit your changes:

  ```shell
  $ git commit -m "finished lab 1"
  ```

Push your changes:

  ```shell
  $ git push
  ```

This will push the changes to your own private repository.

### Some hints

Recall that at any time, you can check the status of your repository using:

  ```shell
  $ git status
  ```

Also, to add all _modified_ files in one shot (without adding them manually),
you can use:

  ```shell
  $ git add -am "finished lab 2"
  ```

__Note__ that the above command will add all _modified_ files to the change
list, but will not add your newly created files (if any); you will have to add
those separately using `git add`.

Finally, please do not commit any object files (`.o`), binary files
(executables), vim swap files (`.swp`), and anything that is compiled and
generated, those will create a mess if you work across different devices and
will pollute your repository.

## 7. Obtaining changes from the class repository

When we add a new lab assignment, you can obtain the added changes using

  ```shell
  $ git pull upstream main
  ```

Then push those changes to your private repo using:

  ```shell
  $ git push
  ```

We will organize things in separate folders to minimize merge conflicts as much
as possible. If you work within each lab's own directory, you should not have
any conflicts arise from obtaining new changes from the class repository.

## 8. Discarding changes to a file

If at any point in time, you made changes to a file, say `lab1.c`, but would
like to restart your lab, you can restore the file to its original version from
the class repository using:

  ```shell
  $ git checkout upstream/main -- lab1.c
  ```

__Note__ that this will discard all changes you have made to `lab1.c`, so make
sure that this is really the behavior you would like to see.

---

This page was last edited by Mohammad Noureddine on {{ page.last-updated }}. If
you notice any typos or inaccuracies, please open a GitHub issue on this
[repository]({{site.gh_repository_url}}).

