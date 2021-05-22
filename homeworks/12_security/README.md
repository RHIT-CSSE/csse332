---
title: Advanced Security Lab
layout: post
date: Sunday May 16
readtime: true
gh-repo: rhit-csse332/csse332-202130
gh-badge: [star,watch,follow]
---

# Table of contents

<!-- vim-markdown-toc GFM -->

* [A statement on ethics and responsibility](#a-statement-on-ethics-and-responsibility)
* [Control flow hijacking](#control-flow-hijacking)
  * [Learning objectives](#learning-objectives)
  * [Logistics](#logistics)
* [Prelude](#prelude)
* [Part 1 (\*)](#part-1-)
  * [Submission](#submission)
* [Part 2 (\*)](#part-2-)
  * [Submission](#submission-1)
* [Part 3 (\*\*)](#part-3-)
  * [Submission](#submission-2)
* [Part 4 (\*\*\*)](#part-4-)
  * [Submission](#submission-3)
* [Part 5 (\*\*\*\*)](#part-5-)
  * [Submission](#submission-4)
* [Part 6 (\*\*\*\*\*)](#part-6-)
  * [Submission](#submission-5)
* [Part 7 (\*\*\*\*\*)](#part-7-)
  * [Submission](#submission-6)
* [Rubric](#rubric)

<!-- vim-markdown-toc -->

# A statement on ethics and responsibility

In this lab, you will be learning and performing control hijacking attacks on a
sample controlled piece of code. Performing the same attacks outside of the
controlled environment that we provide you with is against the law and
Rose-Hulman policies, and may result in _fines, expulsions, and even jail time_.
__You SHALL not attack anyone else's machine without prior authorization and in
a controlled environment__.

# Control flow hijacking

In the lab, we will learn how to perform control hijacking attacks by exploiting
buffer overflows bugs. Control hijacking refers to an attack that attempts to
divert the control flow of a program away from normal execution to execute
arbitrary attack code (typically, dropping into a root shell). Each one of the
provided programs contains a security vulnerability that you must exploit to
hijack the control flow of the program and satisfy the requirements of each
part.

## Learning objectives

In this lab, we will attempt to answer the following questions:
- What is a control flow hijacking attack?
- How bad can buffer overflows be?
- How does a process organize its memory when calling functions?
- How do we examine and debug the machine code of a certain program?
- How can we bypass some of the defense mechanisms employed by the compiler and
  the operating system to stop control flow hijacking attacks?

After this lab, you should be able to understand and appreciate the severity of
buffer overflows, and hopefully write better code that prevents such
vulnerabilities.

## Logistics

- [Source code]({{ site.gh_homeworks_url }}/12_security)
- [Submission box](https://moodle.rose-hulman.edu/course/view.php?id=81937)
- [Submission instructions](https://rhit-csse332.github.io/csse332-202130/docs/submission_instructions/)
- [`gdb` reference card](https://users.ece.utexas.edu/~adnan/gdb-refcard.pdf)
- [Smashing the stack for fun and profit](https://inst.eecs.berkeley.edu/~cs161/fa08/papers/stack_smashing.pdf)
- [The geometry of innocent flesh on the bone](https://hovav.net/ucsd/dist/geometry.pdf)

The virtual machine is loaded with python 2.7, please do not upgrade python as
python 3 will give you some pain when trying to write non-printable ascii
characters (which we will need to do).

# Prelude

Before you get started (and every time you reboot your machine), you will need to run the script
`disable_aslr.sh` that is provided for you in the repository. To do simply, simply type
```shell
$ ./disable_aslr.sh
```

# Part 1 (\*)

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
$ python part1.py | ./part1
Hi mohammad you are pwnd!.
```
__Note__ that `mohammad` should be replaced with your own name!

Here are some suggested steps that you might take:
1. Examine `part1.c`, where is the buffer overflow?
2. Start `part1` in `gdb` and disassemble the `_main` function. Identify the
   function calls and their arguments and local variables.
3. Draw a picture of the stack, where is the variable that you will overflow 
   and where is your target? How are they stored relative to each other?
4. How can your input affect the values of other variables in the code. Test
   your thought process by trying different inputs to the program.

To disassemble a function in `gdb`, you can use `disassemble _main`, here is my
output
```shell
(gdb) disassemble _main
Dump of assembler code for function _main:
   0x08049b05 <+0>:	push   %ebp
   0x08049b06 <+1>:	mov    %esp,%ebp
   0x08049b08 <+3>:	push   %ebx
   0x08049b09 <+4>:	sub    $0x24,%esp
   0x08049b0c <+7>:	call   0x80499e0 <__x86.get_pc_thunk.bx>
   0x08049b11 <+12>:	add    $0x914ef,%ebx
   0x08049b17 <+18>:	movl   $0x6f772061,-0x1c(%ebp)
   0x08049b1e <+25>:	movl   $0x7265646e,-0x18(%ebp)
   0x08049b25 <+32>:	movl   $0x206c7566,-0x14(%ebp)
   0x08049b2c <+39>:	movl   $0x73726570,-0x10(%ebp)
   0x08049b33 <+46>:	movl   $0x216e6f,-0xc(%ebp)
   0x08049b3a <+53>:	sub    $0xc,%esp
   0x08049b3d <+56>:	lea    -0x26(%ebp),%eax
   0x08049b40 <+59>:	push   %eax
   0x08049b41 <+60>:	call   0x80511a0 <gets>
   0x08049b46 <+65>:	add    $0x10,%esp
   0x08049b49 <+68>:	sub    $0x4,%esp
   0x08049b4c <+71>:	lea    -0x1c(%ebp),%eax
   0x08049b4f <+74>:	push   %eax
   0x08049b50 <+75>:	lea    -0x26(%ebp),%eax
   0x08049b53 <+78>:	push   %eax
   0x08049b54 <+79>:	lea    -0x2dff8(%ebx),%eax
   0x08049b5a <+85>:	push   %eax
   0x08049b5b <+86>:	call   0x80509f0 <printf>
   0x08049b60 <+91>:	add    $0x10,%esp
   0x08049b63 <+94>:	sub    $0xc,%esp
   0x08049b66 <+97>:	push   $0x0
   0x08049b68 <+99>:	call   0x804ff20 <exit>
End of assembler dump.
```

## Submission

Submit a python program called `part1.py` that prints the line that you must
pass to your program to cause the overwriting of the intended variable. To test
your program, you can write
```shell
$ python part1.py | ./part1
```

_Hint_: To write a string that contains non-printable ASCII characters in
python, you can supply your code with the hexadecimal value of the byte you
intend to print. For example, to print byte `0xf6`, you can use `"\xf6"` as part
of your python string.

_Hint_: To repeat a character "X" `n` times in python, you can use `"X"*n`.

# Part 2 (\*)

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
$ python part2.py | ./part2
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
   vulnerable buffer stored with respect to the `%ebp` register (which contains
   the frame pointer). Where is the return address stored?
5. Examine the `%esp` and `%ebp` registers using `(gdb) info reg`
6. What should the value of the return address of the vulnerable function be
   (i.e., where should this function return to).
7. Examine the return address of the vulnerable function. To examine two words
   of memory at `%ebp` use `(gdb) x/2wx $ebp`.
8. What should your input be to overwrite the return address?

## Submission

Submit a python program called `part2.py` that prints the line that you must
pass to your program to cause the overwrite of the return address. To test your
program, use
```shell
$ python part2.py | ./part2
```

__Hint__: When developing your solution, you might find it useful to examine the
hex content of your input string, to do so use:
```shell
python part2.py | hd
```

__Hint__: Remember that the x86 architecture is little endian. You can use
Python's `struct` modules to print little-endian values, for example:
```python
from struct import pack

print pack("<I", 0xDEADBEEF)
```

# Part 3 (\*\*)

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
$ sudo ./part3 $(python part3.py)
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
   $ gdb --args ./part3 $(python part3.py)
   ```
4. Set a breakpoint at the vulnerable function and start the program.
5. Disassemble the vulnerable function, where is the starting address of the
   buffer relative to `%ebp`.
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
$ sudo ./part3 $(python part3.py)
```
If you are successful, you should see a command prompt starting with the
character `#`. Typing `whoami` should print `root` as shown in the example
above.

__Hint__: Use gdb to test your solution and place breakpoints at different
locations to examine the contents of the stack.

# Part 4 (\*\*\*)

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
$ sudo ./part4 $(python part4.py)
```

# Part 5 (\*\*\*\*)

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
$ python part5.py > a.txt
$ sudo ./part5 $(cat a.txt)
```

---
__Chose one of the two problems below to submit, or submit both for fun and
extra credit__

# Part 6 (\*\*\*\*\*)

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
$ sudo ./part6 $(python part6.py)
```


# Part 7 (\*\*\*\*\*)

This part implements a doubly-linked list on the heap. It accepts three
arguments from the command line. Study the code and its machine instructions to
figure out a way to exploit it and open a shell. You may need to modify the
`shellcode` that we provided you with slightly. 

## Submission

Create a python program, called `part7.py`, that prints the line you must pass
as an argument to your `part6` to cause the creation of a root shell. To test
your code, use the following:
```shell
$ sudo ./part7 $(python part7.py)
```

# Rubric

| Part                                                                      | Points |
|---------------------------------------------------------------------------|--------|
| Part 1           | 5  |
| Part 2           | 5  |
| Part 3           | 5  |
| Part 4           | 15 |
| Part 5           | 30 |
| Part 6 or Part 7 | 40 |

