#pragma once

// Communication package struct for sending file to compiler server.
typedef struct 
{
    int url_specified;      // Tells whether input file has been specified.
    char *url;              // Name of input file.
    int is_renamed;         // Tells whether option '-o' has been used to set name of file.
    char *name;             // Name of output file.
    int is_offline;         // Tells whether compilation happens offline.
} package;
