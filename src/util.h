#ifndef UTIL
#define UTIL

#include <stdio.h>

extern unsigned long line;

// Throws error.
void error(const char *msg)
{
    printf("Error on line %d: %s\n", line, msg);
    exit(1);
}

// Throws warning.
void warning(const char *msg)
{
    printf("Warning on line %d: %s\n", line, msg);
}

#endif