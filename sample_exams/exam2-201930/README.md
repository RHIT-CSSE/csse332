# Exam 2: Threading

In this exam you will build an implementation of merge sort designed
to sort the letters of strings.  That is it will take a file like
"hello world" and produce " dehllloorw".

<!-- markdown-toc start - Don't edit this section. Run M-x markdown-toc-refresh-toc -->
**Table of Contents**

- [Starting point](#starting-point)
- [Step 1: parallelize the merge (20 points)](#step-1-parallelize-the-merge-20-points)
- [Step 2: wait for the merge threads to finish (20 points)](#step-2-wait-for-the-merge-threads-to-finish-15-points)
- [Step 3: Semaphore to protect chunks array (20 points)](#step-3-semaphore-to-protect-chunks-array-20-points)
- [Step 4: Parallelize Sort with Parameters (15 points)](#step-4-parallelize-sort-with-parameters-20-points)
- [Step 5: Merge Everything to One Final Output (15 points)](#step-5-merge-everything-to-one-final-output-15-points)
- [Step 6: Make sort respect NUM\_SORTERS (10 points)](#step-6-make-sort-respect-numsorters-10-points)

<!-- markdown-toc end -->


## Starting point

The given code already does a fair bit sorting and merging.

    ./mergesort fiveletterwords_small.txt
    sorting 'seven'
    sorting 'about'
    sorting 'again'
    sorting 'heart'
    merging 'eensv' and 'abotu'
    merge of eensv and abotu produces abeenostuv
    merging 'aagin' and 'aehrt'
    merge of aagin and aehrt produces aaaeghinrt
    output chunk: aaaeghinrt
    output chunk: abeenostuv
    done

As you can see, the first step sorts the individual words (or, as
they're described in the code 'chunks').  The second step merges
sorted chunks into longer sorted chunks.

This code does not produce the correct output right now - each initial
chunk is only merged once (so if you input 100 5-letter words the
output would be 50 10-letter output string not one 500-letter string).
You can see this more clearly if you try fiveletterwords\_big.txt as
your input.  We will eventually fix this problem, but not till Step 5.

# Step 1: parallelize the merge (20 points)

So the merge function (called merge\_thread):

1. searches the chunks array, finds two (sorted) input strings
2. removes the strings from the chunks array
3. merges the input strings into one sorted output string
4. puts the output string back in the chunks array

One thing to note about this function is that it doesn't behave well
if its run and there are not two input strings in the chunks array for
it to merge.  As the code stands now though, this isn't really a
problem - we only merge input size/2 times, which is less merging than
we need to do if we'd like to merge everything into one big string.

In the existing code, main calls merge\_thread inputsize/2 times
(serially).  This produces one level of merging.  For example, if
there are 10 chunks main will call merge\_thread 5 times producing 5
output chunks.

The only thing we want in this step is for the merges (merge\_thread)
to run in parallel (that is, if I have 10 input strings, I want 5
merges in parallel to produce 5 output strings).  Use pthreads to make
this happen in paralell.  Most of your modifications should be in the
main function for this step.  The output should look similar to the
starting point, except the "merge of XXX and YYY produces ZZZZZ" lines
may occur in various orders.  For your testing, it's a little more
obvious if you use fiveletterwords\_big.txt as input.

You do not have to join in this step...but you may have to put a sleep
in the parent to prevent it from quitting before all the child threads
have produced their output.

You do not need to worry about concurrency bugs in this step (though
yes, there are some).

# Step 2: wait for the merge threads to finish (20 points)

This step requires step 1.

We only want the parent to print the output\_chunk lines after all the
threads are complete.  Remove any sleep you added in Step 1 and use
join to wait till everything is done before printing.

The output should be the same as Step 1.

# Step 3: Semaphore to protect chunks array (20 points)

This step requires step 1.

When multiple merge\_thread threads run at the same time, their use of
the global chunks array can cause concurrency bugs.  To make one of
these bugs obvious, uncomment the usleep(100) at the top of
merge_thread.  Modify the merge\_thread function using semaphores to
fix the concurrency bugs.

Think carefully about the correctness of your approach!  Just because
you fix the particular concurrency issue caused by the usleep doesn't
mean merge\_thread is correct.  For example, consider how
merge\_thread outputs as well as how it gets its inputs.

Also be sure you do not accidently make merge\_thread do its
calculations serially.

The output should be the same as Step 1.

# Step 4: Parallelize Sort with Parameters (15 points)

Now we will consider the sort step that happens before the merge.
Sorting is done by the sort\_chunk function which takes the index in
the chunk array to sort as a parameter.  Make the individual string
sorts run in parallel, as part of this process you can modify the
sort\_chunk a bit.  However, use parameters and not semaphores to
ensure that each parallel sort\_chunk works on a different input
chunk.

To help you test, uncomment the sleep in sort\_chunk so that if it
runs serially it takes a while.

Output should be similar to Step 1 but the sorts might occur in
different orders.

It is also important that no merging start till all the sorts are
completed, so add appropriate joining to main to ensure merges do not
start till sorts are finished.  Or, if you can't get it correct you
can use sleep but that will cost you points.

# Step 5: Merge Everything to One Final Output (15 points)

This step requires step 3.

To get our entire file to a single sorted string, we simply need to
continue to merge the outputs of previous merges until we only have a
single output remaining.  To get this right in parallel is a bit
tricky though.

The simplest solution is to have a single global that keeps track of
the number of sorted strings in the chunks array.  Before any of the
merges are started, that global should be set to the number of inputs.
Prior to each merge the global should be consulted to determine if a
merge is needed.  When your merge removes 2 input strings from the
array it should decrement this value by 2.  When it inserts a sorted
output it should increase the value by 1.

Here's the trick: if the global is >= 2 when it is checked, then
merge\_thread should run another merge because there is clearly enough
data for it to merge another time.  If on the other hand the global's
value is 1, then there are two possibilities:

1.  All merging is done (and we should exit)
2.  There is at least one other thread still processing a merge that
    will finish and then more merging will be possible.
    
If #2 is true, the other currently unfinished thread will be able to
complete that additional merge.  In both cases this thread should exit
This will produce the merge behavior we want (i.e. half the mergers
die off in each "generation").

Be sure you use semaphores as needed to avoid concurrency bugs in your
solution.

Output:

    ./mergesort fiveletterwords_big.txt
    sorting 'seven'
    sorting 'again'
    sorting 'about'
    sorting 'heart'
    sorting 'pizza'
    sorting 'happy'
    sorting 'green'
    sorting 'water'
    sorting 'music'
    sorting 'three'
    sorting 'party'
    sorting 'woman'
    sorting 'apple'
    sorting 'dream'
    sorting 'tiger'
    sorting 'river'
    sorting 'house'
    sorting 'money'
    sorting 'alon'
    sorting 'eafter'
    sorting 'thing'
    sorting 'light'
    sorting 'story'
    sorting 'women'
    merging 'eensv' and 'abotu'
    merge of eensv and abotu produces abeenostuv
    merging 'aagin' and 'aehrt'
    merge of aagin and aehrt produces aaaeghinrt
    merging 'aipzz' and 'aertw'
    merge of aipzz and aertw produces aaeiprtwzz
    merging 'ahppy' and 'eegnr'
    merge of ahppy and eegnr produces aeeghnppry
    merging 'cimsu' and 'eehrt'
    merge of cimsu and eehrt produces ceehimrstu
    merging 'aprty' and 'amnow'
    merge of aprty and amnow produces aamnoprtwy
    merging 'ademr' and 'aelpp'
    merge of ademr and aelpp produces aadeelmppr
    merging 'egirt' and 'eirrv'
    merge of egirt and eirrv produces eegiirrrtv
    merging 'emnoy' and 'ehosu'
    merge of emnoy and ehosu produces eehmnoosuy
    merging 'alno' and 'aeefrt'
    merge of alno and aeefrt produces aaeeflnort
    merging 'emnow' and 'ghint'
    merge of emnow and ghint produces eghimnnotw
    merging 'ghilt' and 'orsty'
    merge of ghilt and orsty produces ghilorstty
    merging 'aaaeghinrt' and 'abeenostuv'
    merge of aaaeghinrt and abeenostuv produces aaaabeeeghinnorsttuv
    merging 'aeeghnppry' and 'aaeiprtwzz'
    merge of aeeghnppry and aaeiprtwzz produces aaaeeeghinppprrtwyzz
    merging 'aamnoprtwy' and 'ceehimrstu'
    merge of aamnoprtwy and ceehimrstu produces aaceehimmnoprrsttuwy
    merging 'eegiirrrtv' and 'aadeelmppr'
    merge of eegiirrrtv and aadeelmppr produces aadeeeegiilmpprrrrtv
    merging 'aaeeflnort' and 'eehmnoosuy'
    merge of aaeeflnort and eehmnoosuy produces aaeeeefhlmnnooorstuy
    merging 'ghilorstty' and 'eghimnnotw'
    merge of ghilorstty and eghimnnotw produces egghhiilmnnoorstttwy
    merging 'aaaeeeghinppprrtwyzz' and 'aaaabeeeghinnorsttuv'
    merge of aaaeeeghinppprrtwyzz and aaaabeeeghinnorsttuv produces aaaaaaabeeeeeegghhiinnnoppprrrstttuvwyzz
    merging 'aadeeeegiilmpprrrrtv' and 'aaceehimmnoprrsttuwy'
    merge of aadeeeegiilmpprrrrtv and aaceehimmnoprrsttuwy produces aaaacdeeeeeeghiiilmmmnoppprrrrrrstttuvwy
    merging 'egghhiilmnnoorstttwy' and 'aaeeeefhlmnnooorstuy'
    merge of egghhiilmnnoorstttwy and aaeeeefhlmnnooorstuy produces aaeeeeefgghhhiillmmnnnnooooorrssttttuwyy
    merging 'aaaacdeeeeeeghiiilmmmnoppprrrrrrstttuvwy' and 'aaaaaaabeeeeeegghhiinnnoppprrrstttuvwyzz'
    merge of aaaacdeeeeeeghiiilmmmnoppprrrrrrstttuvwy and aaaaaaabeeeeeegghhiinnnoppprrrstttuvwyzz produces aaaaaaaaaaabcdeeeeeeeeeeeeggghhhiiiiilmmmnnnnooppppppr
    rrrrrrrrssttttttuuvvwwyyzz                                                                                                                                   
    merging 'aaaaaaaaaaabcdeeeeeeeeeeeeggghhhiiiiilmmmnnnnoopppppprrrrrrrrrssttttttuuvvwwyyzz' and 'aaeeeeefgghhhiillmmnnnnooooorrssttttuwyy'
    merge of aaaaaaaaaaabcdeeeeeeeeeeeeggghhhiiiiilmmmnnnnoopppppprrrrrrrrrssttttttuuvvwwyyzz and aaeeeeefgghhhiillmmnnnnooooorrssttttuwyy produces aaaaaaaaaaaaab
    cdeeeeeeeeeeeeeeeeefggggghhhhhhiiiiiiilllmmmmmnnnnnnnnooooooopppppprrrrrrrrrrrssssttttttttttuuuvvwwwyyyyzz                                                   
    output chunk: aaaaaaaaaaaaabcdeeeeeeeeeeeeeeeeefggggghhhhhhiiiiiiilllmmmmmnnnnnnnnooooooopppppprrrrrrrrrrrssssttttttttttuuuvvwwwyyyyzz
    done
    
# Step 6: Make sort respect NUM\_SORTERS (10 points)

This step requires Step 4.

So right now we start a number of sort threads equal to the size of
our data.  That is normally not a good idea, because a large datafile
can easily produce threads than our system can support (if only we had
a cool hybrid threads system -eh? but please don't use that).
Instead, we don't want to create more than NUM\_SORTERS sort threads.
Each sorter thread should sort data_size/NUM\_SORTERS individual input
chunks.

To make it obvious which thread is doing what, print some sort of
thread number with your sorting message.

    ./mergesort fiveletterwords_big.txt
    thread 1 sorting 'seven'
    thread 2 sorting 'about'
    thread 3 sorting 'again'
    thread 4 sorting 'heart'
    thread 1 sorting 'pizza'
    thread 2 sorting 'water'
    thread 3 sorting 'happy'
    thread 4 sorting 'green'
    thread 1 sorting 'music'
    thread 3 sorting 'party'
    thread 2 sorting 'three'
    thread 4 sorting 'woman'
    thread 1 sorting 'dream'
    thread 3 sorting 'tiger'
    thread 2 sorting 'apple'
    thread 4 sorting 'river'
    thread 1 sorting 'money'
    thread 3 sorting 'alone'
    thread 2 sorting 'house'
    thread 4 sorting 'after'
    thread 1 sorting 'women'
    thread 3 sorting 'light'
    thread 2 sorting 'thing'
    thread 4 sorting 'story'
    <...all the usual merge output..>

It is not important if you divide the work between the threads
differently than I did - as long as each thread gets an approximately
equal amount of work.
