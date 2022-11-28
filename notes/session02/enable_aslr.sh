#!/bin/bash

echo 1 | sudo tee /proc/sys/kernel/randomize_va_space
