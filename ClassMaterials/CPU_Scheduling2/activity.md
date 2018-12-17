
# The Problem

Imagine a 3 level feedback queue as described in the slides on MLFQ:
Final Rules.  Note that the final level is just Round Robin because
priority can't get any lower.  The quantum for priority 1 is 2, the
quantum for priority 2 is 4, the quantum for priority 3 is 8.


We want to schedule 3 processes as described below.  All processes
start at t = 0.

* Process A will take 5 timeslots and no IO
* Process B has one timeslot and then will wait for an IO operation
  that will take 5 timeslots before it renters the ready queue.  After
  that, it will take 4 timeslots of CPU
* Process C will take 10 timeslots and no IO


In this case, rather than do a GANTT chart, we ask you to 1) specify
what process(s) CPU is running during at particular time; 2) plot the
snapshoots of the queue states (i.e., what process is at which queue)
at these moments

## t = 0
        
Queue 1: A -> B -> C
Queue 2:
Queue 3:

CPU is running Process A

## t = 2






## t = 3







## t = 5







## t = 7






## t = 8






## t = 10


