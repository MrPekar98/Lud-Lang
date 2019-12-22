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
// Format: First 8 bytes tell size of buffer, second 4 bytes tell size of name, third byte tells whether output file is renamed, fourth comes name of output as a string, fifth byte tells what to output (just compile), and sixth is source code as a string.
// TODO: Make sure at least one byte is send as name. The same goes for the source code.
void *tobuffer(package p)
{
    if (!p.url_specified)
        return NULL;

    void *buffer = malloc(strlen(p.url) + strlen(p.name) + 2);
    char renamed = p.is_renamed ? (char) 1 : (char) 0, just_compile = p.just_compile ? (char) 1 : (char) 0;
    long buffer_size = strlen(p.name) + strlen(p.source) + 10;
    int name_size = strlen(p.name);

    memcpy(buffer, &buffer_size, 8);
    memcpy(buffer + 8, name_size, 4);
    memcpy(buffer + 12, &renamed, 1);
    memcpy(buffer + 13, p.name, strlen(p.name));
    memcpy(buffer + strlen(p.name) + 13, just_compile, 1);
    memcpy(buffer + strlen(p.name) + 14, p.source, strlen(p.source));

    return buffer;
}

// Converts buffer from package into package.
package topackage(void *buffer)
{
    package p;
    long buffer_size = *((long *) buffer);
    int name_size = *((int *) buffer + 8);
    char renamed = *((char *) buffer + 12), just_compile = *((char *) buffer + 12 + name_size);

    if ((int) renamed)
    {
        p.is_renamed;
        p.name = (char *) malloc(name_size);
        memcpy(p.name, buffer + 12, name_size);
    }

    if ((int) just_compile)
        p.just_compile = 1;

    // TODO: Continue from copying source code data.
}