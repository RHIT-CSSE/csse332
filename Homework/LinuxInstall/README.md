
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

To run graphical programs you will need to have an windows X-server.  I use this one:

https://sourceforge.net/projects/vcxsrv/

Note that before you can actually run a graphical program you'll have
to start the server and set the display variable on the command line.
This is how to set it:

     export DISPLAY=localhost:0.0

This will only be needed if you are actually using graphical linux
applications.  We'll need that at the end of the term for the BMOS
labs, and you might need it right away if you want to run a graphical
(i.e. not text based) linux editor.

The editor might not be as needed as you think though, because handy
thing about this approach is that your windows filesystem is mapped in
linux.  For example /mnt/c/Users/YourName/Desktop/ probably contains
your windows desktop.  This means if you clone your csse332 repo
somewhere on your windows filesystem, you can edit your code with your
favorite windows editor.

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

