#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "communication.h"

// Prototypes.
package parse_args(int argc, char **argv);

// Main function.
int main(int argc, char **argv)
{
    if (argc == 1)
    {
        printf("Input file not specified.\n");
        return 1;
    }
    
    const package data = parse_args(argc, argv);
    
    return 0;
}

// Parses input parameters and return package.
package parse_args(int argc, char **argv)
{
    package p = {.is_renamed = 0, .is_offline = 0, .url_specified = 0};
    unsigned i;

    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-o") == 0)
        {
            if (i == argc - 1)
            {
                printf("Missing naming argument.\n");
                exit(1);
            }
            
            p.name = (char *) malloc(strlen(argv[i + 1]));
            sprintf(p.name, "%s", argv[i + 1]);
            p.is_renamed = 1;
            i++;
        }

        else if (strcmp(argv[i], "-off") == 0)
            p.is_offline = 1;

        else if (strlen(argv[i]) > 0)
        {
            p.url_specified = 1;
            p.url = (char *) malloc(strlen(argv[i]));
            sprintf(p.url, "%s", argv[i]);
        }

        else
        {
            printf("Input file not specified.\n");
            exit(1);
        } 
    }

    if (!p.url_specified)
    {
        printf("Input file not specified.\n");
        exit(1);
    }

    return p;
}
