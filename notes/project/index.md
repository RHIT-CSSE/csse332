---
layout: post
title: Project Description
readtime: true
date: Sun Jan 28 22:30:29 2024
---

# Ideas out there

- Implement `pthreads` in the xv6 kernel.

Requirements:

- Threads must share all the address space, including newly added pages, if
  any.

- Sharing of pages must be as efficient as possible.

- Simply assigning the page table of the child to that of the parent will not
  work, ask to find out why?

- How do we set the start of the newly created thread to a new function? How do
  we pass arguments to that function?
  - Assume we only pass a single argument.

- Must add a new system call

- Must develop test cases

- Must show design document, pretty much answering these questions.

- Must develop plan of attack, including when test cases will be developed and
  how.
