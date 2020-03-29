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
static inline void add_child(node *parent, node child);
static void make_import(node *parent);
static void make_program(node *parent);
static void check_import(const char *path);
static void make_protocoldecl(node *parent);
static void make_classdecl(node *parent);
static void make_statements(node *parent);

// TODO: Error messages must be on the form "Error/Warning on line x: msg", where x is a line number.

// Main parsing function.
node parse()
{
    node start = {.type = START};
    
    make_import(&start);
    // TODO: Everything is fine in add_child() and make_import(), but here it does not work.
    printf("Count: %d\nChild type: %d\n", start.children_count, ((node *) start.children[0])->type);
    make_program(&start);
    line = 0;

    return start;
}

// Adds child to parent.
static inline void add_child(node *parent, node child)
{
    if (parent->children == 0)
        parent->children = (void **) malloc(sizeof(node));

    else
        parent->children = (void **) realloc(parent->children, sizeof(node) * (parent->children_count + 1));
    
    parent->children[parent->children_count++] = &child;
}

// Makes node for IMPORT.
static void make_import(node *parent)
{
       
}