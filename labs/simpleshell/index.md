---
layout: post
title: Lab 02 -- Simple Shell
readtime: true
date: Thu Nov 30, 2023
---

# Introduction

In this lab, we will build a simple shell. The shell is the program that allows
you to run commands on the command line (i.e., it's the thing where you type
`cd` and `gcc …` into). The most famous shell is called `bash`, though many
variants exists, including `fish`, `zsh` and others. Just like preferred text
editors, Unix people tend to have a preferred shell. To check which shell you
are running, from your Linux terminal, use the following command:
```shell
$ echo $SHELL
/bin/zsh
```
In my case, I am running the `zsh` shell. Feel free to experiment and find your
favorite shell. A lot of customization can make your shell feel like your home. 


## Learning Objectives

At the end of this lab assignment, you should be able to:
- Implement a simple shell that can parse commands and execute them. 
- Use `fork`, `exec`, and `wait` to launch foreground commands.
- Use `fork`, `exec`, and `wait` to launch background commands. 
- Eliminate zombie children using the `SIGCHLD` signal.


# Getting the Source Code

We will do this lab in the `main` branch of your labs repository. To make sure
you are on the right branch, check it out using:

  ```sh
  $ git branch
  ```
The branch you are currently on will be highlighted for you (with a \* next to
its name).

If you are working on the `main` or `master` branch, then follow these
instructions:

  ```sh
  $ git fetch upstream
  $ git pull upstream main
  ```
At this stage, you should have the latest copy of the code, and you are good to
get started. The starter code is contained under the `simpleshell/` directory.

If you are currently on a different branch (say you are still on `clab_solution`
from the last lab), then we need to switch to `main` or `master` (depending on
your default's name).

First, add, commit, and push your changes to the `clab_solution` to make sure
you do not lose any progress you did on the last lab. To check the status of
your current branch, you can use:
  ```sh
  $ git status
  ```
This will show you all the files you have modified and have not yet committed
and pushed. Make sure you `add` those files, then `commit` your changes, and
`push` them.

If `git push` complains about not knowing where to push, you'd want to push the
current branch you are on. So for example, if I am working on `clab_solution`,
then I'd want to do `git push origin clab_solution`.

Now, you are ready to swap back into `main` (or `master`).

```sh
$ git checkout main
```

Then, grab the latest changes using:

```sh
$ git fetch upstream
$ git pull upstream main
```

At this stage, you should have the latest copy of the code, and you are good to
get started. The starter code is contained under the `simpleshell/` directory.


# Starter Code

To make your life easier, we have given you a sample starter code that will
parse input commands into two chunks: the command and an argument (if any).
Actual command parsing in a shell is pretty complex, but the sake of our sanity,
we will keep things insanely simple. If you'd like to check out more complex
parsing libraries, feel free to check out the [libreadline
library](https://tiswww.case.edu/php/chet/readline/rltop.html).

For our purposes, here's a simple breakdown of how things happen in our parsing
scheme. Our shell, let's call it Rose shell, or `rhsh`, prompts the user for a
command that is no longer than 82 characters. We then parse this command into
two strings stored in an array of character pointers, thus the following two
lines:
```c
/* the full command entered by the user. */
char command[82]; 
/* the array of two string representing the parsed command. */
char *parsed_command[2];
```

Two cases might arise when a command is parsed:
1. The user input a command without an argument, in that case
   `parsed_command[0]` contains the command the user wants to execute, and
   `parsed_command[1]` is `NULL`. 
2. The user input a command with an argument, in that case `parsed_command[0]`
   contains the command the user wants to execute, and `parsed_command[1]`
   contains the string representation of the argument the user wants to pass to
   that command. 

Note that our simple parser does not handle edge cases or malformed commands, so
we will make the nice assumption that our users are all well-behaved. Now, take
a look at the `simple_shell.c` file and make sure you understand how parsing the
commands work. Ask any questions you might have.

## The `donothing.c` Program

In addition to the simple parser, we also provided you with the `donothing.c`
program. As its name suggests, this program simply reads its arguments, sleeps
for 5 seconds, and then exits, pretty much doing nothing. Give this code a read,
and make sure to ask any questions you might have. 

# Compiling the Code

To make the building and compilation process simpler for you, we have provided
you with a `Makefile` that automates the build process. To build the lab, simply
issue the file command from your Linux terminal:
```shell
$ make
gcc -c simpleshell.c
gcc simpleshell.o -o simpleshell
gcc -c donothing.c
gcc donothing.o -o donothing
```
If the build is successful, you should see two executables show up,
`simpleshell` and `donothing`. Any time you make changes to your code, simply
issue `make` again to recompile only the necessary files and reproduce the
needed binaries. 

# Step 1: Executing Foreground Commands

The first thing we would like our shell to do is to actually execute the command
that the user passes to it. To do so, modify the code to use one of the variants
of the `exec` system call we discussed in class. We recommend that you use
`execlp` but feel free to use whichever variant works best for you.

Recall that calling `exec` will replace the existing process with another one.
__However__, we would like our shell to still be running after the called
program is executed. In other words, if the user issues the command
`./donothing`, then the shell must execute `./donothing` but it should not
replace the shell itself; `./donothing` should execute in a separate process.
Therefore, to achieve this desired behavior, you will need to use the `fork` and
`wait` calls. 

Your code should work correctly when passed zero or one input argument. To help
you out, the `donothing` program will print out information about its arguments.
Here is an example of running the shell on my terminal:
```shell
$ ./simpleshell
RHSH% ./donothing
Command is './donothing' with no arguments
Do nothing program started!
EXECUTABLE './donothing'
Pausing for 5 seconds...
Do nothing program finished!
RHSH% ./donothing foobar
Command is './donothing' with argument 'foobar'
Do nothing program started!
EXECUTABLE './donothing'
ARGUMENT 1: foobar
Pausing for 5 seconds...
Do nothing program finished!
RHSH% ^C
```
Note that to exit `RHSH`, you should hit `^C` (or ctrl-c), which will kill the
main `simpleshell` process.

# Step 2: Basic Background Commands

In step 1, you will notice that when `rhsh` executes a command, the main shell
will wait for the command to finish up before accepting new commands (thus the
term __foreground__). In this step, we would like to provide support for
__background___ commands, i.e., commands that run without blocking the simple
shell.

Using a combination of `fork`, `exec`, and `wait`, modify your code such that if
the user's command starts with the `BG` prefix, then the shell will run the
command in the background. In other words, if the user types
```shell
RHSH% BGemacs
```
The shell will open an `emacs` window and return to the prompt (i.e., the
`RHSH%` prompt) before `emacs` finishes running. You should be able to run as
many background commands as you like. 

Here is an example output from my machine:
```shell
$ ./simpleshell
RHSH% BG./donothing
Command is 'BG./donothing' with no arguments
RHSH% Do nothing program started! <=== NOTE: new shell prompt prints out fast but that's not bad
EXECUTABLE './donothing'
Pausing for 5 seconds...
BG./donothing <=== NOTE: this is me running a 2nd donothing in the background
Command is 'BG./donothing' with no arguments
RHSH% Do nothing program started!
EXECUTABLE './donothing'
Pausing for 5 seconds...
Do nothing program finished!
Do nothing program finished!
^C
```
Note that your printouts might show in a different order, that is okay. We will
understand this phenomenon more when we talk about scheduling and concurrency.

At this stage of our implementation, we will be generating zombie processes.
Let's not worry about them now, we will take care of them in Step 4. 

# Step 3: Background Commands with Finish Notification

When a background command finishes, we would like our shell to let us know that
the command is done by printing `Background command finished`. Augment your code
from Step 2 and use `fork` and `wait` to detect when a background command has
finished execution. Note that you are not allowed to change the source code in
the `donothing.c` file; all of your code changes must happen in `simpleshell.c`. 
_Hint_: You might need more than one `fork`.

Here's an example output from running a background command in my shell:
```shell
$ ./simpleshell
RHSH% BG./donothing
Command is 'BG./donothing' with no arguments
RHSH% Do nothing program started!
EXECUTABLE './donothing'
Pausing for 5 seconds...
BG./donothing
Command is 'BG./donothing' with no arguments
RHSH% Do nothing program started!
EXECUTABLE './donothing'
Pausing for 5 seconds...
Do nothing program finished!
Background command finished
Do nothing program finished!
Background command finished
```

# Step 4: Zombies

Now let's do a small experiment. You will need two terminal windows for this
step. You can either open two separate terminal windows or check out
[tmux](https://github.com/tmux/tmux) for ways to split your terminal and have
some more customization. 

Start your `simpleshell` and run a background command (e.g., `BGemacs` or any
other background command). Then, from the __other__ terminal window, issue the
command
```shell
$ ps -a
```
This should show you the list of processes that are running in your current
Linux shell (not in `simpleshell`, but in your Linux shell). Next, kill or exit
the background process (e.g., the `emacs` process you started from
`simpleshell`) __but do not exit `simpleshell`__, and from your other Linux
terminal, run `ps -a` again. If your implementation is correct, you should see
something like `<simpleshell defunct>`, which indicates that you have a zombie
process.

Your goal in this step is to get rid of these zombie processes. To achieve that,
we will need to `wait` on __all__ our processes that are finishing. However, if
we wait too much, we'll end up stalling our `simpleshell`, which goes against
the whole point of having background processes. 

In this lab, we will make use of the `SIGCHLD` signal. When a process exits
(using the `exit` call or by `return` from `main`), it will send its parent the
`SIGCHLD` signal. In this lab, we will catch the `SIGCHLD` signal from the
parent and then execute the `wait` call on the finished child, preventing it
from becoming a zombie. 

To achieve this, in your code, before starting your shell, register a signal
handler for `SIGCHLD` with the operating system using:
```c
signal(SIGCHL, handle_sigchld);
```
where the function `handle_sigchld` is the function that the parent will execute
when the `SIGCHLD` signal is received from the child.
```c
void handle_sigchld(int ignored)
{
  /* TODO: Insert your code here! */
}
```

Once you wait for all the children, defunct processes should no longer appear in
your process list. 

## Small Caveat

The above solution introduces a different bug. If you execute a background
command followed by a foreground command, this will cause a double `wait` which
will stall the shell until the background command completes.  There are more
sophisticated solutions that will solve this problem, but they're beyond the
scope of this assignment - we'll leave it at that for now.

# Submitting your code

There are no special submission requirements for this lab, you only have to
submit the `simpleshell.c` file. __Please do not submit any other files than
`simpleshell.c`__. 

## Submission Checklist

- [ ]  My code compiles and generates the right executables.
- [ ]  I submitted `simpleshell.c` to [Gradescope]({{ site.gradescope_url }}).

## Grading

Check out this assignment's [grading](checklist/) page for more information.
