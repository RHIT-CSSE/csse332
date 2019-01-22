# Milestone 4

# Objective

In this milestone you will implement functions for deleting and
creating files, and add several new commands to your shell.  At the
end of the milestone, you will have a fully functional single-process
operating system about as powerful as CP/M.


# Tools

You will use the same utilities you used in the last milestone, and
you will also need to have completed the previous milestones
successfully.  There are no new files provided for this milestone.
Copy your solution for the previous milestone from the m3
folder into the m4 folder and build upon it.


# Write Sector

Create a writeSector function in kernel.c to go with
the readSector function.  Writing sectors is provided by the
same BIOS call as reading sectors, and is almost identical.  The only
difference is that AH should equal 3 instead of 2.  Your
write sector function should be added to interrupt 0x21, and
should be handled as follows:


Write Sector
* AX = 6
* BX = address of a character array holding the sector to write
* CX = the sector number

If you implemented readSector correctly, this step should be
very simple.


# Delete File

Now that you can write to the disk, you can delete files.  Deleting a
file takes two steps:

* Mark all the sectors currently reserved for the file in the Disk Map as free, and
* Set the first byte in the file's directory entry to 0x0.


Add a deleteFile function to the kernel which takes as an
argument a character array holding the name of the file to be deleted
(e.g. void deleteFile(char* name)).  The function should find the
file in the directory and delete it if it exists.  Your function
should do the following:


* Load the Directory and Map to separate 512 byte character arrays
* Search through the directory and try to find the file name.
* Set the first byte of the file name to 0x00.
* Step through the sectors numbers listed as belonging to the
  file.  For each sector, set the corresponding Map byte to 0x00.  For
  example, if sector 7 belongs to the file, set the 7th Map byte to
  0x00.

  Note: remeber the Map is zero-indexed.
* Write the character arrays holding the Directory and Map back
  to their appropriate sectors on disk.


Notice that this does not actually delete the file from the disk.  It
just makes it available to be overwritten by another file.  This is
typically done in operating systems; it makes deletion fast and
undeletion possible.

# Adding an interrupt

You should add delete file to interrupt 0x21:


Delete File
* AX=7
* BX=address of the character array holding the file name

# Adding to the shell

You should make a "delete filename" command to the shell.  Try
loading message.txt onto floppya.img.  When you type
"delete messag", the interrupt should be called and
messag should be deleted.  When you type "type
  messag", nothing should be printed.

You should open up floppya.img with hexedit before
and after you call "delete messag".  You should see the
appropriate Map entries changed to 0 and the file marked as deleted in
the Directory.


# Writing A File

Add a writeFile function to the kernel that takes three
arguments, a character array holding the file name, a character array
holding the file contents, and the number of sectors to be written and
writes a file to the disk (e.g. void writeFile(char* name,
  char* buffer, int numberOfSectors)).  Then add writeFile
to interrupt 0x21 as follows:

Write file:
* AX = 8
* BX = address of character array holding the file name
* CX = address of character array holding the file to be written
* DX = number of sectors


Your function should:

* Load the Map and Directory sectors into separate buffers
* Find a free directory entry (i.e. one that begins with 0x00)
* Copy the first six characters of the name to that directory
  entry.  If the name is less than six bytes, fill in the remaining
  bytes with 0x00
* For each sector making up the file:
  
    1. Find a free sector by searching through the Map for a 0x00
    2. Set that sector to 0xFF in the Map
    3. Add that sector number to the file's directory entry
    4. Write 512 bytes from the buffer holding the file to that sector

* Fill in the remaining bytes in the directory entry to 0x00
* Write the Map and Directory sectors back to the disk


If there are no free directory entries or insufficient free sectors,
your writeFile function should just return.

# Copying A File

Write a copy command for the shell.  The copy command should
have the syntax "copy filename1 filename2".  Without
deleting filename1, the copy command should create a file with name
filename2 and copy all the bytes of filename1 to filename2.  Your copy
command should use only interrupt 0x21s for reading and
writing files.

You can test this by loading message.txt onto
floppya.img.  At the shell prompt, type "copy messag
  m2".  Then type "type m2".  If the contents of
message.txt print, your copy function most likely works.

You should check the directory and map in floppya.img using
hexedit after copying to verify that your writing function
works correctly.

If your copy function works correctly, you should be able to copy
shell to another file.  Then try executing the duplicate
shell.  If you get the shell prompt, it works correctly.


# Listing The Directory Contents

Write a shell command "dir".  This command should print out
the files in the directory.  Only extant (not deleted) files should be
listed.  Optionally (for extra credit), you might also print out the
sizes, in sectors, of the files.


# Creating A Text File

Write a shell command "create filename".  This command
should allow you to create a text file.  The create command should
repeatedly prompt you for a line of text until you enter an empty
line.  It should put each line in a buffer.  It should then write this
buffer to a file.

Test this step by calling "type filename" and see if what
you typed is printed.

You have now created a fully functional command-line based single
process operating system!  Your operating system has nearly the same
functionality that CP/M did in the early 1980s, and is almost
as powerful as MS-DOS version 1.

# Turning It In

Be sure to submit your updated kernel.c and Makefile
or compileOS.sh along with all of the files necessary to
build and run your OS to your team repository in the the m4
folder.  Be sure to include a README that explains 1) what
you did, and 2) how to verify it.  Please include a comment at the top
of each file with the name of each team member and your team number.
In addition, be sure to submit all other deliverables that are
highlighted on the project’s main page.

