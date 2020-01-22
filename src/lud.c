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
void print_ast();

// Main function.
int main(int argc, char **argv)
{
    if (argc <= 1)
    {
        printf("Missing input file.\n");
        return 1;
    }

    prog = fopen(argv[1], "r");

    // Compile here.
#if !defined (DEBUG1) && (DEBUG2)
    gen_target();
#endif

    // Testing.
#if defined (DEBUG1)
    print_tokens();
#elif defined (DEBUG2)
    print_ast(parse());
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

extern void reverse_token(lex_t last_read);

// Prints all tokens and lexemes.
void print_tokens()
{
    lex_t lex;

    while ((lex = read_token()).token != -1)
    {
        printf("Token: %d\nLexeme: %s\n\n", lex.token, lex.lexeme);
    }
}

// TODO: Test parsing of protocols with inheritance and without. Do the same with classes with and without inheritance and polymorphism.
// Prints AST from parser.
void print_ast(node n)
{    
    printf("Children: %d\nType: %d\nData: %s\n\n", n.children_count, n.type, n.data);

    if (n.children_count > 0)
    {
        unsigned i;

        for (i = 0; i < n.children_count; i++)
        {
            print_ast(*((node *) n.children[i]));
        }
    }
}