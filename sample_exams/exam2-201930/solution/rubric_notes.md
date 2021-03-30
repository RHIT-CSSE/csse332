# Step 1: parallelize the merge (20/20)

5 - Use pthread\_create() in main

5 - Pass in pointer to storage for threadid - for this step it's okay of
	the same storage is reused

5 - Pass merge\_thread() as the thread function - be sure to cast it to
	the correct signature (void *(*)(void *))

5 - Pass null as the argument to the thread function

# Step 2: wait for the merge threads to finish (20/20)

5 - Use a loop to join all threads

5 - Use pthread\_join()

10 - Pass in threadid - for this step each thread must get its own storage

# Step 3: Semaphore to protect chunks array (20/20)

4 - Create a global semaphore and initialize it before the threads are created

8 - Correctly wait and post in merge\_thread while finding the two
	chunks to merge
	- in this step the two chunks should be found atomically; in step
	  5 the chunks can be done independently after the count is decrimented 
	- be sure there are posts before the exits

8 - Correctly wait and post in merge\_thread while find a chunk to store
	the results
	- be sure there is a post before the return and after the final loop

**NOTE**: 1 or 2 semaphores can be used.

# Step 4: Parallelize Sort with Parameters (15/15)

15 - use pthread\_create() and pthread\_join() similar to steps 1 and 2

# Step 5: Merge Everything to One Final Output (15/15)

6 - Properly create, initialize and use a global variable for the
	number of chunks and a semaphore to protect it.
	- make sure semaphore is posted in both paths of conditional
      exactly once.

6 - Make sure the check on the global count and the decriment happen
	atomically
  
3 - Adjusting count and selecting chunks are separate operations
	- adjusting count should happen before selecting chunks

# Step 6: Make sort respect NUM\_SORTERS (10/10)

5 - Limit the number of sort threads to NUM\_SORTERS and print out
	thread number in the output messages
	- be sure that the thread function argument passed in to
      sort\_chunk() is separate for each thread

5 - Divide the work between sorters in a reasonable way
