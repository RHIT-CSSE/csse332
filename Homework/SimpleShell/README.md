# Simple Shell

# Introduction

This code represents a simple shell.  The shell is the program that
allows you to run commands on the command line (i.e. the thing you
type 'cd' and 'gcc blah blah blah' into).  The most famous shell is
called bash - that's probably the one you're using right now.  Just
like preferred text editors, unix people tend to have a preferred
shell (alternatives include zsh, csh).

# What we've given you

So the current shell does nothing except parse input commands into two
chunks - command and argument.  Actual command parsing in a shell is
pretty complex, but in our simple shell we're going to keep it
insanely simple.  The first thing the user types is the executable
they'd like to run, then (optionally) a space, then anything that
follows that is a single input argument, until the newline.  This
parsing is already written for you and should work out something like
this:

    SHELL% emacs somefile.txt
    Command is 'emacs' with argument 'somefile.txt'
    SHELL% emacs
    Command is 'emacs' with no arguments
    SHELL% ^C

Note it will not actually run emacs - that will be part of your job.

# Building

We've provided a Makefile for this project, so they only thing you
should need to do to build is type

    $ make
    
to build both simpleshell and donothing.  If you make changes, typing
make should rebuild just what is needed.

We'll discuss how this works in the last step.

# Step 1: Execute foreground commands

Modify the given simpleshell code so it actually runs the commands.
You should use one of the variants of exec we discussed in class (I
think execlp is easiest in this case but either is fine).  Do NOT use
system.

Calling exec will replace your existing process, but we want our shell
to continue after the called program is run.  To make that work you'll
have to call fork and wait.

Your code should work correctly when passed zero or one input
argument.  To help you, the given program donothing is very explicit
about what arguments it was called with.

    $ ./simpleshell 
    SHELL% ./donothing
    Command is './donothing' with no arguments
    Do nothing program started!
    EXECUTABLE './donothing'
    Pausing for 5 seconds...
    Do nothing program finished!
    SHELL% ./donothing foobar
    Command is './donothing' with argument 'foobar'
    Do nothing program started!
    EXECUTABLE './donothing'
    ARGUMENT 1: foobar
    Pausing for 5 seconds...
    Do nothing program finished!
    SHELL% ^C

# Step 2: Basic background commands

We want to be able to run programs in the background (i.e. start up
one program while another keeps running).  Using fork, modify the code
so if you prefix a command with \`\`BG'' it runs that command in the
background.  That is, if you type:

    BGemacs 

it will open an emacs window and return to the prompt before
emacs finishes running.  You should be able to run as many background
commands as you like.

Example output:

    ./simpleshell
    SHELL% BG./donothing
    Command is 'BG./donothing' with no arguments
    SHELL% Do nothing program started! <=== NOTE: new shell prompt prints out fast but that's not bad
    EXECUTABLE './donothing'
    Pausing for 5 seconds...
    BG./donothing <=== NOTE: this is me running a 2nd donothing in the background
    Command is 'BG./donothing' with no arguments
    SHELL% Do nothing program started!
    EXECUTABLE './donothing'
    Pausing for 5 seconds...
    Do nothing program finished!
    Do nothing program finished!
    ^C


# Step 3: Background commands with finish notification

Modify the BG feature so that when a background command finishes
running, the shell prints out "Background command finished".  

This could be implemented with signals, but I'd like you to do it with
fork/wait.  The shell will start a parent process which will start a child process for executing and then wait for the child to finish and print "Background command finished".

    ./simpleshell
    SHELL% BG./donothing
    Command is 'BG./donothing' with no arguments
    SHELL% Do nothing program started!
    EXECUTABLE './donothing'
    Pausing for 5 seconds...
    BG./donothing
    Command is 'BG./donothing' with no arguments
    SHELL% Do nothing program started!
    EXECUTABLE './donothing'
    Pausing for 5 seconds...
    Do nothing program finished!
    Background command finished
    Do nothing program finished!
    Background command finished


# Step 4: Zombies

Run your simpleshell, start a program (e.g emacs or gedit) in the
background from your simple shell, and then NOT IN YOUR SIMPLE SHELL but in some other terminal
run "ps -a".  How many instances of the program are running?  Next,
exit the background program, but do not exit simpleshell, and NOT IN
YOUR SIMPLE SHELL run "ps -a" again.  You should see something like
"simpleshell <defunct>".  Defunct processes are zombie processes
as described in class and on page 121 of your textbook.  [How-to
Geek](https://www.howtogeek.com/119815/htg-explains-what-is-a-zombie-process-on-linux/)
has a nice article explaining what zombie processes are. and how to
deal with them.

NOTE: if your processes aren't appearing defunct, it may be a sign of
a different bug (see "common problems" below).

To fix this, we need to wait on all our processes that are finishing.
BUT if we wait too much, we'll end up stalled, waiting on a process
that hasn't finished yet.  Solution?  The SIGCHILD signal.  This
signal is sent when a child process finishes.

Add a signal handler for the SIGCHILD signal that calls wait.  This
will prevent zombies from occurring.

This solution also introduces a bug - now if you execute a background
process, then a foreground process, you'll cause a double wait that
will stall your shell until your background process completes.  There
are more sophisticated solutions that will solve this problem, but
they're beyond the scope of this assignment - we'll leave it at that
for now.

# Step 5: Makefiles

We'll talk about makefiles in class one day, so if that hasn't
happened yet you might consider waiting.  But if not, all the
materials plus an activity are in the make\_tutorial directory.

Once you've gone through that stuff you should be able to look at the
Makefile we provided and understand it.

Now we'd like you to build a makefile for a more complicated project.
Go into the make\_problem directory and follow the instructions in
that Makefile.

# Conclusion

Submit your simpleshell.c and the Make problem makefile in the usual way.

# Common problems

## My simpleshell processes are not appearing defunct. Why?

It's usually the same issue - your child that prints "Background
process finished" is not actually exiting.  Instead, it's going back
into the command processing while(1) loop that it inherited when it
was forked from its parent.  It won't actually process any commands of
course because it's not the parent, but it also won't end which means
it can't be a zombie (you must die first to become a zombie right?).

To fix this be sure your child exits after printing background process
finished.

# Rubric

|                         | Points | Notes                                                                                |
|:------------------------|--------|--------------------------------------------------------------------------------------|
| S1: Foreground Commands | 15     | check arguments with donothing<br>should use exec<br>should not quit after first run |
| S2: Background Commands | 15     |                                                                                      |
| S3: Finish notification | 10     | check for use of exec and proper exit of printing process (e.g. common probs)        |
| S4: Zombies             | 15     | be sure your run ps outside of simple shell                                          |
| S5: Makefile            | 15     | make sure it builds .o files with their own rules                                    |
