#include <stdio.h>

// Throws error.
static inline void error(const char *msg)
{
    printf("Error on line %d: %s\n", line, msg);
    exit(1);
}

// Throws warning.
static inline void warning(const char *msg)
{
    printf("Warning on line %d: %s\n", line, msg);
}