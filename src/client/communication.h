#pragma once

#include <string.h>
#include <stdlib.h>

// Communication package struct for sending file to compiler server.
typedef struct 
{
    int url_specified;      // Tells whether input file has been specified.
    char *url;              // Name of input file.
    int is_renamed;         // Tells whether option '-o' has been used to set name of file.
    char *name;             // Name of output file.
    int is_offline;         // Tells whether compilation happens offline.
    int just_compile;       // Tells whether to return target code or executable.
    char *source;           // All text to be compiled.
} package;

// Returns buffer of package.
// Format: First byte tells whether output file is renamed, second comes name of output as a string, third byte tells what to output (just compile), and fourth is source code as a string.
void *tobuffer(package p)
{
    if (!p.url_specified)
        return NULL;

    void *buffer = malloc(strlen(p.url) + strlen(p.name) + 2);
    char renamed = p.is_renamed ? (char) 1 : (char) 0, just_compile = p.just_compile ? (char) 1 : (char) 0;

    memcpy(buffer, renamed, 1);
    memcpy(buffer + 1, p.name, strlen(p.name));
    memcpy(buffer + strlen(p.name) + 1, just_compile, 1);
    memcpy(buffer + strlen(p.name) + 2, p.source);

    return buffer;
}