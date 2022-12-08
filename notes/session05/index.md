---
layout: post
title: Signals
readtime: true
date: Tue Dec  6 07:38:55 2022 
---

# Learning Objectives

At the end of this lecture, you should be able to:
- Define inter-process-communication (IPC) and some of its mechanisms.
- Implement signal sending, receiving, and handling in a Linux environment.

---

# Topics

In this lecture, we will cover the following topics:
- Inter Process Communication.
- Signals.

---

# Notes


<!-- vim-markdown-toc GFM -->

* [Definition](#definition)
* [The life of a signal](#the-life-of-a-signal)
* [Signal Delivery](#signal-delivery)

<!-- vim-markdown-toc -->


# Definition

- Consider an example where you are working at your desk, and you get a text
  message from your crush or your significant other, what would you do at that
  point in time?
- What did your partner do?
  - Communicate with you by sending a signal
- Your current work will be **interrupted**, you go to handle the corresponding
  signal, and then you return to where you were in the studying
- What if the text message was from your mom or dad? What would be your reaction
  then? probably ignore right?!
- Now in terms of the processes that are running, they can communicate with each
  other with the use of **signals**
- A signal is a short message sent to a particular task, process, or group of
  processes
  - Signals allow for **IPC**: Inter Process Communication
- In most circumstances, only the signal number is delivered the signalled task,
  and no further information
  - So you cannot know who sent you the signal, only that a signal has been
    delivered to you
  - There are other ways to tell the a task more information, using the
  `siginfo` method to deliver more information, but we won't worry about that
  one for now.
  - A signal serves two roles:
    1. **Notification**: A task is notified that a signal has been sent to it
    2. **Handling action**: A task is caused to break current execution to
       handle the signal, but it can customize the way it can handle signals,
       including blocking or ignoring them.
       - Cannot ignore to substitute a handler for `SIGSTOP` and `SIGKILL`
       - Imagine if all processes could not be stopped or killed, what would
       happen in that case?

# The life of a signal

- A signal goes through two phases:
    1. The signal generation phase: This is sent from the sending task with a
       specified destination, the OS will handle the delivery
    2. The signal delivery phase: The OS will try to deliver the signal to the
       destination task or process or group of processes
- The OS acts as the mailman between the sending and the receiving tasks.
- Examples:

```c
/* The hardware will send the SIGFPE to the task */
float x, y = 0.0;

x = x / y;
```

```c
/* We will receive a SIGSEGV to the task from the kernel */
int *ptr = 0;

*ptr = 3;
```

- Most common signals that we will use:
  - `SIGSEGV`: segmentation fault
  - `SIGINT`: Interrupt from keyboard (Ctl - C)
  - `SIGSTOP`: Stop process, cannot be caught or ignored
  - `SIGCONT`: Continue signal, sent after a stop
  - `SIGKILL`: Kill process, cannot be caught or ignored
  - `SIGTERM`: Terminate signal, **can** be caught or ignored
- From your source code, you can use the routine `kill(PID, signal_number)`to
send a signal to a specific task having a certain PID

# Signal Delivery

- When attempting to deliver a signal, the OS will check what the process has
  chosen to do with that signal
- A process can choose one of several **default** actions:
  - Terminate: The process or task is terminated
  - Dump: Terminate by generating a core dump (i.e., a stack trace of the
    current process) that can be read using `gdb`
  - Ignore: Simply ignore the signal
  - Stop: Put the process in a stopped state and do not schedule it any further
  - Continue: Continue from a stopped state
- In addition, a task may choose to install a **customer handler** to handle
  receiving a specific signal (with some exceptions as noted above)
  - We can achieve that by registering a function with the operating system
  - It is like we tell the OS that when we receive a specific signal, break the
    current execution, execute that custom function, and then go back to current
    state if not terminated
- Two ways to achieve this:

```c
/* This way is now deprecated, it's okay to use in this class, but better
 * learn the correct way to do so as well */
void signal_handler(int signum) {
	/* Add code here */
}

signal(SIGINT, signal_handler);
```

```c
struct sigaction new_action, old_action;

new_action.sa_handler = signal_handler;
sigmeptyset(&new_action.sa_mask);
new_action.sa_flags = 0;

sigaction(SIGINT, &new_action, NULL);
```

- Finally, a task may chose to temporarily **block** a signal. Blocking a signal
  is **NOT** the same as ignoring a signal. All what happens is that the signal
  is not instantly delivered, it remains as pending until the task decides to
  **unblock** that signal type.
  - The signal is delivered but not received
- Signals are **NOT** queued
  - Only one signal of a given type is dealt with a time
  - If a signal is blocked, it is set to pending until the task unblocks it and
  **consumes** the signal
