#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "util.h"

// External prototypes.
extern lex_t read_token();
extern void reverse_token(lex_t last_read);

// External line number variable.
extern unsigned long line;

// Prototypes.
static inline void add_child(node *parent, node child);
static inline void error(const char *msg);
static inline void warning(const char *msg);
static void make_import(node *parent);
static void check_import(const char *path);

// TODO: When parsing classes and protocols, write the signature into the data field of the node.

// Main parsing function.
node parse()
{
    node start = {.type = START, .children_count = 0};
    line = 0;
    
    make_import(&start);

    return start;
}

// Adds child to parent.
static inline void add_child(node *parent, node child)
{
    if (parent->children_count == 0)
        parent->children = (void **) malloc(sizeof(node));

    else
        parent->children = (void **) realloc(parent->children, sizeof(node) * (parent->children_count + 1));
    
    parent->children[parent->children_count++] = &child;
}

// Makes node for IMPORT.
static void make_import(node *parent)
{
    lex_t token;
    node child = {.type = IMPORTS, .children_count = 0};

    while ((token = read_token()).token == IMPORT_T)
    {
        if (token.error)
            error("Unrecognized token.");

        if ((token = read_token()).token != LITERAL_T)
            error("Following import statement comes string literal.");

        line++;
        sprintf(child.data, "%s\0", token.lexeme);
        add_child(parent, child);
    }

    reverse_token(token);
}

// Checks import literal for being valid.
static void check_import(const char *path)
{
    unsigned i, limit = strlen(path);

    for (i = 1; i < limit - 1; i++)
    {
        if ((path[i] < '0' || (path[i] > '9' && path[i] < 'A') || (path[i] > 'Z' && path[i] < 'a') || path[i] > 'z') && path[i] != '.')
            printf("Line %d: Import path may only contain letters, numbers, and dots.\n", line);
    }

    if (path[0] != '\"' || path[limit - 1] != '\"')
        printf("Line %d: Import path must be a string literal.\n", line);
}