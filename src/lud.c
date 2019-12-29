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

// TODO: Make use of main parameters instead.
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
    //print_tokens();
    print_ast(parse());

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

// Prints AST from parser.
void print_ast(node root)
{
    /*unsigned i;

    for (i = 0; i < root.children_count; i++)
    {
        printf("Type: %d\nData: %s\n\n", root.type, ((node *) root.children[i])->data);
        print_ast(*((node *) root.children[i]));
    }*/

    printf("Root children count: %d\n", root.children_count);
    printf("\nImport type: %d\nImport data: %s\n", ((node *) root.children[0])->type, ((node *) root.children[0])->data);
    printf("\nProtocol type: %d\nProtocol data: %s\n", ((node *) ((node *) root.children[1])->children[0])->type, ((node *) ((node *) root.children[1])->children[0])->data);
}