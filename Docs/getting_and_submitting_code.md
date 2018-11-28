---
layout: togit
---
In 332 you will checkout code via git but you will submit code via Moodle.

# Getting Code

You will want to install git on your linux OS (be it virtual machine
or real local OS).  It's usually as easy as a package install
(e.g. sudo apt-get install git for you ubuntu users), be each linux
flavor is a little different.

Once it is installed, check out the public course repo:

    git clone https://github.com/RHIT-CSSE/csse332.git
    
That should check out a complete copy of the 332 source materials
(including slides, homework code, sample exams, etc.) to your local
system in a csse332 directory.

As new stuff is posted to the repo, you may need to update your local
copy of the repo.  To get the latest, cd into your csse332 directory
and then:

    git pull

Note: one thing you should definitely *not* do is "fork" the public
git repo via github.  That is because if you commit/push your homework
solutions, you'll be committing them to a public repo that other
students can find and use to cheat.  If you really want to be able to
store your homework code in a git repo, you can create a private repo
on ada.csse.rose-hulman.edu and we can show you how to get that setup.
You'll still have to submit any solutions to us via Moodle however.

# Submitting Code

You'll submit code via Moodle.  Click on the relevant assignment and
upload all the files (you can drag and drop from you file chooser if
you like).

PLEASE - do not zip all your files and upload them as one zip.  That
makes it much more annoying for us to grade.

## What files should I upload?

Upload all source files

* All .h or .c files, including files you didn't edit (like tests, etc.)
* Also any Makefiles or anything else required to build your code
* Don't upload compiled binary files (.o files, executable files)




