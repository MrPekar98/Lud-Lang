#include <stdio.h>
#include "token.h"

// Line number of compilation.
unsigned long line = 1;

// .lud program file.
FILE *prog;

// External prototypes.
extern lex_t read_token();
extern node parse();
extern void type_check(node *root);

// Internal prototypes.
void gen_target();
void print_tokens();

// Main function.
int main()
{
    char input[200];

    printf(": ");
    scanf("%[A_Za-z. 0-9´'\"/]", input);

    prog = fopen(input, "r");

    // Compile here.
    //gen_target();

    // Testing.
    print_tokens();

    fclose(prog);
    return 0;
}

// Generates target code.
void gen_target()
{
    // 1. Get output AST from parser.
    // 2. Run type checker on AST.
    // 3. Optimise AST.
    // 4. Run code generation on AST.

    node ast = parse();
	type_check(&ast);
}

// Printf all tokens and lexemes.
// For testing purposes only!
void print_tokens()
{
    lex_t lex;

    while ((lex = read_token()).token != -1)
    {
        printf("Token: %d\nLexeme: %s\n\n", lex.token, lex.lexeme);
    }
}
