#ifndef _PRETTYPRINT_H
#define _PRETTYPRINT_H

// colors defs
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define pr_red(fmt, ...)                                                       \
  printf(ANSI_COLOR_RED fmt ANSI_COLOR_RESET, ##__VA_ARGS__)
#define pr_green(fmt, ...)                                                     \
  printf(ANSI_COLOR_GREEN fmt ANSI_COLOR_RESET, ##__VA_ARGS__)
#define pr_yellow(fmt, ...)                                                    \
  printf(ANSI_COLOR_YELLOW fmt ANSI_COLOR_RESET, ##__VA_ARGS__)
#define pr_blue(fmt, ...)                                                      \
  printf(ANSI_COLOR_BLUE fmt ANSI_COLOR_RESET, ##__VA_ARGS__)
#define pr_magenta(fmt, ...)                                                   \
  printf(ANSI_COLOR_MAGENTA fmt ANSI_COLOR_RESET, ##__VA_ARGS__)
#define pr_cyan(fmt, ...)                                                      \
  printf(ANSI_COLOR_CYAN fmt ANSI_COLOR_RESET, ##__VA_ARGS__)


// this doesn't belong here but don't want to create another header file
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr, type) sizeof(arr) / sizeof(type)
#endif

#endif /* prettyprint.h */
