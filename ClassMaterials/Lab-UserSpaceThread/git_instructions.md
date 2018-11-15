---
layout: page
title: "Git Instructions"
premalink: /git_instructions/

---

### Introduction

This term we'll be using git and Rose Hulman's server ada.csse to do our
assignment submission.  Follow these steps in your linux environment.

> *NOTE*: follow these instructions on linux (e.g. in your virtual
machine).  Though you can setup windows in a similar way, you'll code
and submit all your assignments for this class in linux so that's
where it will be most convenient to have your SSH keys setup, etc.

#### Step 1: Generating your credentials

1. Go to [https://ada.csse.rose-hulman.edu/](https://ada.csse.rose-hulman.edu/)
2. Login with your Rose Hulman username and password
3. Click the icon in the upper right corner of the screen and click
   "Settings"
4. On the left hand side of the screen, click SSH Keys
5. Follow the instructions in the "generate it" link from the page,
   then pasted the key into the textbox and click add

#### Step 2: Doing a checkout

1. In the ada.csse interface, click on the "Projects" link on the upper
    left corner of the screen
2. You should see a list of projects - find the one associated with
   this class (mine is called something like
   `201830-csse332-all/spring-2018-hewner`).  Click it.
3. In the center of the page next to the word SSH there should be a
   URL (mine looks like
   `git@ada.csse.rose-hulman.edu:201830-csse332-all/spring-2018-hewner.git`
   ).  Copy it.
4. Open a command line and navigate to wherever you'd like to do a
   checkout.  Then do this

   `git clone <your copied url>`

5. You should see that some stuff was pulled down.  There should now
   be a directory with the name of your repo (mine is
   spring-2018-hewner) that has files you can look through.

#### Step 3: Submitting a test commit

1. Make some changes to the files.
2. At the command line, when you are happy with your changes, use `git
   add <filename>` to mark all the files you'd like to commit.
3. Type
        `git fetch -v`

   If things are good, it should say everything is update to date.  If
   it downloads a change, you probably need to do a merge and that's
   beyond the scope of this document.

3. Type
        `git status`
   
   You should see a summary that has all the files you want to submit
   in "Changes to be committed".  If it's missing something, that's
   probably because you forgot to git add that file.

4. Type 
        `git commit -m "some message you want associated with the commit"`
   
   This should print a summary of how many things changed if it
   completes successfully.

5. Type
        `git push`

   This should show that it updated the branch on the remote server.

6. Type
        `git status`

   And verify there are no things uncommitted that you wanted to be
   committed and it says the branch is up to date.
