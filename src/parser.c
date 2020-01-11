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

// Main parsing function.
node parse()
{
    node start = {.type = START};
    
    make_import(&start);
    make_program(&start);
    line = 0;

    return start;
}

// Adds child to node.
static inline void add_child(node *parent, node child)
{
    if (parent->children == 0)
    {
        parent->children = (void **) malloc(sizeof(node));
        parent->children_count++;
    }

    else
        parent->children = (void **) realloc(parent->children, sizeof(node) * (parent->children_count + 1));
    
    parent->children[parent->children_count - 1] = &child;
    parent->children_count++;
}

// Makes node for IMPORT.
static void make_import(node *parent)
{
    lex_t t = read_token();
    
    if (t.error)
        printf("Line %d: Illegal use of '%s' at this point.\n", line, t.lexeme);

    else if (t.token == IMPORT_T)
    {
        lex_t path = read_token();
        
        if (path.token != LITERAL_T)
            printf("Line %d: Following an import must come a string literal specifying path.\n", line);

        check_import(path.lexeme);
        node n = {.type = IMPORTS};
        sprintf(n.data, "%s", path.lexeme);
        add_child(parent, n);
    }

    else
    {
        reverse_token(t);
        return;
    }
    
    t = read_token();
    reverse_token(t);
    line += 2;

    if (!t.error && t.token == IMPORT_T)
        make_import(parent);
}

// Checks format of import path.
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

// Makes node for PROGRAM.
static void make_program(node *parent)
{
    lex_t next = read_token();
    reverse_token(next);

    if (!next.error && next.token == PROTOCOL_T)
        make_protocoldecl(parent);

    else if (!next.error && next.token == CLASS_T)
        make_classdecl(parent);

    next = read_token();
    reverse_token(next);

    if (!next.error && (next.token == PROTOCOL_T || next.token == CLASS_T))
        make_program(parent);
}

// Makes node for PROTOCOLDECL.
static void make_protocoldecl(node *parent)
{
    lex_t t = read_token();
    node child = {.type = PROTOCOLDECL};

    if (t.token != PROTOCOL_T)
        printf("Line %d: Expected protocol declaration here.\n", line);

    t = read_token();

    if (t.token != ID_T)
        printf("Line %d: Expected identifier to protocol.\n", line);

    else
        sprintf(child.data, "%s", t.lexeme);

    t = read_token();

    if (t.token == INHERITS)
    {
        if ((t = read_token()).token != ID_T)
            printf("Line %d: Identifer must follow 'inherits' operator for protocol '%s'.\n", line, child.data);

        else
            sprintf(child.data, "%s->%s", child.data, t.lexeme);

        t = read_token();
    }

    add_child(parent, child);
    line += 2;

    if (t.token != LBRACE_T)
        printf("Line %d: Missing left curly brace for protocol '%s'.\nLexeme: %s\nToken: %d\n\n", line - 1, child.data, t.lexeme, t.token);

    make_statements(&child);

    if (read_token().token != RBRACE_T)
        printf("Line %d: Missing right curly brace for protocol '%s'.\n", line, child.data);

    line += 2;
    make_program(parent);
}

// Makes node for CLASSDECL.
static void make_classdecl(node *parent)
{
    lex_t t = read_token();
    node child = {.type = CLASSDECL};

    if (t.token != CLASS_T)
        printf("Line %d: Expected class declaration here.\n", line);

    t = read_token();

    if (t.token != ID_T)
        printf("Line %d: Expected identifier to protocol.\n", line);

    else
        sprintf(child.data, "%s", t.lexeme);

    t = read_token();

    if (t.token == INHERITS)
    {
        if ((t = read_token()).token != ID_T)
            printf("Line %d: Identifer must follow 'inherits' operator for class '%s'.\n", line, child.data);

        else
        {
            sprintf(child.data, "%s inherits %s", child.data, t.lexeme);
            t = read_token();
        }
    }

    if (t.token == IMPLEMENTS)
    {
        if ((t = read_token()).token != ID_T)
            printf("Line %d: Identifer must follow 'implements' operator for class '%s'.\n", line, child.data);

        else
            sprintf(child.data, "%s implements %s", child.data, t.lexeme);
        
        while ((t = read_token()).token == COMMA_T)
        {
            if ((t = read_token()).token != ID_T)
                printf("Line %d: Identifer must follow ',' for class '%s'.\n", line, child.data);

            else
                sprintf(child.data, "%s, %s", child.data, t.lexeme);
        }
    }

    add_child(parent, child);
    line += 2;
    
    if (t.token != LBRACE_T)
        printf("Line %d: Missing left curly brace for class '%s'.\n", line - 1, child.data);

    make_statements(&child);

    if (read_token().token != RBRACE_T)
        printf("Line %d: Missing right curly brace for class '%s'.\n", line, child.data);

    line += 2;
    make_program(parent);
}

// Makes node for STATEMENTS.
static void make_statements(node *parent)
{
    read_token();
    read_token();
    read_token();
    read_token();
    read_token();
    line++;
}
