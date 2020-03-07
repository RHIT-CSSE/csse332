# This is application for handling author data.  Most of the actual
# code is in an author library (author.c/author.h). To acutally
# produce an author executable, though, we must combine the library
# with main.c which handles input/output.
#
# We also want to build an exectuable that runs some unit tests on the
# author library - this requires the author library, the unittest
# library CuTest, plus unittest.c which actually contains the unit
# test testcases and a main.
#
# Write a makefile that can build 2 different executables:
#
# author - which is built from main.c
# unittest - which is built from unittest.c
#
# both executables depend on the author library (author.c/author.h)
# unitest also depends on the CuTest library (CuTest.c/CuTest.h)
#
# Create makefile rules that build both executables
#
# make author
# make unittest
# make all   <- builds both
#
# Your rules should use .o files wherever possible to speed building,
# and your should never rebuild them unnecessarily.
# 
# Be sure you set dependencies on .h files correctly.  HINT:
# executables (e.g. author and unnitest) don't need to depend on .h
# files.  They will be rebuilt automatically when .h changes trigger
# rebuilds of their .os.

## Here's a free rule for your convenience
## Remove all the compilation and debugging files
clean: 
	rm -f author unittest *.o *~
