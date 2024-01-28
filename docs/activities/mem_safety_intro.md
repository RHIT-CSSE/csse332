---
layout: post
title: Introduction to Memory Safety
readtime: true
date: 'Sun Jan 28 18:27:02 2024'
last-updated: 'Sun Jan 28 18:27:08 2024'
---

# Introduction

In this activity, we will explore basic concepts of memory safety and control
flow integrity attacks. We will use input to a vulnerable software to modify
the data flow and the control flow of a program, in the goal of eventually take
over the machine running the program.

# Getting the source

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

The code for this activity is found under the `mem_safety_intro` directory
under `acitivities`. 

# Installing tools

We need a few tools for this activity and for your next lab, so here's how you
can get them. We will need to cross compile code for the Intel i386 32 bits
architecture, so we must have the necessary compiler and libraries. To download
and install those, use:

```sh
sudo apt install -y gdb gdb-multiarch gcc-multilib python2
```

We will also attach a better wrapper on top of `gdb` to be able to explore
memory more clearly. We will use `gef` to achieve that. You can install `gef`
using:

```sh
bash -c "$(curl -fsSL https://gef.blah.cat/sh)"
```

If `curl` is not installed, then you can install it using `sudo apt install -y
curl`.

## Upgrade WSL1

It seems that WSL1 on Windows is not able to run 32 bit applications. To be able
to do this lab, you must be running on WSL2. To upgrade from WSL1 to WSL2,
follow the instructions
[here](https://dev.to/adityakanekar/upgrading-from-wsl1-to-wsl2-1fl9) and
[here](https://learn.microsoft.com/en-us/windows/wsl/install#change-the-default-linux-distribution-installed).
If running the `wsl` command from Powershell does not work, then you are running
an older version of Windows, and you need to manually upgrade WSL by following
the instructions
[here](https://learn.microsoft.com/en-us/windows/wsl/install-manual).

Note that you can still run Linux virtual machines on WSL1 if you require them,
you can use the command `wsl --set-version <vm-name> 1` from Powershell, where
`<vm-name>` is the name of the distribution that you would like to run on WSL1.

