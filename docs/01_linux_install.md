---
layout: post
title: Linux Installation
readtime: true
date: Tue Mar  9 13:08:35 2021
gh-repo: rhit-csse332/csse332-202130
gh-badge: [star, watch, follow]
---

## Introduction and aim
You will need to have a working Linux installation for this class. The official reference version is
Ubuntu 20.04 LTS, but other versions would probably do the job as well. 

What will not work in this class are the following:
- Windows programs that emulate a Linux environment (e.g., `cygwin`)
- MacOS
- For the project, the Windows 10 Linux Subsystem will not work

I highly recommend that you install a Linux virtual machine since you can use it to do all of your
assignments as well as your project. However, you could still opt for using the Windows 10 Linux
Subsystem for your assignments and a virtual machine for the project. 

## Using a virtual machine
This is the common approach and is the one I highly recommend for this class. 

We have provided you with a virtualbox virtual machine onto which we have already installed all of
the necessary packages for this class. To use, follow the instructions below:
1. Download VirtualBox from the [official website](https://www.virtualbox.org/wiki/Downloads) and
   follow the installation instructions. 
2. Download and install the latest VirtualBox extension pack ([direct
   link](https://download.virtualbox.org/virtualbox/6.1.18/Oracle_VM_VirtualBox_Extension_Pack-6.1.18.vbox-extpack)).
3. Download this [pre-built virtual machine
   image](http://mirror.csse.rose-hulman.edu/.csse332/CSSE332.ova). Note that you will need to be
   on the campus VPN to be able to access it. 

Once you do that, you should be able to import that file into VirtualBox and start using the Linux
distribution. You can find the login information below
```
 username: ubuntu
 password: csse332
 ```

## Windows 10 Linux Subsystem
This is a relatively new option that is easier on the resources of your machine, especially for the
assignments. However, I do not recommend this for two reasons:
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


The editor might not be as needed as you think though, because handy
thing about this approach is that your windows filesystem is mapped in
linux.  For example `/mnt/c/Users/<YourName>/Desktop/` probably contains
your windows desktop.  This means if you clone your csse332 repo
somewhere on your windows filesystem, you can edit your code with your
favorite windows editor. You can also access your Linux folder by following
the instructions
[HERE](https://askubuntu.com/questions/759880/where-is-the-ubuntu-file-system-root-directory-in-windows-subsystem-for-linux-an)

Also, the recent version of VSCode has a seamless integration with the Linux subsystem that allows
you to use `gcc` and other Linux tools right from your IDE's window. 

## A note on editors
I encourage you in this class to get familiar with terminal-based text editors. I understand that
you have been primed to avoid the command line like the plague (or like COVID?), yet I believe
that it will be highly beneficial for you in your career if you can navigate your way around a
terminal, especially when configuring certain services. 

My favorite editor is `vim`, but `emacs` or `nano` work equally well. I encourage you to live
dangerously and try to complete at least one assignment completely in a terminal window. 

## Submission
I will verify that you have installed a workable Linux distribution on the first day of class.
I highly recommend that you do this before coming to class so we could use class time to solve any
problems or issues that arise in the process. 
