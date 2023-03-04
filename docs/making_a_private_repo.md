---
title: Creating a private repo
layout: post
date: Sat Nov 26 13:25:48 2022 
readtime: true
---

# How to Make a Personal Git Repo for CSSE332

The class material for CSSE332 is hosted in public GitHub repository.
Similarly, all xv6 labs are hosted in a public repository with different lab
branches. Given that setup, you might be tempted to fork your own personal copy
of these repositories so that you can maintain your code and keep copies of your
submissions. Please do not do that. 

The reason behind that is that a fork of a public repository is always going to
be public, and thus your solutions would be publicly available on GitHub, which
can be considered academic misconduct. In what follows, we give you another
approach to clone the course repository as well as maintain **private** copies
of your code and submissions. 


## Step 1: Make the Private Repo But Don't Clone It

First, create a **private** repository on GitHub using your RHIT account. Make
sure that the repository is empty and that it is set to be private. Do not clone
this repository yet, we will make use of it later on. However, make note of the
repository URL, it should look like the following:
```  
git@github.com:hewner/ExampleCSSERepo.git
```

You will also want to create another empty, private, repository for the xv6 labs
and make note of its URL. 

## Step 2: Add A Remote To Your Local Copy of the Course Repo

Second, let's set things up so that we pull from the public course repository
and we push to your private repository. Launch a terminal window and navigate to
where you cloned the main CSSE332 repository. In what follows, we will assume
that you have cloned the course repository in `/home/username/csse332/csse332`.
Therefore, you would want to do something like
```shell
$ cd ~/csse332/csse332
```

Currently, the repository is set to push and pull from the same remote, this is
not what we want. We want our repository to pull from the class repo and to push
to your private repo. To achieve that, from the root of the CSSE332 repository,
issue the following command
```shell
$ git remote set-url --push origin <YOUR PRIVATE REPO URL>
```

Now, all of your `git pull` commands will pull from the course public repository
and all of your `git push` commands will push to your own private repository.
Putting everything together using the example we used in the previous section,
it should look something like:
```shell
$ cd ~/csse332/csse332
$ git remote set-url --push origin git@github.com:hewner/ExampleCSSERepo.git
$ git push
```

# FAQ

## How can I minimize merge conflicts?

Setting up your private repository in the way we suggested above runs the risk
of running into merge conflicts in case both you and I edit the same files,
especially for the course content repository (less so on the xv6 repository).
If you are confident in your abilities to handle git merge conflicts, then
please feel free to skip the following section. 

If you would like to avoid merge conflicts on the course content repository,
then an easy way to do so is to copy your solution into a separate directory
before you start working on the assignment. For example, if you are working on
the process lab homework, then can do the following:
```shell
$ cd ~/csse332/csse332
$ git pull
$ cd homeworks
$ cp -r 04_process_lab 04_process_lab_solution
$ git add 04_process_lab_solution
$ git commit -m "starting the process lab assignment"
$ git push
```

For the xv6 assignments and project, we have set things up to have each lab in a
branch to minimize the possibility of merge conflicts. Therefore it is safe to
assume that once an xv6 assignment is released, there will be no more changes to
that branch, so it is safe to work on the files directly. 

## Have more git tips and tricks?

If you have more git setup tips and tricks that you would like to share with us
and the rest of the class, then we encourage you to do so. Please create a
public note on the [Moodle forum]({{ site.moodle_url }}) We will review your
suggestion and add it to this page. 

<!--
## What if I already have a public forked repo?

That's bad, and you should delete it.  BUT we can save everything
you've done in your newly created private repo.  First follow the
steps above.  Then get the url of your fork, add it as a remote, and
pull:

    git remote add badrepo git@github.com:hewner/ForkIShouldDelete.git
    git pull badrepo master
    git push
    
The final push should push all your old changes to your new private
repo.  Once you verify that everything is there, delete that bad repo.

-->
