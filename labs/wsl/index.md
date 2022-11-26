---
layout: post
title: WSL2 Installation
readtime: true
date: Sat Nov 26 07:22:38 2022 
---

# Introduction and aim

We will need to have a working Linux distribution for this class. Since most of
you have been exposed to the Windows Subsystem for Linux (WSL) version 2 in
previous classes, we will adopt WSL2 as the subsystem of use for our purposes in
the class. In this document, we give you some pointers to install a working copy
of the Ubuntu distribution as part of WSL2, clone the class repository, connect
VSCode to the Ubuntu distribution, and some tips and tricks to access and manage
your file. 

# WSL2 installation

If you do not have WSL2 and a Linux distribution already installed, Microsoft
have created a great set of instructions that you can follow to enable wsl2 and
install the latest stable distribution of Ubuntu (Feel free to use another
distribution if you comfortable with it). These instructions can be found
[here](https://learn.microsoft.com/en-us/windows/wsl/install).

If you run into any issues during the installation process, please make a post
on [CampusWire]({{ site.campuswire_url }}) detailing the errors you faced along
with the version of wsl2 that you can trying to install. One of your instructors
(or fellow classmates) will reply to help you out as soon as possible. 

## [Optional] Installing the Windows terminal

In this class, you will need access to terminal emulator to install software and
compile your labs code. When you install wsl2 (assuming you install the Ubuntu
Linux distribution), a new Ubuntu terminal application will show up in your
launchpad. You can use that terminal emulator for all the needs in this class. 

Alternatively, you can use the [Windows
Terminal](https://learn.microsoft.com/en-us/windows/terminal/install)
application which allows you to connect your terminal emulator to the different
subsystems installed on your machine (like Windows Command Line prompt,
PowerShell, and any Linux distribution you have installed using wsl2). Either
applications (the Ubuntu terminal emulator or Windows Terminal) are equally good
for this class, it is totally up to you to choose whichever application is more
suitable for your needs. 

# Installing needed Linux software

After having a Linux distribution installed, you will need to install a few
software package to get you started in the class. To do so, open a Linux
terminal (either through the Ubuntu terminal emulator or using Windows Terminal)
and issue the following commands:
```shell
$ sudo apt update
```
and then
```shell
$ sudo apt install -y build-essential git gdb
```

There will be other software packages that you will need, but we will get to
those later on in the class. 

# Cloning the class repository

Let's first clone the class repository so that we are set up to start the labs.
You should have `git` installed on your Linux distribution in the previous step.
From your Linux terminal, issue the following command:
```shell
$ cd
```
This will move you to your Linux home directory (`/home/<your username>/`). Then
create a folder for this class and then `cd` (change directory) into it:
```shell
$ mkdir csse332
$ cd csse332/
```
and then clone the class repository using
```shell
git clone git@github.com:RHIT-CSSE/csse332.git
```
A new `csse332` directory should show up, you can use the `ls` command to list
the contents of your current directory. To know in which directory you are, you
can use the `pwd` command. 

Now that you have the class repository cloned, you can follow the instructions
[here]({{ site.baseurl }}/docs/making_a_private_repo) to push to a private copy
of the repository. 

# Some tips and tricks

## Open the current directory in VSCode

[VSCode](https://code.visualstudio.com/) is powerful editor that many of you
have shown interest in using.  Luckily, wsl2 has a nice and seamless integration
with VSCode that you can use to write your labs and assignments. 

First, make sure that VSCode is installed in your **Windows** distribution (not
in the Linux virtual machine). I am pretty sure that by this time of your
journey through college, you should have a working and configured copy of VSCode
running on your Windows machine. 

Then, to launch an instance of VSCode that is connected to your wsl2
distribution, use your **Linux terminal** and navigate (using the `cd` command)
to the `csse332` cloned repository, and then issue the following command:
```shell
$ code .
```
A VSCode instance should launch and it should be connected to your wsl2
virtual machine. 

## Open the current directory in Windows Explorer

To submit your code to [Gradescope]({{ site.gradescope_url }}), you will
probably need access to your Linux files from the Windows File Explorer. You can
easily access those from the Linux terminal emulator by first navigating to the
`csse332` repository and then using the following command:
```shell
$ explorer.exe .
```
A new file explorer window should show up that contains the content of your
`csse332` repository. 

## More tips and tricks

If you have any wsl2, VSCode, or Linux tips and tricks that you would like to
contribute, we would be more than happy to add them to this page. To submit your
contributions, please make a public post on [CampusWire]({{ site.campuswire_url
}}) and tag it with the `Tips and Tricks` tag. We will then review it and add it
to this page.

# Troubleshooting

If at any point of this document you run into issues with your installation or
have any questions, then please make a public post on [CampusWire]({{
  site.campuswire_url }}) and tag it with the `WSL2 Troubleshooting` tag. One of
the instruction staff should reply to you with details as soon as possible. 

# Submission

At the end of this lab, you should have a working copy of wsl2 with the Ubuntu
Linux distribution installed on your Windows machine. Once you are confident
about your setup, please submit your work for this lab by commenting on the
''Introductions Main Thread'' thread on [CampusWire]({{ site.campuswire_url }})
(you can find a direct link to the thread
[here](https://campuswire.com/c/G0EB73CA1/feed/1)) and introducing yourself.

Make sure your include the following information:
- Your name (i.e., don't make your post anonymous, otherwise we won't know who
  you are). 
- Your class and major. 
- Where you are from. 
- Your favorite book, movie, video game, TV series, etc.
- A book or movie you are currently reading/watching. 
- Your favorite thing about your journey at Rose so far.
- [Optional] A cute picture of your pet(s) if your have any. 

We will use your post to first get to you know, and second as an indicator that
you have completed your setup successfully and grade this lab accordingly.

