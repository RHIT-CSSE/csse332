# NOTES

10/20

uses a variable to track number of available printers but accesses the
variable in an unprotected way, leading to a concurrency bug where
some printers over max don't wait

15/20 Busywait

15/20 Busywait + plus some unnecessary printer specific semaphores that don't appear to do anything

10/20 Concurrency OK but you broke the way an available printer is selected

# GRADED STUDENTS

Abernathy

15/20 Busywait.  Also, lock sem is unnecessary but doesn't cause any problems

Agarwal Abhimanyu

20/20 Nice work.  Could be slightly nicer if you inited you allBusy to NUM_PRINTERS/

Alba-Lopez

20/20 Great solution

Bissmeyer Philip

20/20 Great solution

Bradshaw Alexander

0/20 No printer solution submitted

Burke Brian

20/20 Great solution

Byers Robert

10/20

uses a variable to track number of available printers but accesses the
variable in an unprotected way, leading to a concurrency bug where
some printers over max don't wait

Cesario Luke

20/20 Great solution.  But use NUM_PRINTERS not 3! :)

Chakraborty Praneet

15/20 Busywait

Chen Lilin

15/20 Busywait + plus some unnecessary printer specific semaphores that don't appear to do anything

Chen Xiangbei 

10/20 Concurrency OK but you broke the way an available printer is selected
