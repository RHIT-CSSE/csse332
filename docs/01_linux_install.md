---
layout: post
title: Linux Installation
readtime: true
date: Thu 26 Aug 2021
gh-repo: rhit-csse332/csse332
gh-badge: [star, watch, follow]
---

# Introduction and aim

You will need to have a working Linux installation for this class. The official
reference version is Ubuntu 20.04 LTS, but other versions would probably do the
job as well. The instructions below work for both Windows and macOS.

Note that you could still opt for using the Windows 10 Linux Subsystem (WSL2)
for your assignments and a virtual machine for the project, but you will have
to debug WSL2 on your own.

I highly recommend that you install this Linux virtual machine early before the
quarter starts so you can be ready from day one.

## Table of contents


<!-- vim-markdown-toc GFM -->

* [Installing VirtualBox](#installing-virtualbox)
* [Importing the Class VM](#importing-the-class-vm)
  * [Verify Network Configuration](#verify-network-configuration)
* [Starting the Virtual Machine](#starting-the-virtual-machine)
  * [Optional: Test ssh connectivity](#optional-test-ssh-connectivity)
* [Hooking VSCode to the VM](#hooking-vscode-to-the-vm)
  * [Obtaining a terminal](#obtaining-a-terminal)
* [Troubleshooting](#troubleshooting)
* [A note on editors](#a-note-on-editors)
* [Submission](#submission)
* [UNSUPPORTED: Windows 10 Linux Subsystem](#unsupported-windows-10-linux-subsystem)

<!-- vim-markdown-toc -->

# Installing VirtualBox

Let's get started by installing VirtualBox, a hypervisor that will
allow you to run a virtual machine on your laptop.
First, download the latest version of VirtualBox from [the official
website](https://www.virtualbox.org/wiki/Downloads) and follow the installation
instructions.

Next, download and install (simply by double-clicking) the VirtualBox Extension
Pack from the following [direct
link](https://download.virtualbox.org/virtualbox/6.1.30/Oracle_VM_VirtualBox_Extension_Pack-6.1.30.vbox-extpack).

# Importing the Class VM

As a courtesy, we have provided you with a virtual machine image that contains
all of the tools and packages you need to survive in this class.
1. Download the virtual machine image from [this direct
   link](http://mirror.csse.rose-hulman.edu/.csse332/csse332.ova).
  - The file is 1.3GB in size and can only be downloaded from campus or on the
    campus VPN.
  - This file is a pre-built virtual machine that runs a headless (i.e., without a
    GUI) Ubuntu server. You will develop your assignment solutions and kernel
    project on this server.

2. Launch `VirtualBox` and select `Tools`, `Import`.

    ![00_import]({{ site.baseurl }}/assets/img/00_import.png)

3. In the `Appliance to import` window:
  - Choose `Local File System` as the `Source`
  - Click on the small folder with the green arrow
  - Navigate to the file `csse332.ova` that you downloaded in the previous step,
  and then select that file.
  - Click on `Continue

4. On the `Appliance settings` window
  - Leave all the options as defaults
  - Click on `Import`

5. A new virtual machine by the name of `csse332` should appear on the left hand
   side of the VirtualBox menu, as shown below

   ![01_after]({{ site.baseurl }}/assets/img/01_after.png)

## Verify Network Configuration

Before starting the virtual machine, let's first make sure that the network
configuration options are correct.

- Click on the `csse332` virtual machine and then on the `Settings` icon (the
  gear icon).
- Click on the `Network` tab and then expand the `Advanced` toggle, you should
  see something similar to the following:

    ![02_network]({{ site.baseurl }}/assets/img/02_network.png)

- Click on `Port Forwarding` and make sure that the rules you see look as
  follows:

    ![03_verify]({{ site.baseurl }}/assets/img/03_verify.png)

Here's a cute GIF that walks you through the steps of verifying the network
configuration
![04_verify]({{ site.baseurl }}/assets/img/04_verify.gif)

# Starting the Virtual Machine

Now let's start the virtual machine and verify that it is running.

1. Click on the `csse332` virtual machine
2. Click on the green `Start` arrow
3. A new window will launch and your virtual appliance will start loading and
   then finally drop into a shell as shown below:

   ![05_shell]({{ site.baseurl }}/assets/img/05_shell.png)

4. You can now login to the virtual machine using the username `csse` and the
   password `csse`.

   > **Note that the password prompt will not show the characters as you
   are typing them, but rest assured, the keys are being sent to the VM**

5. Play around with the terminal window and learn some new Unix commands, they
   will be very handy in this class. To install software or do admin stuff,
   preface your commands with `sudo` as shown below:

   <img src="https://imgs.xkcd.com/comics/sandwich.png" />

## Optional: Test ssh connectivity

This part is optional and requires you to be already familiar with managing a
terminal.

To further verify the installation, launch a terminal on your host machine (the
host is the OS running VirtualBox, as opposed to the guest, which is the OS
running in VirtualBox), and then type
```shell
ssh -p 2222 csse@localhost
```
You should get a prompt that asks you if you'd like to accept the identity of
the remote server. Type `yes` and then you will get a password prompt. Type the
password (remember, it's `csse`) and make sure that you can log in to the
virtual machine.

# Hooking VSCode to the VM

Finally, let's hook `VSCode` to the virtual machine so you can start developing
your assignments.

1. Power off the virtual machine.
2. From the `VirtualBox` window, click on the `csse332` virtual machine
3. Click the small downward arrow at the bottom of the green start arrow and
   choose `Headless Start`.
4. The virtual machine will now run the background and won't bother you anymore.
1. Download and install `VSCode` on your laptop or desktop machine.
2. Install the `Remote Development` extension by following this
   [link](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.vscode-remote-extensionpack)

After you install the extension, open the Command Palette (F1 for Windows or Cmd
\+ Shift + P for macOS) and select the
```
   Remote-SSH: Connect to host...
```
option from the drop down menu.

1. Select `Configure SSH hosts...`
2. For the location, select the location that contains the path `.ssh/config`
   (usually, it is the first one on all platforms)
3. Add the following to the configuration file
```
Host csse332
  HostName localhost
  Port 2222
  User csse
```
4. Save the file and close it.
5. Open the Command Palette again (F1 or Cmd + Shift + P) and select
   `Remote-SSH: Connect to host...` again.
6. The drop down menu should now contain an entry called `csse332`, click on
   that one and then enter the virtual machine's password when prompted.
   Here's a cute GIF that shows you what to expect:

   ![06_vscode]({{ site.baseurl }}/assets/img/06_vscode.gif)
7. Now your `VSCode` instance is connected to the remote virtual machine and you
   can open the course repository in `VSCode` using the file explorer.

## Obtaining a terminal
To get a terminal logged into the virtual machine, simply launch a terminal
instance from `VSCode` and it will automatically connect to the class machine.

> **NOTE**: The virtual machine already contains a copy of the class repo under
`/home/csse/csse332/`. Make sure to always update your copy using `git pull`

# Troubleshooting
For the virtual machine installation, contact your instructor or TA to get help
if you feel stuck at a certain point.

# A note on editors
I encourage you in this class to get familiar with terminal-based text editors.
I understand that you have been primed to avoid the command line like the
plague (or like COVID?), yet I believe that it will be highly beneficial for
you in your career if you can navigate your way around a terminal, especially
when configuring certain services.

My favorite editor is `vim`, but `emacs` or `nano` work equally well. I
encourage you to live dangerously and try to complete at least one assignment
completely in a terminal window.

# Submission
I will verify that you have installed a workable Linux distribution on the
second day of class.  I highly recommend that you do this before coming to
class so we could use class time to solve any problems or issues that arise in
the process.

---

# UNSUPPORTED: Windows 10 Linux Subsystem

This is a relatively new option that is easier on the resources of your machine,
especially for the assignments. However, I do not recommend this for two
reasons:
1. You will miss out on the opportunity to try out a different operating system.
2. You will not be able to use this option for your term project.

However, if you'd still like to play around with it, here are the instructions.

1. Follow the instructions to install WSL from [here](https://docs.microsoft.com/en-us/windows/wsl/install-win10).
   I tried this using WSL2 and Ubuntu 20.04 LTS.
2. Open a Linux terminal and install the following packages:
```shell
sudo apt update
sudo apt install -y git qemu bin86 nasm bcc build-essential hexedit bochs bochs-sdl
```
<!--
To install the X server for GUI apps:
* Download [VcXsrv](https://sourceforge.net/projects/vcxsrv/) and install
* In bash run "export DISPLAY=:0" Now when you run it you should get a display to pop-up, there may
be issues related to graphics drivers. Sadly, this is where the instructions diverge if you don't
have an NVIDIA graphics card.
* Locate the XLaunch shortcut in the Start Menu, and click it
* Install GLUT with `sudo apt-get install freeglut3-dev`
* Run this command `export DISPLAY=$(cat /etc/resolv.conf | grep nameserver | awk '{print $2}'):0`
* Go to **Windows Security -> Firewall & network protection -> Allow an app through firewall**. Make sure VcXsrv has both public and private checked.
* Launch VcXsrv with "Disable access control" ticked
* To test if the X server installation is successful,
  * Install `xeyes` by running `sudo apt install x11-apps`.
  * Open terminal and run `xeyes`.
  * If the installation is good, you should see a pair of eyes pop up. If not refer to [this
  document]({{ site.baseurl }}/docs/WSL_and_VcXsrv.pdf) for troubleshooting. Kudos to Sam Dickinson
  who put together this doc.
-->

The editor might not be as needed as you think though, because handy
thing about this approach is that your windows filesystem is mapped in
linux.  For example `/mnt/c/Users/<YourName>/Desktop/` probably contains
your windows desktop.  This means if you clone your csse332 repo
somewhere on your windows filesystem, you can edit your code with your
favorite windows editor. You can also access your Linux folder by following
the instructions
[HERE](https://askubuntu.com/questions/759880/where-is-the-ubuntu-file-system-root-directory-in-windows-subsystem-for-linux-an)

Also, the recent version of VSCode has a seamless integration with the Linux
subsystem that allows you to use `gcc` and other Linux tools right from your
IDE's window.


