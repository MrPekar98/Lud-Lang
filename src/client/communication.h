#pragma once

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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
    int update;             // Request to get updated compiler executable.
} package;

// Returns buffer of package.
// Format: First 8 bytes tell size of buffer, second 4 bytes tell size of name, third byte tells request for update,
// fourth byte tells whether output file is renamed, fifth comes name of output as a string, sixth byte tells what to output (just compile),
// and seventh is source code as a string.
void *tobuffer(package p)
{
    if (!p.url_specified)
        return NULL;

    long buffer_size = strlen(p.name) + strlen(p.source) + 15;
    int name_size = strlen(p.name);
    void *buffer = malloc(buffer_size);
    char renamed = p.is_renamed, just_compile = p.just_compile, update = p.update;

    memcpy(buffer, &buffer_size, 8);
    memcpy(buffer + 8, &name_size, 4);
    memcpy(buffer + 12, &update, 1);
    memcpy(buffer + 13, &renamed, 1);
    memcpy(buffer + 14, &just_compile, 1);
    memcpy(buffer + 15, p.name, strlen(p.name));
    memcpy(buffer + strlen(p.name) + 15, p.source, strlen(p.source));

    return buffer;
}

// Converts buffer from package into package.
package topackage(void *buffer)
{
    package p = {.is_renamed = 0};
    long buffer_size = *((long *) buffer);
    int name_size = *((int *) (buffer + 8));
    char renamed = *((char *) (buffer + 13));

    if ((int) renamed)
    {
        p.is_renamed = 1;
        p.name = (char *) malloc(name_size);
        memcpy(p.name, buffer + 15, name_size);
        p.name[name_size] = '\0';
    }

    p.update = (int) *((char *) buffer + 12);
    p.just_compile = (int) *((char *) buffer + 14);
    p.source = (char *) malloc(buffer_size - name_size - 15);
    memcpy(p.source, buffer + 15 + name_size, buffer_size - name_size - 15);

    return p;
}
