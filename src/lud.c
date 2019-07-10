#include <stdio.h>
#include "token.h"

// .lud program file.
FILE *prog;

// External prototypes
extern lex_t read_token();

// Internal prototypes

// Main function
int main(int argc, char **argv)
{
    char input[200];
    
    printf(": ");
    scanf("%[A_Za-z. 0-9´'\"]", input);

    prog = fopen(input, "r");

    // Compile here.

    fclose(prog);
    return 0;
}
