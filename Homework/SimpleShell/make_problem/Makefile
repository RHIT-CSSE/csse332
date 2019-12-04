# So this project has two different executables
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
# and your should never rebuild them unnecessarily.  Be sure you set
# dependencies on .h files correctly.

## Here's a free rule for your convenience
## Remove all the compilation and debugging files
clean: 
	rm -f author unittest *.o *~
