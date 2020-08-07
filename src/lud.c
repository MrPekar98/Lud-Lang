#include <stdio.h>
#include <time.h>
#include "token.h"
#include "compiler.h"

// Line number of compilation.
unsigned long line = 1;

// .lud program file.
FILE *prog;

// Prototypes.
void gen_target();
void print_tokens();
void print_ast(node n);

// Main function.
int main(int argc, char **argv)
{
    clock_t start = clock();
    
    if (argc <= 1)
    {
        printf("Missing input file.\n");
        return 1;
    }

    prog = fopen(argv[1], "r");

    // Testing.
#if defined (DEBUG1)
    print_tokens();

#elif defined(DEBUG2)
    print_ast(parse());
#else   // Otherwise, compile.
    gen_target();
    printf("Compiled in %.3f seconds.\n", (double) (clock() - start) / CLOCKS_PER_SEC);
#endif

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

// Prints all tokens and lexemes.
void print_tokens()
{
    lex_t lex;

    while ((lex = read_token()).token != -1)
    {
        printf("Token: %d\nLexeme: %s\n\n", lex.token, lex.lexeme);
    }
}

// Prints AST from parser.
void print_ast(node n)
{
    printf("Type: %d\nChilren: %d\nData: %s\n\n", n.type, n.children_count, n.data);

    if (n.children_count > 0)
    {
        unsigned i;

        for (i = 0; i < n.children_count; i++)
        {
            print_ast(*((node *) n.children[i]));
        }
    }
}