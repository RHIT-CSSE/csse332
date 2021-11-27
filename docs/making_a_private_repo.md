---
title: Creating a private repo
layout: post
date: Mon Mar 8 14:09:52 2021
readtime: true
---

# How to Make a Personal Git Repo for CSSE332

Given that that materials for CSSE332 are in a github repo, you might
be tempted to fork your own personal copy.  DO NOT DO THAT.  The issue
is that a fork of a public repo is always public, so if you commit
assignment solutions or other stuff you'll be publishing your CSSE
assignments where other students can find them.  That's academic
misconduct.

## Step 1: Make the Private Repo But Don't Clone It

But if you like to use git for your own development work, it's easy to
make a private repo you can use as a personal backup.  First step is
to make the repo itself.  You can use github or the CSSE gitlab ada or any
other host - the only requirement is that the repo be PRIVATE to
everybody but you.  Create the private repo and note it's url - mine
looks like this:

    git@github.com:hewner/ExampleCSSERepo.git

You'll want a unique repo for each course you'd like to backup.

## Step 2: Add A Remote To Your Local Copy of the Course Repo

Now open up your command line and go to where you've checked out your
local copy of the course repo.  Then type add 

    git remote set-url --push origin <your url>

Henceforth all "git pull" commands will pull from the course repo but
all "git push" commands will push to your private backup.

So if I want to make a private backup of the csse332 repo which I had
cloned in `~/csse332` it would look like this:

    cd ~/csse332
    git remote set-url  --push origin git@github.com:hewner/ExampleCSSERepo.git
    git push

That's it!

# FAQ

## How can I minimize merge conflicts?

So if you edit files on your local repo and the professor edits those
same files in the course repo, doing a 'git pull' can put you in a
conflicted state.  I suggest you copy directories before you start
editing them.  Eg:

    cd ~/csse332/Homework/
    cp -r SimpleShell SimpleShellSolution
    git add SimpleShellSolution
    git commit -m "Starting the SimpleShell assignment"
    git push
   
This is 100% optional though, especially if you're savvy about
handling git merge conflicts.

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

