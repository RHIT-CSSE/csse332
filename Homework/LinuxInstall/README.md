
You need to have a working linux installation for this class.  The
official reference version is Ubuntu 17.10 but other versions of Linux
can probably be made to work if you're reasonably savvy about
installing packages.

What will definitely NOT work:

-   Windows programs that emulate a linux environment (e.g. cygwin)
-   MacOS

There are three ways you can install linux.

# A Virtual Machine

This is the historically the most common approach.

We've created a virtualbox virtual machine and we've pre-installed all
the needed packages.  To use it you must install virtualbox for you
particular OS.  Follow the instructions [on virtualbox's
website](https://www.virtualbox.org/wiki/Downloads)

You will also want the extension pack, downloaded at the same place.

Install VirtualBox on your system, and [then download this
file](http://mirror.csse.rose-hulman.edu/.csse332/CSSE332.ova).

You should be able to run that file and it should install an
appropriate virtual machine on your system.  The first time it starts
up it should ask you to configure a username and password (for max
ease of use, I recommend you use the same username as your rose
login).


# Windows 10 Linux Subsystem

This is a new option this term.  It's easier and faster than other
approaches, but we may discover compatability problems with some of
the labs.  At this point, I only reccommend it for folks who feel
confident debugging any weird issues that might arise.

Follow the instructions here:

https://docs.microsoft.com/en-us/windows/wsl/install-win10

I used WSL2 and the Ubuntu 20.04 LTS for my distribution.

Run this command on your unix command line to install the necessary packages:

    sudo apt update
    sudo apt install git qemu bin86 nasm bcc build-essential hexedit bochs bochs-sdl

Install X server for GUI apps:

* Download (VcXsrv)[https://sourceforge.net/projects/vcxsrv/] and install
* In bash run "export DISPLAY=:0" Now when you run it you should get a display to pop-up, there may be issues related to graphics drivers. Sadly, this is where the instructions diverge if you don't have an NVIDIA graphics card.
* Locate the XLaunch shortcut in the Start Menu, and click it
* Install GLUT with `sudo apt-get install freeglut3-dev`
* Run this command `export DISPLAY=$(cat /etc/resolv.conf | grep nameserver | awk '{print $2}'):0`
* Go to **Windows Security -> Firewall & network protection -> Allow an app through firewall**. Make sure VcXsrv has both public and private checked.
* Launch VcXsrv with "Disable access control" ticked
* To test if the X server installation is successful,
  * Install `xeyes` by running `sudo apt install x11-apps`.
  * Open terminal and run `xeyes`.
  * If the installation is good, you should see a pair of eyes pop up. If not refer to [this document](./WSL_and_VcSsrv.pdf) for troubleshooting. Kudos to Sam Dickinson who put togother this doc.


The editor might not be as needed as you think though, because handy
thing about this approach is that your windows filesystem is mapped in
linux.  For example `/mnt/c/Users/<YourName>/Desktop/` probably contains
your windows desktop.  This means if you clone your csse332 repo
somewhere on your windows filesystem, you can edit your code with your
favorite windows editor. You can also access your Linux folder by following
the instructions
[HERE](https://askubuntu.com/questions/759880/where-is-the-ubuntu-file-system-root-directory-in-windows-subsystem-for-linux-an)

# On a hard disk partition, or mSata drive

This option is mostly for students who have done this before or want
to use linux as a primary OS.  

If you want to do this but aren't sure how, talk to either:

-   Darryl Mouck, our sysadmin
-   the Rose Hulman linux users group (who often do an installfest in
    the first few days of the term for OS students)

If you do choose this approach, ensure you have:

-   gcc (the ubuntu package build-essential installs this)
-   git
-   a good text editor (emacs, vim, whatever you like)
-   valgrind

For the final project (this is the package list for ubuntu other
distros may have slightly different names for the same things:

-   qemu
-   bin86
-   nasm
-   bcc
-   hexedit
-   bochs
-   bochs-sdl

installed as well to do the assignments.

# Turning In

We will verify that you have this installed on the first lab day
(look at the schedule to see exactly when).  PLEASE do this beforehand and
do not plan to do it in lab - there will be too many people, it will
overload the wireless, and then you won't get help on your first
assignment.

