#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"

// External prototypes.
extern lex_t read_token();
extern void reverse_token(lex_t last_read);

// External line number variable.
extern unsigned long line;

// Prototypes.
static void add_child(node *parent, node child);
static void make_import(node *parent);
static void make_program(node *parent);
static void check_import(const char *path);
static void make_protocoldecl(node *parent);
static void make_classdecl(node *parent);

// Main parsing function.
node parse()
{
    node start = {.type = START};
    
    make_import(&start);
    make_program(&start);

    return start;
}

// Adds child to node.
static void add_child(node *parent, node child)
{
    parent->children = (void **) realloc(parent->children, sizeof(void *) * ++parent->children_count);
    parent->children[parent->children_count - 1] = &child;
}

// Makes node for IMPORT.
static void make_import(node *parent)
{
    lex_t t = read_token();
    
    if (t.error)
        printf("Line %d: Illegal use of '%s' at this point.\n", line, t.lexeme);

    else if (t.token == IMPORT)
    {
        lex_t path = read_token();
        
        if (path.token != LITERAL)
            printf("Line %d: Following an import must come a string literal specifying path.\n", line);

        check_import(path.lexeme);
        node n = {.type = IMPORTS};
        sprintf(n.data, "%s", path.lexeme);
        add_child(parent, n);
    }
    
    t = read_token();
    reverse_token(t);

    if (!t.error && t.token == IMPORT)
    {
        line++;
        make_import(parent);
    }
}

// Checks format of import path.
static void check_import(const char *path)
{
    unsigned i, limit = strlen(path);

    for (i = 1; i < limit - 1; i++)
    {
        if ((path[i] < '0' || (path[i] > '9' && path[i] < 'A') || (path[i] > 'Z' && path[i] < 'a') || path[i] > 'z') && path[i] != '.')
            printf("Line %d: Path may only contain letters, numbers, and dot.\n", line);
    }

    if (path[0] != '\"' || path[limit - 1] != '\"')
        printf("Line %d: Path must be a string literal.\n", line);
}

// Makes node for PROGRAM.
static void make_program(node *parent)
{
    lex_t next = read_token();
    node n = {.type = PROGRAM};
    add_child(parent, n);

    if (!next.error && next.token == PROTOCOL)
    {
        reverse_token(next);
        make_protocoldecl(&n);
    }

    else if (!next.error && next.token == CLASS)
    {
        reverse_token(next);
        make_classdecl(&n);
    }

    next = read_token();

    if (!next.error && (next.token == PROTOCOL || next.token == CLASS))
    {
        reverse_token(next);
        make_program(parent);
    }
}

// Makes node for PROTOCOLDECL.
static void make_protocoldecl(node *parent)
{

}

// Makes node for CLASSDECL
static void make_classdecl(node *parent)
{

}
