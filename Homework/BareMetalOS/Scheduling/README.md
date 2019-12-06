---
title: BmOS - Scheduling
layout: togit
---

# BmOs: Scheduling

<!-- markdown-toc start - Don't edit this section. Run M-x markdown-toc-refresh-toc -->
**Table of Contents**

- [Objective](#objective)
- [Multitasking](#multitasking)
- [Memory management](#memory-management)
- [Scheduling](#scheduling)
- [Loading and terminating](#loading-and-terminating)
- [Tools](#tools)
- [Timer Interrupt](#timer-interrupt)
- [Process Table](#process-table)
- [Load Program](#load-program)
    - [initializeProgram](#initializeprogram)
    - [terminate](#terminate)
    - [setKernelDataSegment](#setkerneldatasegment)
    - [enableInterrupts](#enableinterrupts)
- [Scheduling](#scheduling-1)
    - [Starting Slowly](#starting-slowly)
    - [Two warnings](#two-warnings)
    - [Testing](#testing)
- [Kill Process](#kill-process)
    - [Testing](#testing-1)
- [Process Blocking](#process-blocking)
- [Optional Features](#optional-features)
- [Reflection](#reflection)
- [Turning It In](#turning-it-in)

<!-- markdown-toc end -->


# Objective

In the previous milestones you wrote a single process operating system
where only one program could be executed at a time.  In this project
you will make it possible for up to eight programs to be executing
concurrently.

*You will also reflet on what you have done during the last five
weeks.*


# Multitasking

There are two basic requirements for multitasking.  First, you need to
have a preemptive scheduler that can interrupt a process and save its
state, find a new process, and start executing it.  Second, you need
to have memory management so that the processes do not get in the way
of each other.  Ideally, no process ever knows that it is interrupted
and every process thinks it has the whole computer to itself.


# Memory management

Nearly all modern operating systems use virtual memory, which
completely isolates one process from another.  Although all processors
since the 386 support virtual memory, you will not use it in this
project.  Instead, you will take advantage of an earlier method called
segmentation.

As you observed in previous projects, all addresses in real mode are
20 bits long, while all registers are 16 bits.  To address 20 bits,
the processor has six segment registers (of which only three are
really important): CS (code segment), SS (stack
segment), and DS (data segment).  All instruction fetches
implicitly use the CS register, stack operations (including
local variables) the SS register, and data operations (global
variables and strings) the DS register.  The actual address
used by any instruction consist of the 16 bit value the program thinks
it is using, plus the appropriate segment register times 0x10
(shifted left by 4).  For example, if your program states: JMP
  0x147, and the CS register contains 0x3000, then
the computer actually jumps to 0x3000 * 0x10 + 0x147, or
0x30147.

An interesting note is that 16-bit C programs (those compiled with
bcc) never touch the segment registers.  The pleasant thing
about this is if we set the registers beforehand ourselves, the same
program can run in two different areas of memory without knowing.  For
example, if we set the CS register to 0x2000 and ran
the above program, it would have jumped to 0x20147 instead of
0x30147.  If all of our segment register values are
0x1000 away from each other, the program's memory spaces will
never overlap.

The catch to all this is that programs must be limited to
64KB in size and never touch the segment registers
themselves.  They also must never crash and accidentally take out the
rest of memory.  That is why this approach has not been used in
practical operating systems since the 1980s.  However, 64KB
will be enough for our operating system.


# Scheduling

All x86 computers (and most others) come equipped with a
timer chip that can be programmed to produce interrupts periodically
every few milliseconds.  The interrupt produced by this timer is
interrupt 8.  If the timer is enabled, this means that the
program is interrupted and the interrupt 8 service routine is
run every few milliseconds.  Your scheduler should be called by the
interrupt 8 service routine.

The scheduler's task is to choose another process and start it
running.  Consequently it needs to know who all the active processes
are and where they are located in memory.  It does this using a
process table.

Every interrupt 8, the scheduler should back up all the
registers onto the process's own stack.  It should then save the stack
pointer (the key to where all this information is stored) in the
process's process table entry.  It should select a new process from
the table, restore its stack pointer, and restore its registers.  When
the timer returns from interrupt, this new process will start running.


# Loading and terminating

Creating a new process is a matter of finding a free segment for it,
putting it in the process table, and giving it a stack.  If a program,
such as the shell, wants to execute a program, it creates a new
process, copies the program into the new process's memory, and just
waits around until the timer preempts it.  Eventually the scheduler
will start the new process.  Terminating a program is done by removing
the program from the process table and busy waiting until the timer
goes off.  Since the program no longer has a process table entry, the
computer will never go back to it.

An interesting thing now is that a program (e.g., the shell) does not
have to terminate when it executes another program.  Instead it can
start the new process running and move on to another task.  The shell,
for example, can start another process running and not end.  This is
known as making a background process; the user can still use the shell
to do other things and the new program runs in the background.


# Tools

You will use the same utilities you used in the last milestone, and
you will also need to have completed the previous milestones
successfully.

This milestone directory includes a new bunch of files.  Copy your
existing BareMetalOs version from the Filesystem directory into a new
directory called "Scheduling" and then copy these new files over the
top.  You'll keep your milestone 1 & 2 submissions in their own
directory to make everybody's grading life easier.

# Timer Interrupt

Making a timer interrupt service routine is almost identical to the
interrupt 0x21 service routine.  The only difference is that
the service routine must back up all the registers and reinitialize
the timer.

Since nearly everything related to making this service routine
involves handling the registers, this step is mostly already done for
you in assembly.  You are provided with three new assembly functions.
The first, makeTimerInterrupt sets up the timer interrupt
vector and initializes the timer.  You need simply call it at the end
of main in kernel.c before launching the shell.  The
second assembly function is the interrupt 8 service routine,
which calls handleTimerInterrupt (which you must write) that
takes two arguments, the segment and the sp
(e.g. void handleTimerInterrupt(int segment, int sp)).  The
third is returnFromTimer which you will call at the end of
handleTimerInterrupt.

For now, your handleTimerInterrupt routine should call
printString to print out a message (such as "Tic")
and call returnFromTimer with the same two parameters that
were passed into handleTimerInterrupt.

Compile your operating system and run it.  If it is successful, you
will still be able to use the shell, but you will see the screen fill
up with "Tic"s.


# Process Table

Be sure to comment out the printString in
handleTimerInterrupt before proceeding.

A process table entry should store two pieces of information:

* Whether or not the process is active (stored as an int:
  1=active, 0=inactive)
* The process's stack pointer (stored as an int)


The process table itself should be a global array in
kernel.c.  Note that you do not have to store the segment,
since that is what you will use to index the table.  You should choose
to make an entry a struct.

Your table should contain eight entries.  Segment 0x2000
should index entry 0, and segment 0x9000 should index entry
7.  Thus, to find the correct table entry, divide the segment by
0x1000 and subtract 2.

To keep track of the current process, you should have a global integer
variable currentProcess, that points to the process table
entry currently executing.

Initialize the process table in main before calling
makeTimerInterrupt.  For each entry in the process table, set
active to 0 and set the stack pointer to 0xff00 (this is the
initial stack location for each segment).  Set currentProcess
to 0.

Now revise your interrupt 0x21 execute program
function.  Instead of launching a program at the segment given as a
parameter, it should launch programs in a free segment.  In
executeProgram,

1. Search through the process table for a free entry,
2. Set that entry to active, and
3. Call launchProgram on that entry's segment.

You should change executeProgram so that it takes only one
parameter, the name of the program to execute.  Make sure your
operating system compiles correctly.

# Load Program

## initializeProgram

launchProgram never returns to the program that called it.
This is a problem for multitasking -- you want the caller program to
be able to continue running while the new program runs.

You are provided with a new assembly function
initializeProgram which takes a segment as an argument
(e.g. void initializeProgram(int segment)).  This function
sets up a stack frame and registers for the new program, but does not
actually start running it.  Change your executeProgram
function to call initializeProgram instead of
launchProgram.

## terminate

Additionally, terminate needs to be modified.  Currently, it
reloads the shell.  However, in a multitasking OS the shell
never stops running.  Modify terminate so that it sets the
process that called it (i.e. currentProcess) to inactive and
starts an infinite loop.  Eventually, the timer will go off and the
scheduler will choose a new process.

## setKernelDataSegment

At this point, there is a problem with accessing the proper data
segment.  The process table is a global variable and is stored in the
kernel's segment.  However, when interrupt 0x21 is called,
the DS register still points to the caller's segment.  When
you try reading the process table in executeProgram and
terminate, you are actually accessing garbage in the calling
process's segment instead.

To solve this, you are provided with two more assembly functions:
setKernelDataSegment and restoreDataSegment.  You
must call setKernelDataSegment before accessing the process
table in both executeProgram and terminate.  In
executeProgram, you should then call
restoreDataSegment right after accessing the process table.

## enableInterrupts

By default, hardware boots and programs are started with interrupts
disabled.  This means that programs cannot currently be preempted.
You are provided with an enableInterrupts function in
lib.asm.  You should call that function at the very beginning
of shell and all other user programs.


# Scheduling

Now you should write the scheduler (handleTimerInterrupt) so
that it chooses a program using round robin.  The scheduler should
first save the stack pointer (sp) in the current process
table entry.  Then it should look through the process table starting
at the next entry after currentProcess and choose another process to
run (loop back to the beginning when you get to the end).  Finally, it
should set currentProcess to that entry, and call
returnFromTimer with that entry's segment and stack pointer.

## Starting Slowly

This particular step can be trixy.  Here's an alternative way to get
started:

1. have your kernel load 2 different programs into memory initially
   (likely shell and phello) before turning on the scheduler.
2. Initially write your scheduler with the assumption that it only
   needs to swap between process 0 and process 1 (and that both are
   always active).  This isn't a good assumption of course, but it
   lets you verify your other code without the more complex round
   robin stuff.
   
Then once you get that working, get the more complete version working.

## Two warnings

1. Always be sure that there is a valid target before you run the
   scheduler.  Don't start the interrupt handler until you've got a
   valid shell program fully loaded to switch to.  For similar
   reasons, always be sure setting a process as valid in the process
   table is the last thing you do...it's bad news to switch to a
   program that hasn't been fully copied into memory.
2. When the interrupt handler is run for the first time, the stack
   pointer will be pointing to a place on the kernel stack.  This
   stack pointer is not useful and should not be stored in your
   process table.  But if you're not careful, it's common to overwrite
   the stack pointer for processid 0 (usually the shell) the first
   time the handler is run because currentProcess has been incorrectly
   initialized to 0.  A better idea is to initialize the
   currentProcess to -1 and check for that in your handler.

## Testing

Compile your operating system and check that the shell loads
correctly.

The phello test program prints out "Hello World"
ten thousand times.  In the shell, "execute phello".  It
should start printing.  While it is printing, you should still be able
to issue shell commands: try typing "dir".


# Kill Process

You should create a new interrupt and shell command that terminates a
process (similar to the kill command in Unix).  A user should be able
to type "kill 3" in the shell and process 3 should be
forcibly terminated.

You will need to do three things:

1. Create a killProcess function in kernel.c,
2. Create a new interrupt 0x21 call to terminate a
  process, and
3. Add the command to the shell.

Killing a process is simply a matter of setting that process's process
table entry to inactive (0).  Once that is done, the process will
never be scheduled again.

Your interrupt 0x21 call should take the form:


Kill process
* AX = 9 (or the next available value for AX)
* BX = process id (from 0 to 7)


## Testing

From the shell, start phello executing.  Then quickly type
"kill 1".  If you immediately stop seeing "Hello
  World" and are able to type shell commands normally, then your
\textit{kill function works.


# Process Blocking

You will notice that all calls to execute run concurrently
with the shell.  It is often convenient for the shell to stop
executing temporarily while the new process is running, and then
resume when the new process terminates.  This is essential if the new
process wants keyboard input; otherwise it has to compete with the
shell.

This can be done by marking processes in the process table as
"waiting".  There should also be a field in the process
table entry stating what other process the process is waiting on.  A
process marked as waiting is never executed by the scheduler
but is also never overwritten.

When a process is killed or terminates, any processes waiting on it
should be set back to active.

You should make another execute interrupt 0x21 call that
causes the caller process to "wait."  Then make a new shell
command "execforeground filename" that causes the shell to
block until the program "filename" terminates.


# Optional Features

For extra credit credit (up to 50 additional points) you might want to
implement a few additional features in you operating system.  The more
advanced or complex the featues are, the more points you will receive.
Below is a partial list of possible optional features that you might
want to consider:


* Quit command,
* Print size as a number of sectors for dir command, and
* Option to change foreground and background colors.
* Alternative directory structures using inodes
* Alternative scheduling algorithms
* Fancy printing libraries

Of course, you can add other features that are not included in this
partial list.  In order to receive credit for your additoinal
features, you should describe them in your README file and
include instructions on how to use each.

# Team Member Feedback

There is a Moodle survey to submit feedback about your teammates.
Please fill it out.

# Turning It In

Be sure to submit your complete project to your team repository in the
the m5 folder.  Be sure to include a README that
explains 1) what you did, and 2) how to verify it.  Please include a
comment at the top of each file with the name of each team member and
your team number.  In addition, be sure to submit all other
deliverables that are highlighted on the project’s main page.

# Rubric

145 Points

| Part                                                                          | Point Value |
|:------------------------------------------------------------------------------|-------------|
| some evidence that timer interrupt works (if nothing else, does is print tic) | 10          |
| Can run phello from shell and do shell stuff at the same time                 | 40          |
| Can run at least 2 programs in the background + shell                         | 10          |
| kill works                                                                    | 25          |
| Execute foreground works                                                      | 25          |
| Foreground process can execute another foreground process and it works right  | 10          |
| Team Member Survey (complete on Moodle, individually)                         | 25          |

Extra credit too, just describe it in the readme and we'll evaluate it separately.

