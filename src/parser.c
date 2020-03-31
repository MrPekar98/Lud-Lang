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
static inline void add_child(node *parent, node *child);
static void make_import(node *parent);
static void check_import(const char *path);

// TODO: When parsing classes and protocols, write the signature into the data field of the node.

// Main parsing function.
node parse()
{
    node start = {.type = START};

    make_import(&start);

    return start;
}

// Adds child to parent node.
void add_child(node *parent, node child)
{
    if (parent->children_count == 0)
        parent->children = (void **) malloc(sizeof(node));

    else
        parent->children = (void **) realloc(parent->children, sizeof(node) * (parent->children_count + 1));

    parent->children[parent->children_count++] = &child;
}

// TODO: For some reason, the same data is written to all children.
// TODO: Start immediately parsing import files.
// Makes node for IMPORT.
static void make_import(node *parent)
{
    lex_t t = read_token();

    if (t.error)
        printf("Line %d: Illegal use of '%s' at this point.\n", line, t.lexeme);

    while ((token = read_token()).token == IMPORT_T)
    {
        lex_t path = read_token();

        if (path.token != LITERAL_T)
            printf("Line %d: Following an import must come a string literal specifying path.\n", line);

        check_import(path.lexeme);
        node n = {.type = IMPORTS};
        sprintf(n.data, "%s\0", path.lexeme);
        add_child(parent, n);
    }

    else
    {
        reverse_token(t);
        return;
    }

    t = read_token();
    reverse_token(t);
    line++;

        if ((token = read_token()).token != LITERAL_T)
            error("Following import statement comes string literal.");

        line++;
        check_import(token.lexeme);
        strcpy(child.data, token.lexeme);
        add_child(parent, &child);
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
            error("Import path may only contain letters, numbers, and dots.");
    }

    if (path[0] != '\"' || path[limit - 1] != '\"')
        error("Import path must be a string literal.");
}
