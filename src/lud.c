#include <stdio.h>
#include "token.h"

// Line number of compilation.
unsigned long line;

// .lud program file.
FILE *prog;

// External prototypes.


// Internal prototypes.
void gen_target();

// Main function.
int main()
{
    char input[200];
    
    printf(": ");
    scanf("%[A_Za-z. 0-9´'\"]", input);

    prog = fopen(input, "r");

    // Compile here.
    gen_target();

    fclose(prog);
    return 0;
}

// Generates target code.
void gen_target()
{
    // Get output AST from parser.
    // Run type checker on AST.
    // Optimise AST.
    // Run code generation on AST.
}
