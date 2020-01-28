# The problems

All problems are worth 20 points.

In approximate order of difficulty:

* alternating.c
* coffee_break.c
* order.c
* printers.c
* bus.c

Instructions are in the c files.

# How we will grade

There will be 5 problems each worth 20 points

* You can get 5/20 points if your semaphore-based solution reliably
  produces correct output with the given data (and small
  modifications).  0 points if doing this requires weird sleeps or
  other hacky non-general purpose code.
* You can get 10/20 points you did above and your solution is
  plausible and does not do something obviously bad.  It's impossible
  to enumerate every obviously bad thing but here's a big one : having
  multiple concurrent threads modify a global variable.  Basically
  this is for solutions that basically are OK but miss some edge
  cases.
* You can get 15/20 points if you have a fully working solution but
  you utilize busywaits or non-standard semaphore functions
  (i.e. stuff other than init, post, wait, and destroy)
* A fully correct solution, even if it's not the one we envisioned, is
  20/20
