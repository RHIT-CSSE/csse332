---
layout: post
title: WSL2 Installation
readtime: true
date: Fri 24 Nov 2023
---


<!-- vim-markdown-toc GFM -->

* [Introduction and aim](#introduction-and-aim)
* [WSL2 installation](#wsl2-installation)
  * [[Optional] Installing the Windows terminal](#optional-installing-the-windows-terminal)
* [Installing needed Linux software](#installing-needed-linux-software)
* [Some tips and tricks](#some-tips-and-tricks)
  * [Open the current directory in Windows Explorer](#open-the-current-directory-in-windows-explorer)
* [Other references](#other-references)

<!-- vim-markdown-toc -->

# Introduction and aim

We will need to have a working Linux distribution for this class. Since most of
you have been exposed to the Windows Subsystem for Linux (WSL) version 2 in
previous classes, we will adopt WSL2 as the subsystem of use for our purposes in
the class. In this document, we give you some pointers to install a working copy
of the Ubuntu distribution as part of WSL2, clone the class repository, connect
VSCode to the Ubuntu distribution, and some tips and tricks to access and manage
your file.

**Please make sure that you are running WSL2 and not WSL1.** If you still need
to run WSL1 for another class, then you can easily specify which version of WSL
you want for each virtual machine you run.

# WSL2 installation

If you do not have WSL2 and a Linux distribution already installed, Microsoft
have created a great set of instructions that you can follow to enable wsl2 and
install the latest stable distribution of Ubuntu (Feel free to use another
distribution if you comfortable with it). These instructions can be found
[here](https://learn.microsoft.com/en-us/windows/wsl/install).

If you run into any issues during the installation process, please make a post
on the [Moodle forum]({{ site.moodle_url }}) detailing the errors you faced
along with the version of wsl2 that you can trying to install. One of your
instructors (or fellow classmates) will reply to help you out as soon as
possible.

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
software packages to get you started in the class. To do so, open a Linux
terminal (either through the Ubuntu terminal emulator or using Windows Terminal)
and issue the following commands:
  ```shell
  $ sudo apt update
  ```
and then
  ```shell
  $ sudo apt install -y build-essential git \
        gdb \
        python3 \
        python-is-python3 \
        ipython3 \
        gdb-multiarch \
        qemu-system-misc \
        gcc-riscv64-linux-gnu \
        binutils-riscv64-linux-gnu
  ```

# Some tips and tricks

## Open the current directory in Windows Explorer

To submit your code to [Gradescope]({{ site.gradescope_url }}), you will
probably need access to your Linux files from the Windows File Explorer. You can
easily access those from the Linux terminal emulator by first navigating to the
desired repository and then using the following command:
```shell
$ explorer.exe .
```
A new file explorer window should show up that contains the content of your
repository.

# Other references

You have already used WSL2 in several classes prior to CSSE332, specifically,
CSSE132.  Therefore, if you need some more resources on WSL2, feel free to go
back to the CSSE132 course webpage and check out the instructions there. Here is
a [direct
link](https://www.rose-hulman.edu/class/csse/csse132/2324b/labs/prelab1-wsl2.html)
to the current version of those instructions.

<!--
% CHANGELOG
%
% Fri 24 Nov 2023   Removed mentioning vscode as we won't be using it!
%                   Updated list of dependencies to now include everything
%                    needed to build xv6. 
%                   Added table of contents.
%
-->
