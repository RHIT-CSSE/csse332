---
layout: post
title: The wfi Instruction
readtime: true
date: Thu Feb 2 2023 
---

# The `wfi` Instruction

In some cases, it might be useful for the operating system to turn off the CPUs
to conserve power, heating, and batter life (if applicable). Therefore,
instruction set architectures (ISA) such as RISC-V come equipped with the `wfi`
(wait-for-interrupt) instruction. `wfi` puts the CPU core on which it is
executing into a low power state until an interrupt is received. At that point,
the core is awakened and it can resume executing from where i left off. This is
very similar to the way condition variables work, but instead, we are dealing
with the physical hardware on the machine.

## Implementation

In order to implement this feature, we need to first be able to call an assembly
instruction from our kernel code implementation. Luckily, the compiler allows
you to do by using the `asm` directive as follows:

```c
static inline void
__wfi(void)
{
  asm volatile("wfi");
}
```

When you call the function `__wfi()` from the kernel code, the compiler will
replace the body of the function with the assembly instruction `wfi`.  Your job
in this step to figure out where and when you must call the `__wfi()` function
so that you turn off the CPU cores and fix the problems that we have identified
with the xv6 scheduler.

# Testing

To test if our implementation can solve the inefficiency problem in xv6, you can
make use of the `htop` utility to measure the CPU utilization when the xv6
kernel is running. If `htop` is not installed on your Linux machine, you can
install it using your favorite package manager. In the case of Ubuntu (or any
Debian-based distribution), you can do something like the following:

```shell
$ sudo apt install -y htop
```

After that, you can use `htop` to visualize the CPU utilization on all the
available cores in your system and verify if your implementation has solved the
inefficiency problem or not. 

