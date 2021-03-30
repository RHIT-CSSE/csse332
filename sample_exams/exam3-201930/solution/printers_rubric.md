# NOTES

10/20

uses a variable to track number of available printers but accesses the
variable in an unprotected way, leading to a concurrency bug where
some printers over max don't wait

15/20 Busywait

15/20 Busywait + plus some unnecessary printer specific semaphores that don't appear to do anything

10/20 Concurrency OK but you broke the way an available printer is selected

