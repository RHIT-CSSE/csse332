# Prelude: Getting the Source Code (0 points)

From your VSCode terminal that is connected to the virtual machine, open a new
terminal window and enter the following:
```shell
$ cd
```
At this point, you should be in your home directory, to double check that, use
`pwd` and you should see something like
```shell
$ pwd
/home/csse/
```

Now create a directory for the exam

```shell
$ mkdir exam2
```
then cd into it
```shell
$ cd exam2
```

From there, clone the following repository:
```shell
$ git clone ssh://student@332exam.csse.rose-hulman.edu//exam2-202230
```
when asked for the password, enter the following password
```shell
AwgVtQY2
```
You only have read-only access to this repository so please don't try to push
your changes, you will get an authentication error. 

Now you can change into the exam directory and start your exam
```shell
$ cd exam2-202230
```

# The Exam

CV questions (in order of appoximate difficulty)

* shower.c (20 points)
* task_dispatch.c (20 points)
* stoplight.c (20 points)

pthread question

* sieve.c (40 points)