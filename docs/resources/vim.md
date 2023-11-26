---
title: Vim Tutorial
layout: post
readtime: true
date: Sat 25 Nov 2023
last-updated: Sat 25 Nov 2023
---

# Table of contents


# Introduction

During the exams, you will be asked to remotely login (via `ssh`) into a server
and doing all of the problems there. Therefore, it is in your best interest to
get good practice navigating your terminal and using a terminal-based text
editor.

Our text editor of choice is `vim`, but you are free to chose `nano` or `emacs`,
depending on your preferences and experience. We elected to focus on `vim` since
you have already encountered it in CSSE132.

`vim` is already installed on most distributions. However, in case you cannot
find it in your setup, you can use (assuming your are running Ubuntu or a
Debian-based distribution):

  ```shell
  $ sudo apt install -y vim
  ```

# Tutorial

In case you need a quick tutorial (you should have already done this one in
CSSE132), please read through the tutorial provided by `vim`, it should give you
all the basic concepts needed to navigate your editor.

You can launch the tutorial from your shell using:

  ```shell
  $ vimtutor
  ```

Please read through the tutorial then take some time to practice moving around a
file in `vim`.

# The basics

## Operation modes

`vim` has several modes of operation, the three that are of interest to us are
the following:

  1. `[n]ormal` mode: This is where you can navigate your text and perform some
     operations, such as copy and paste, delete (cut) lines or words, etc.

     `vim` by default starts in normal mode.
  2. `[i]nsert` mode: This is where you add (or insert) your text. This is
     pretty much the mode in which you will be doing your regular editing.

     You can enter `insert` mode by pressing `i` on your keyboard when in
     `normal` mode. This will start adding text under the current cursor.

     To exit `insert` mode and go back to `normal` mode, pressing the `Escape`
     key anywhere in your text.
  3. `[c]ommand-line` mode: This is where you can enter editor commands to do
     very useful things (like find and replace, regular expression-based
     operations, etc.). 

     To enter command-line mode from `normal` mode, press the `:` key on your
     keyboard, you will notice that your cursor will move to the bottom of your
     screen where you can enter commands.

     Try the following commands:
      - `:help Normal-mode` to read the help page on normal mode.
      - `:help Insert-mode` to read the help page on insert mode.
      - `:help Command-line-mode` to read the help page on command-line mode.

      To exit the help page and go back to your main page, enter command-line
       mode and type `:q` (for quit).

## Loading our config file

To make things look prettier and nicer for you, we have provided you with a
sample `vim` configuration file that sets up some parameters and shortcuts. To
get that file and place in the right spot, from your shell, do the following:

  ```shell
  wget -O ~/.vimrc https://www.rose-hulman.edu/class/csse/csse332/current/assets/files/vimrc
  ```

This will download our `vimrc` file and place it in your home directory under a
hidden file (thus the `.` at the start). Feel free to read through that file and
checkout the comments around each command, it is pretty self-explanatory.

## Some practice

Let's do some practice and write a simple C file to test our skills. From your
terminal windows, open a file called `example.c` using

  ```shell
  $ vim example.c
  ```

### Editing the file

You are now in `normal` mode, let's enter `insert` mode using `i`. Then start
typing the following code:

  ```c
  #include <stdio.h>

  int x = 3;

  int main(int argc, char **argv) {
    printf("Hello world!\n");
    printf("x = %d\n", x);
  }
  ```

Okay, now we want to save our file. To do so, go into `normal` mode by hitting
`Escape`. Then enter `command-line` mode using `:` and type `w`. This will save
your buffer to the file `example.c`.

  > Our configuration file (`vimrc`) maps a shortcut to exit `insert` mode into
    `normal` mode. If you type the characters `jk` right after one another (in
    `insert` mode), it will act as if you pressed `Escape`. Try it out!

  > _Note_: If for some reason, you need `jk` to appear in your text, simply
    write the `j`, wait 2 seconds, then write the `k`, it will keep you in
    insert mode and the `jk` sequence will show up.

### Some text manipulation

You can notice that in our file, the line `int x = 3;` shows up at line 3, but
what we really wanted it to be part of the `main` function. Let's make that
happen.

1. __Go to line 3__:
  
    To go to a specific line in `vim`, enter `normal` mode, then enter
    `command-line` mode using `:`, then type your line number followed by
    `Enter` (so your command line looks like `:3`). Line 3 should now be
    highlighted.

2. __Cut the line__:

    To cut line 3 so we can move it to another spot, enter `normal` mode, then
    hit the sequence `dd`, you will see that the line will disappear and the
    text will be shifted one line higher.

    Normally, we use `dd` both to cut a line and deleting it (simple cut it and
    don't paste it!).

    > To see what you have copied/cut, you can use `:reg` in `command-line` mode.

3. __Paste the line__:
  
    Now let's go paste that line where we want it to be. We want `int x = 3;` to
    appear right after the `int main(int argc, char **argv) {` line. Therefore,
    in `normal` mode, move your cursor to line 4 (`:4`), then in `normal`
    mode, hit the `p` key. The cut line (`int x = 3;`) will be pasted in the
    line after the current one!

    If you want to paste the cut line in the line above the current one, you can
    use `P` instead of `p`.

4. __Fixing the indentation__:
  
    If you notice now, the line is in the right spot, but the indentation is
    messed up, we don't like that!

    To fix the indentation, here are a few methods to do so:
      - Place your cursor on the line you want to indent. Then, in `normal`
        mode, hit the `==` keys.
      - Place your cursor at the opening (or closing) brace (or curly brace or
        parenthesis) surrounding the line you want to indent (in this case, at
        the brace at the end/start of the `main` function). Then, in `normal`
        mode, hit the `=%` keys. This will indent the whole block of code in
        between the braces/parenthesis.
      - Indent the whole file. In `normal` mode, hit the `gg=G` key sequence.
        This will reindent the whole file but will replace your cursor at the
        top of the file.

    Your lines should now look better, with proper indentation!

5. __Copy instead of cut__:

    To copy a line instead of cutting it, redo the same steps, except replace the
    `dd` sequence in step (2) with `yy` (where `y` stands for yank or copy).

6. __Save and exit the file__:

    Now, let's save and exit our file. We need to enter `command-line` mode and
    issue two commands: `w` for saving the file and `q` to quit the file.
    Fortunately, you can do both in one short by chaining them together, so you
    can use `:wq` to save the file and exit.

      > You can also use `:x` to achieve the same purpose.

# A warning

Often times, you will feel tempted to close off the terminal window without
properly quitting `vim`. This is not a good idea as it can risk you losing your
progress. To prevent that, `vim` often save temporary buffers that can be used
to recover a file in case of an incorrect quit (you might see something like
`.example.c.swp` show up in your directory when editing `example.c`).

To prevent this from messing up your submissions, please always make sure to
save and quit your files before your close the terminal window. It is as simple
as typing `:x` or `:wq` in command-line mode.

# Useful tips and tricks

For a full cheat sheet, please take a look at the well organized document
[here](https://vim.rtorr.com/); it is very useful when starting with `vim.`

## Widely used ones

Here are a couple of very useful commands that I often find myself using.

1. When entering `insert` mode from `normal` mode, you can use the following:
    - `i`: enter `insert` mode on the current cursor.
    - `a`: enter `insert` mode at the spot __next__ to the current cursor.
    - `A`: enter `insert` mode at the __end__ of the current line.
    - `o`: enter `insert` mode by adding a new line __below__ the current line.
    - `O`: enter `insert` mode by adding a new line __above__ the current line.

    - `r`: replace the character under the current cursor with the one pressed
      next.
    - `R`: enter replace mode. All the keys you press next will replace the
      characters starting from the current cursor.

2. In `normal` mode:
    - `l`: move the cursor to the next character (equivalent to `right` arrow
      key).
    - `h`: move the cursor to the previous character (equivalent to `left`
      arrow key).
    - `j`: move the cursor to the character __below__ the current character
      (equivalent to the `down` arrow key).
    - `k`: move the cursor to the character __above__ the current character
      (equivalent to the `up` arrow key).
    - `w`: move to the start of the next word.
    - `e`: move to the end of the current word, or the end of the next if
      currently at the end of a word.
      > `ea` in normal mode will move to the end of the current word and then
        start editing right after it!
    - `u`: undo the last action.
    - `<C-r>` : redo the last action. (`<C-r>` stands for holding the control
      key and then pressing `r`).
    - `zz`: visually move the current line to the middle of the buffer; useful
      if you are editing a file and reached the end of it and need to see more
      of it.
    - `dw`: cut the word under the cursor, up to __and including__ the `space`
      character following the word.
    - `de`: cut the word under the cursor, up to __but not including__ the
      `space` character following the word.
    - `gg`: Go to the start of the file.
    - `<Shift-g>` or `G`: Go to the end of the file.
    - `$`: Go to the end of the line.
    - `^`: Go to the first character in the line, skipping spaces and tabs.
    - `0`: Go to the first character in the line, not skipping spaces and tabs.
    - `~`: Switch case for the character under the current cursor.

---

This page was last edited by Mohammad Noureddine on {{ page.last-updated }}. If
you notice any typos or inaccuracies, please open a GitHub issue on this
[repository]({{site.gh_repository_url}}).


