---
layout: post
title: WSL2 Installation
readtime: true
date: Thursday 29 Feb 2024
---

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

# 1. Initial WSL2 installation

**CSSE majors**: you should already have WSL2 installed from taking CSSE132. If
not, follow the instructions here: [install
WSL2.](https://learn.microsoft.com/en-us/windows/wsl/install)

**CPE majors**: you probably have WSL1 installed, which is the wrong version.
You need to [upgrade your WSL1 to
WSL2](https://learn.microsoft.com/en-us/windows/wsl/install#upgrade-version-from-wsl-1-to-wsl-2).
If you do not have WSL installed at all, follow the above link for the CSSE
majors.

## Issues with these instructions? `Catastrophic failure`?
If you run into any issues during the installation process, please make a post
on the [CampusWire forum]({{ site.campuswire_url }}) detailing the errors you
faced along with the version of wsl2 that you can trying to install. One of
your instructors (or fellow classmates) will reply to help you out as soon as
possible.

# 2. Installing needed Linux software

1. In Windows, start the **Ubuntu** app.
2. In the Ubuntu shell that appears, run the following two commands:

```shell
  sudo apt update
```

```shell
  sudo apt install -y build-essential git \
        gdb \
        python3 \
        python-is-python3 \
        ipython3 \
        gdb-multiarch \
        qemu-system-misc \
        gcc-riscv64-linux-gnu \
        binutils-riscv64-linux-gnu
```

# 3. Open the current directory in Windows Explorer

Run the following command to verify that you can access your Linux files from Windows:

```shell
explorer.exe .
```

A new File Explorer window should show up in Windows that contains the content of your Ubuntu work. You can then drag those files into the GradeScope dropbox as normal. Run this command from Ubuntu whenever you want to turn in your homework to GradeScope.

# 4. Next steps

Return to the course home page by clicking CSSE332 in the top left. Complete the rest of the setup.

<!--
% CHANGELOG
%
% Fri 24 Nov 2023   Removed mentioning vscode as we won't be using it!
%                   Updated list of dependencies to now include everything
%                    needed to build xv6. 
%                   Added table of contents.
%
-->
