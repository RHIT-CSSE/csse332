# EXAM2-202120

Solve the 3 semaphore problems and a pthread problem.

Here is a list of problems with its points in the order of difficulty.

* pthread: reverse_verify STEP 1 (15 points)
* pthread: reverse_verify STEP 2 (15 points)
* canoe (20 points)
* chopsticks (25 points)
* priority_tunnel (25 points)


RUBRIC FOR SEMAPHORE QUESTIONS:


* You can get 25% points if your semaphore-based solution reliably
  produces correct output with the given data (and small
  modifications).  0 points if doing this requires weird sleeps or
  other hacky non-general purpose code.
* You can get 50% points if you did above and your solution is
  plausible and does not do something obviously bad.  It's impossible
  to enumerate every obviously bad thing but here's a big one : having
  multiple concurrent threads modify a global variable.  Basically
  this is for solutions that basically are OK but miss some edge
  cases.
* You can get 75% points if you have a fully working solution but
  you utilize busywaits or non-standard semaphore functions
  (i.e. stuff other than init, post, wait)
* A fully correct solution, even if it's not the one we envisioned, is
  100%.
