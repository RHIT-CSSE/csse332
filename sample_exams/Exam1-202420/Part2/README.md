---
title: 'CSSE332 Exam 1 Part 2'
date: 'December 21, 2023'
term: 'Winter 2023-24'
---

<style>
@page {
margin: .25in;
}
</style>

<style>
pre[class*='sourceCode']
{font-size: 0.8em;}
</style>

# CSSE332 Exam 1 Part 2

**Please write your name here:** <hr/>

**Please write your section number here:** <hr/>


This exam has three problems, located in `problem4.c`, `problem5.c`, and `problem6.c`.
You will have a total of 50 minutes to complete all parts.

## Accessing the exam

1. ssh into the class server.

2. You will find a new file, `exam_1_2.zip`. Unzip it with:

	 > `unzip -P enter_exam1_password_here exam_1_2.zip`

   Your instructor will tell you what the exam password is.

3. `cd exam_1_2` to access the exam code.

## Allowed Resources

This exam is **closed book**. You are allowed to use one 8.5'' by 11'' single
sided page of notes that you develop yourself. You may not use a web browser or
any means of communication (e.g., cell phones, MS Teams, Discord, etc.)

<div style="page-break-after: always;"></div>

# Problem 4: 70 points

In problem4.c, use `pipe(2)` to run the provided program, *`buffalosay.bin`* (compiled from `buffalosay.c`) to print a user-provided message.

`buffalosay.bin` is similar to `buffalosay.bin` from an earlier in-class activity. It uses `pipe(2)` instead of command line arguments to read the message to "say." Inspect `buffalosay.c` for details on its operations.

## Sample Output:

```
$ ./problem4.bin "hello world"
[ASCII art of a buffalo]
< hello world > - Buffalo, probably

buffalosay exited with exit code [a random code each time]
$
```

<div style="page-break-after: always;"></div>

# Problem 5: 20 points

Problem 5 is an extension of Problem 4. In this variation, call *`buffalopipe.bin`* instead of `buffalosay.bin`, then print the piped output from `buffalopipe.bin`. Inspect `buffalopipe.c` for details on its operations.

## Sample Output:

```
$ ./problem5.bin "hello world"
[ASCII art of a buffalo]
< hello world > - Buffalo, probably

buffalopipe exited with exit code [a random code each time]
$
```

<div style="page-break-after: always;"></div>

# Problem 6: 10 points

Problem 6 is an extension of Problem 5. In this variation, pipe the output of `buffalopipe.bin` to a new program, `nobuffalopipe.bin`, which strips the buffalo ASCII art from the output. Print the final output from `nobuffalopipe.bin.`

## Sample Output:

```
$ ./problem6.bin "hello world"
nobuffalopipe sent this message in the pipe:
< hello world > - Buffalo, probably

buffalopipe exited with exit code [a random code each time]
nobuffalopipe exited with exit code [a random code each time]

$
```

# Submission instructions

Leave your code in your exam folder and exit your ssh session.
  
