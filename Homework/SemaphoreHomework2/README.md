Solve the 5 problems, each worth 20 points.  Note the sample output
files to check your solutions (though there is randomness, so there
may be some variations)

Some guidance on how we grade:

* You can get 50% points if your semaphore-based solution reliably
  produces correct output with the given data (and small modifications) 
  and your solution is plausible and does not do something obviously bad. 
  It's impossible to enumerate every obviously bad thing but here's a big 
  one : having multiple concurrent threads modify a global variable.  
  Basically this is for solutions that basically are OK but miss some edge
  cases.
* You can get 50% points if you have a fully working solution but
  you utilize busywaits or non-standard semaphore functions
  (i.e. stuff other than init, post, wait, and destroy)
* A fully correct solution, even if it's not the one we envisioned, is
  worth 100%

1. balloon
2. threeJobs
3. band
4. pri_sem
5. or_sem
