---
layout: post
title: Simple Shell Grading Information
readtime: true
date: Thu Nov  3 22:29:11 2022 
---

# Rubric

|                         | Points | Notes                                                                                |
|:------------------------|--------|--------------------------------------------------------------------------------------|
| S1: Foreground Commands | 25     | check arguments with donothing<br>should use exec<br>should not quit after first run |
| S2: Repeated Commands   | 15     | check that processes are created according to given number<br>and all are waited on. |
| S3: Background Commands | 25     |                                                                                      |
| S4: Finish notification | 25     | check for use of exec and proper exit of printing process (e.g. common probs)        |
| S5: Zombies             | 10     | be sure your run ps outside of simple shell                                          |
