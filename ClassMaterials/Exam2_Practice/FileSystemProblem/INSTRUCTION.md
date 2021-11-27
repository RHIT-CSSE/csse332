# File System Practice



### Description

In this problem, we ask you to implement a slightly advanced file system compared to the one in your project. 



In your project, the file system uses a sector on the disk as a directory to track information about each individual file (If you feel rusty on that, click [here](https://github.com/RHIT-CSSE/csse332/blob/master/Homework/BareMetalOS/m3/README.md#the-file-system) to review the part in Milestone 3). In this sector, the file system utilizes 32 bytes for each file: the first 6 bytes stores the file name, and the rest 26 bytes are (at most) 26 sector indexes where the file content locates. The limitation of such a file system is that the maximum file size is upper capped by `26*512(sector size) = 13312`.  



In order to increase the maximum file size, we propose to **use an extra file entry in the directory to store more sector indexes**.  Specifically, first, we want to use the last byte (i.e., the 32-nd byte) to indicate where is this extra file entry located if needed. If it is zero, it means the current file is relatively small and does not need extra space. If this byte is `N`, it means more sector indexes of this file are stored in the `N+1`th file entry (0 is the first entry). In this extra entry, we set the first byte as `0x01` to indicate this is an extra entry of another file entry (it is impossible to misinterpret this entry as a regular file entry, because file names can not start with `0x01` which is not a letter or number or symbol). The rest of the 31 bytes are used to store more sector indexes for this file. Overall, by using this approach, we can expand the maximum file size to `(25+31)*512 = 28672`.   



For example, in the current folder, there a file named *lotr* with the file size of 22KB. The file entry in the directory of this file looks like:

```
6c 6f 74 72 00 00 0e 0f 10 11 12 13 14 15 16 17
18 19 1a 1b 1c 1d 1e 1f 20 21 22 23 24 25 26 03
```

The first four bytes are the file name - *lotr*. Starting from the 7-th byte (0x0e) to the 31-st byte(0x26), it stores all the regular sector indexes where part of the file content locates. The last byte (i.e., 32-nd byte) 0x03 indicates where is the extra file entry. The following shows the content in this particular entry:

```
01 27 28 29 2a 2b 2c 2d 2e 2f 30 31 32 33 34 35
36 37 38 39 00 00 00 00 00 00 00 00 00 00 00 00
```

It shows the rest part of the file content is stored at sector 0x27 (39 in decimal) all the way to sector 0x39 (57 in decimal).  The **zeros** in the end mean no more sectors for this file. 



### How To Do It

#### What is ready?

The `loadFile` program has been changed to support this feature. The `Makefile` can load necessary file(s) into the *floppya.img*. By typing `make`, it will automatically compile everything and start to run the `qemu`. The `shell` program will be launched once the system is booted. The `shell` can take several input commands (e.g., `type`, `copy`) just like what you've done in Milestone 3 and 4. 



> NOTE: Don't change any macro definitions (the # define) in this task. It may crash your code if you are not careful.

#### STEP 1: 

To read a file, we can do `type <filename>` .  When receiving this command, `shell` calls a function `runTypeCommand(char *file)` inside `shell.c`. In this function, it first calls 0x21 interrupt 3 to read the file into a buffer and then calls 0x21 interrupt 0 to print on the screen. For now, the print interrupt should be commented out since the read file function is not implemented correctly yet. You need to go the `kernel.c` to change the `readFile(char *name, char *buffer)`function to support the extra entry. 



In this step, you only need to print out **THE EXTRA FILE ENTRY NUMBER** of a file if it uses such index. For example, if you find such index (say `indir_entry`) and it is not zero, then you can use the following code to print that number on the screen.

``` c
itoa(indir_entry,print_buff);                                                
printString(print_buff);
printNewLine();
```

where `itoa(int n, char* buff)` is a function to convert an integer into a string for printing. In this case, `print_buff` is a character array (say with size 16) you need to define at the beginning of the function (This is ANSI C, all variable declarations have to be put in the beginning). The desired output will be

```
SHELL>type messag
SHELL>type lotr
3
SHELL>
```

As the `messag` file is small and never uses extra entry, so `type messag` prints nothing. However, file `lotr` is fairly big that requires the extra space, it prints the extra file entry 3 (the fourth entry) where are more sector indexes are stored.



#### STEP 2:

If the extra entry is used, you need to read the sector indexes stored in that entry. In this step, you need to write code to read the entry number and then store the sector indexes in that entry to a buffer. Print these sector indexes in that entry as a line on the screen.  



Again, you will need to use `itoa()` function to convert a number into a string. Even nicer is that, the `itoa()` function automatically append a white space after the number which innately plugs in a separator for you when you print numbers in a loop.  Remember to put a `printNewLine()` after you finish printing the numbers. 



The desired output looks like:

```
SHELL>type lotr
3
39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57
SHELL>
```

The second line of numbers is the sector indexes stored in the extra entry.



#### STEP 3:

Complete the implementation to read entire file content into the `buffer`.  The variable `bufferPos` is used to track the current position of the filled data in the buffer. In the end, this variable should tell us the size of the file. However, since we always read data by sector, this number is always multiple of the sector size (512).  

Print the number `bufferPos` on the screen with

```
itoa(bufferPos,print_buff);
printString(print_buff);
printNewLine();

```

The desired output looks like

```
SHELL>type lotr
3
39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57
22528
SHELL>
```

NOTE: When the kernel loads the shell file to run, the size of the shell is also printed. Just ignore that number. 



#### STEP 4: 

If you have successfully completed the previous step, this step is very straightforward. Just uncomment the line that prints out buffer in `shell.c`. You should see all the content of `lotr` on the screen. Notably, you should be able to see the last sentence saying `\\This is the end of the file`.



#### STEP 5: 

At this point we have the `readFile` function modified to support this advanced feature, now you need to modify the `writeFile` function accordingly to support the same design. Once you complete modifying `writeFile`, you can test it by using `copy` command, for example,

```
SHELL>copy lotr lotr1
SHELL>type lotr1
...........
...........
...Blabla...
............
//This is the end of the file
```

