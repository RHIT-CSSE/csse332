---
layout: post
title: Lab 13 Stack Smashing
readtime: true
date: Wed Feb 8 2023
---

# Introduction

In the lab, we will learn how to perform control hijacking attacks by exploiting
buffer overflows bugs. Control hijacking refers to an attack that attempts to
divert the control flow of a program away from normal execution to execute
arbitrary attack code (typically, dropping into a root shell). Each one of the
provided programs contains a security vulnerability that you must exploit to
hijack the control flow of the program and satisfy the requirements of each
part.

Note that you may do this lab in pairs if you like to.

# A Statement on Ethics and Responsibility

In this lab, you will be learning about and performing control flow hijacking
attacks on sample controlled piece of code.  Performing the same attacks outside
of the controlled environment that we provide you with is against the law and
Rose-Hulman policies, and may result in _fines, expulsions, and even jail time_.
__You SHALL not attack anyone else's machine without prior authorization and in
a controlled environment__.

# Learning Objectives

At the end of this lab, you should be able to:

- Define control flow hijacking attacks.
- Implement different types of exploits to cause unintended consequences when
  running vulnerable code.
- Identify defenses against control flow hijacking attacks, and then subvert
  them.
- Write better code and avoid buffer overflow bugs.

# Getting the Source Code

For this lab, you will be using the native Linux virtual machine (or baremetal
machine if you have one) and not the xv6 operation system. Please note that this
lab might behave slightly differently if you are running it on Windows or
MacOs; therefore, we highly recommend that you stick to using a Linux machine,
either natively or via WSL2.

To obtain the starting code for this lab, navigate to the top level directory of
your __csse332 class repository__ and `cd` into the `labs/lab13` directory as
follows:
```shell
$ cd /path/to/csse332/git/repository/
$ git pull
$ cd labs/lab13
```

# Installing Needed Software

To make things a bit simpler, we will run 32-bits version of the code that we
provide you with. To do so, you should install some software first to cross
compile 32 bit applications on a 64 bit machine. To do so, run

```shell
$ sudo apt install -y gdb gdb-multiarch gcc-multilib python2
```

Also, install `gef` on top of `gdb`, it will make your life a lot easier.

```shell
$ bash -c "$(curl -fsSL https://gef.blah.cat/sh)"
```

## A Note on Python

Our scripts in this lab run on `python2` (some day we will upgrade those to
`python3`, but not today). The commands above install `python2` on your machine.
You can use your favorite scripting language to write these solutions. For me,
`python2` was the most comfortable since it was easy to print weird characters
(we're going to need that a lot in this lab).

## A Note About WSL2

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

# Prelude

__PLEASE READ THE NOTE BELOW__, your code will not work if you do not do the
below two steps.

## Generating Your Cookie

Before you start this assignment, you must run the cookie generation script to
generate your own unique ID. We use this ID in our code to make sure that your
solution will only work for you, and for none of your classmates.

To generate your cookie, run the `setcookie.py` script and pass it your Rose
email ID (without the `@rose-hulman.edu`). For example, running the script for
myself would be:

```shell
$ ./setcookie.py noureddi
```

This will generate a file name `cookie` in your lab directory. You need that
file to be present at all times with solving this lab.

Please make sure that you use your usename as input to the script. That is what
we will be using when grading the lab. If you use the wrong value, all your
solution will break and you will receive no credit for any of the parts.

### Working with a Partner

If you are working in a team of 2, you can use the same script and pass it both
your usernames, separated by a space. For example, running the script for us is

```shell
$ ./setcookie.py noureddi hewner
```

## Disabling Kernel Protection

Every time you want to work on this lab, you must turn the Linux kernel's
virtual memory protection scheme (called Address Space Layout Randomization
(ASLR)). To do so, run the following script:

```shell
$ ./disable_aslr.sh
```

Once you are done with this lab, please reenable ASLR using the script:

```shell
$ ./enable_aslr.sh
```

# Part 1 🌶️

__Note that we did this part together in class, so you can simply just copy the
solution we did together. This is to reward you for attending class__.

In this part, we will start off easy by overwriting a variable on the stack.
Take a look at the source code in `part1.c`, compile it using `make` and run the
program. The program will wait for you to enter your name and then will print
the following:
```shell
$ ./part1
mohammad
Hi mohammad you are a wonderful person!.
```

Your job is to pass an input to this program such that it will change the 
output of the program. We will create our input using a `python` script which
we will then redirect to the standard input for this program. When successfully
completing this part, you should see something that looks like the following
```shell
$ python2 part1.py | ./part1
Hi mohammad you are pwnd!.
```
__Note__ that `mohammad` should be replaced with your own name!

Here are some suggested steps that you might take:

1. Examine `part1.c`, where is the buffer overflow?
2. Start `part1` in `gdb` and disassemble the `_main` function. Identify the
   function calls and their arguments and local variables.
3. Draw a picture of the stack, where is the variable that you will overflow and
   where is your target? How are they stored relative to each other?
4. How can your input affect the values of other variables in the code. Test
   your thought process by trying different inputs to the program.

## Submission

Submit a python program called `part1.py` that prints the line that you must
pass to your program to cause the overwriting of the intended variable. To test
your program, you can write
```shell
$ python2 part1.py | ./part1
```

_Hint_: To write a string that contains non-printable ASCII characters in
python, you can supply your code with the hexadecimal value of the byte you
intend to print. For example, to print byte `0xf6`, you can use `"\xf6"` as part
of your python string.

_Hint_: To repeat a character "X" `n` times in python, you can use `"X"*n`.

# Part 2 🌶️

__Note that we did this part together in class, so you can simply just copy the
solution we did together. This is to reward you for attending class__.

In this part, instead of overwriting a variable, we will overwrite the return
address of a function so that it goes and executes a piece of code that we
select (which the program originally was not intended to execute). Take a look
at `part2.c` and try it out:
```shell
$ ./part2
mohammad
Have a nice day =)
```

Your job is to overwrite the return address of a function that you must
identify, in order to cause the program to execute the function
`print_bad_outcome` instead of executing `print_good_outcome`. By successfully
completing this part, you should see something like the following
```shell
$ python2 part2.py | ./part2
You are pwnd!
```

Here's a suggested way to approach this problem
1. Examine `part2.c`, where is the buffer overflow?
2. Start `part2` in `gdb` and find the starting address of `print_bad_outcome`.
   To do so, you can use `(gdb) info address print_bad_outcome`. Record that
   address in your notes.
3. Set a breakpoint at the vulnerable function. To set a breakpoint at a
   specific machine address, you can use `(gdb) b *(0xDEADBEEF)` where
   `0xDEADBEEF` is the address you want to break at.
   1. Run the program using `run` to reach the breakpoint.
4. Disassemble the vulnerable function and draw its stack. Where is the
   vulnerable buffer stored? `gef` will prove to be very useful here.
6. What should the value of the return address of the vulnerable function be
   (i.e., where should this function return to)?
7. Examine the return address of the vulnerable function.
8. What should your input be to overwrite the return address?

## Submission

Submit a python program called `part2.py` that prints the line that you must
pass to your program to cause the overwrite of the return address. To test your
program, use
```shell
$ python2 part2.py | ./part2
```

__Hint__: When developing your solution, you might find it useful to examine the
hex content of your input string, to do so use:
```shell
python2 part2.py | hd
```

__Hint__: As mentioned in class, you might find the `struct` package in `python`
very useful.

# Part 3 🌶️

In this part, we will start the actual fun of redirecting the user's program to
execute a system call that opens a shell for us to use at we please. This
program takes its input as a command line argument, rather than through the
standard input.

To help you out, we have provided you with `shellcode.py` that contains
machine instructions that, when executed, will open a shell. Therefore, placing
this piece of code in memory and redirecting the execution to the start of these
instructions will cause the program to open a shell.

Your job is to exploit the `part3` binary to open a root shell when executed.
Here is an example output
```shell
$ sudo ./part3 $(python2 part3.py)
# whoami
root
#
```

Here is a suggested approach:
1. Examine `part3.c`, where is the buffer overflow?
2. Write a python program, called `part3.py` and make it print the shell as
   follows
   ```python
   from shellcode import shellcode
   print shellcode
   ```
3. Start up `gdb` with the program arguments as follows:
   ```shell
   $ gdb --args ./part3 $(python2 part3.py)
   ```
4. Set a breakpoint at the vulnerable function and start the program.
5. Disassemble the vulnerable function, where is the starting address of the
   buffer?
6. Identify the instruction that follows the call to `strcpy` and set a
   breakpoint there, then continue execution
   ```shell
   (gdb) b *(<address of instruction>)
   (gdb) c
   ```
7. Examine the content of the buffer, does it now contain the shell code? To do
   so, you can use the following gdb instruction `(gdb) x/32bx 0x<address>`.
8. Disassemble the shellcode using `(gdb) disassemble/r 0x<address>,+32`. What
   is it doing? Note the call to `int` and check what it is doing.
9. Modify `part3.py` so that the input overflows the buffer and causes the
   program to run the shellcode.

## Submission

Submit a python program called `part3.py` that prints the line that you must
pass as an argument to your program to create the exploit. To test your code,
use
```shell
$ sudo ./part3 $(python2 part3.py)
#
```
If you are successful, you should see a command prompt starting with the
character `#`. Typing `whoami` should print `root` as shown in the example
above.

# Part 4 🌶️🌶️

In this part, the developer has realized the errors of their ways, and decided
to use a safer function called `strncpy` (check the manpage for `strncpy` if you
are not sure what the difference between it and `strcpy` is). Therefore, the
exploit technique we used in the previous part no longer works.

Lucky for us, the developer miscalculated the size of the buffer. Hopefully this
will help you figure out another way to redirect the program's control flow to
execute our shell code.

## Submission

Create a python program, called `part4.py`, that prints the line you must pass
as an argument to your `part4` to cause the creation of a root shell. To test
your code, use the following:
```shell
$ sudo ./part4 $(python2 part4.py)
#
```

# Part 5 🌶️🌶️

In this part, the developers are really on our tail, they have enabled data
execution prevention (DEP) in the compiler so that no code from the stack can be
executed. What are we to do? Do we just give up? Never!

This part resembles part 3, except that now the program will not execute any
code from the stack, which renders our previous solution to part 3 obsolete.
You can still overflow a buffer and overwrite the return address, but you can't
execute any code on the stack or heap (so maybe there's another place to execute
from?).

Your task is the find a way to hijack the control flow of the program in a way
that causes it to start a bash shell. Check out the manpage for the `system()`
function call, and try to pass `/bin/sh` to `system` and see what happens.

## Submission

Create a python program, called `part5.py`, that prints the line you must pass
as an argument to your `part5` to cause the creation of a root shell. To test
your code and avoid issues with special characters, use the following:
```shell
$ sudo ./part5 $(python2 part5.py)
#
```

## Hint

Some binaries contains a lot of constant strings that you did not even create.
To find such strings, you can use the `strings` Unix utility (surprise!) as
follows:

```shell
$ strings part5
```

For example, if you want to find the string `/bin/sh` in your binary, you can
use:

```shell
$ strings part5 | grep /bin/sh
```

Furthermore, you can ask `strings` to print you the offset of the starting byte
of this string in memory as follows:

```shell
$ strings -t x part5 | grep /bin/sh
6d871 /bin/sh
```

This means that the start of my `/bin/sh` string in memory is `0x6d871` bytes
away from the start of my process's memory space. (Note that the offset in your
case might be different than mine).

To find the actual address of `/bin/sh` in memory, we will have to use `gdb`.
First load the program using `gdb --args part5 abc`.

Then, start the program using:
```
gef> start
```

After that, check the starting address of our memory address space:
```
gef> info proc mappings
Start Addr   End Addr       Size     Offset objfile
	 0x8048000  0x8049000     0x1000        0x0 ...
	 0x8049000  0x80b5000    0x6c000     0x1000 ...
	 0x80b5000  0x80e4000    0x2f000    0x6d000 ...
	 0x80e5000  0x80e7000     0x2000    0x9c000 ...
	 0x80e7000  0x80e9000     0x2000    0x9e000 ...
	 0x80e9000  0x810c000    0x23000        0x0 [heap]
	0xf7ff9000 0xf7ffc000     0x3000        0x0 [vvar]
	0xf7ffc000 0xf7ffe000     0x2000        0x0 [vdso]
	0xfffdd000 0xffffe000    0x21000        0x0 [stack]
```

You can see that the starting address in my case is `0x8048000`. Next, we can
add the offset we obtained above and find the address of `/bin/sh` as follows:

```
gef> x/x 0x8048000 + 0x6d871
0x80b5871:	0x6e69622f
```

To see the string in memory, you can change the print format as follows:

```
gef> x/s 0x8048000 + 0x6d871
0x80b5871:	"/bin/sh"
```


# Part 6 (Choose this or part 7 or part 8) 🌶️🌶️🌶️🌶️

This part uses the same code as the part 3, but it is compiled with DEP enabled.
Your job is to construct a return-oriented-programming (ROP) attack to open a
root shell.

__Hint__: You might find the tool `ROPgadget` useful. Here's a link to the
information: [ROPgadget](https://github.com/JonathanSalwan/ROPgadget).

__Hint__: You might find it useful to read through the paper "The Geometry of
Innocent Flesh on the Bone" listed in the resources section. Focus on sections
3.4 and 4 of the paper.

__Hint__: To disassemble a program outside of `gdb`, you can use
```shell
objdump -d ./part6 > part6.txt
```

## Submission

Create a python program, called `part6.py`, that prints the line you must pass
as an argument to your `part6` to cause the creation of a root shell. To test
your code, use the following:
```shell
$ sudo ./part6 $(python2 part6.py)
#
```

# Part 7 (Choose this or part 6 or part 8) 🌶️🌶️🌶️🌶️

This part implements a doubly-linked list on the heap. It accepts three
arguments from the command line. Study the code and its machine instructions to
figure out a way to exploit it and open a shell. You may need to modify the
`shellcode` that we provided you with slightly (maybe add an instruction or
two).

## Submission

Create a python program, called `part7.py`, that prints the line you must pass
as an argument to your `part7` to cause the creation of a root shell. To test
your code, use the following:
```shell
$ sudo ./part7 $(python2 part7.py)
#
```

# Part 8 (chose this or part 6 or part 7) 🌶️🌶️🌶️

At the start of this lab, we asked you to turn off ASLR to make your exploits
feasible. If enabled, ASLR will make exploiting a buffer overflow really hard
since the position of the stack is changed on each execution. To give you an
idea of what that looks like, we will simulate ASLR, but not really turn it on.

This part is very similar to part 3. However, the stack position is randomly
changed every time you run this code. Try it out in `gdb` and check out the
start of the `vulnerable_fn`'s stack, it should change on every run. The caveat
is, unlike ASLR, we are only modifying the stack position by a __bounded__
random offset.

Your job is to write an exploit that can open a root shell every time you run
this program, despite the presence of our little randomization.

## Submission

Create a python program, called `part8.py`, that prints the line you must pass
as an argument to your `part8` to cause the creation of a root shell. To test
your code, use the following:
```shell
$ sudo ./part8 $(python2 part8.py)
#
```

Note that you will loose some points if your exploit generates error messages.
We strive to have clean exploits that are not easily detected.

## Some Hints

In x86, the NOP instruction (opcode `0x90`) can be used to simply advance the
program counter without really doing anything else. You might find the NOP
instructions very useful in this part. Maybe _spraying_ some NOPs here and there
would be a useful idea?

# Submission

Submit all of your python (or any other scripting language) scripts to
[Gradescope]({{ site.gradescope_url }}). Do not submit your `cookie` file or any
of the binaries.

# Rubric

| Part                       | Points |
| ----------------           | ------ |
| Part 1                     | 10     |
| Part 2                     | 10     |
| Part 3                     | 15     |
| Part 4                     | 20     |
| Part 5                     | 20     |
| Part 6 or Part 7 or Part 8 | 25     |
