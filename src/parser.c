#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "compiler.h"

// External line number variable.
extern unsigned long line;

// Prototypes.
static inline void add_child(node *parent, node child);
static void make_namespace(node *parent);
static void make_import(node *parent);
static void check_path(const char *path);
static inline char *replace_char(const char *str, char which, char new);
static inline short file_exists(const char *file);
static void make_program(node *parent);
static void make_protocoldecl(node *parent);
static void make_classdecl(node *parent);
static void make_statements(node *parent);

// TODO: When parsing classes and protocols, write the signature into the data field of the node.

// Main parsing function.
node parse()
{
    node start = init_node(START, 0);
    line = 0;
    
    make_namespace(&start);
    make_import(&start);
    make_program(&start);

    return start;
}

// TODO: Exchange the <Imports> nodes in master with <Start> nodes from subs.
// Links ASTs to master. This function must be used when all files have been parsed.
// The master AST is the last child, namely <Program>.
// The master is the file with the main function.
node ast_link(node *restrict master, node *subs, unsigned count)
{

}

// Constructor of node.
node init_node(enum rule type, size_t data_len)
{
    node n = {.type = type, .children_count = 0, .data = (char *) malloc(data_len)};
    strcpy(n.data, "");

    return n;
}

// TODO: <Imports> nodes are exchanged with <Start> nodes. Iterate these too!
// Destructor of AST.
void dispose_tree(node root)
{    
    unsigned i;

    for (i = 0; i < root.children_count; i++)
    {
        dispose_tree(*((node *) root.children[i]));
        free(root.children[i]);
    }

    free(root.data);
    free(root.children);
}

// TODO: Try to not use pointer for child.
// Adds child to parent.
static inline void add_child(node *parent, node child)
{
    if (parent->children_count == 0)
        parent->children = (void **) malloc(sizeof(void *));

    else
        parent->children = (void **) realloc(parent->children, sizeof(void *) * (parent->children_count + 1));
    
    parent->children[parent->children_count] = malloc(sizeof(node));
    memcpy(parent->children[parent->children_count++], &child, sizeof(node));
}

// Makes node for namespace.
static void make_namespace(node *parent)
{
    lex_t namespace = read_token();

    if (namespace.token != NAMESPACE_T)
    {
        reverse_token(namespace);
        return;
    }

    namespace = read_token();

    if (namespace.token != LITERAL_T)
        error("Expected string literal after namespace keyword.");

    node child = init_node(NAMESPACE, strlen(namespace.lexeme));
    strcpy(child.data, namespace.lexeme);
    add_child(parent, child);
    line += 2;
}

// Makes node for IMPORT.
static void make_import(node *parent)
{
    lex_t token;

    while ((token = read_token()).token == IMPORT_T)
    {
        if (token.error)
            error("Unrecognized token.");

        if ((token = read_token()).token != LITERAL_T)
            error("Following import statement comes string literal.");

        check_path(token.lexeme);

        node child = init_node(IMPORTS, strlen(token.lexeme));
        strcpy(child.data, token.lexeme);
        add_child(parent, child);
        line++;
    }

    reverse_token(token);
}

// Checks import literal for being valid. Checks also if file exists.
static void check_path(const char *path)
{
    unsigned i, limit = strlen(path);

    for (i = 1; i < limit - 1; i++)
    {
        if ((path[i] < '0' || (path[i] > '9' && path[i] < 'A') || (path[i] > 'Z' && path[i] < 'a') || path[i] > 'z') && path[i] != '.' && path[i] != '_')
            error("Import path may only contain letters, numbers, and dots.");
    }

    if (path[0] != '\"' || path[limit - 1] != '\"')
        error("Import path must be a string literal.");

    // TODO: Check file exists.
    /*char *file = (char *) malloc(strlen(path) - 2);
    strncpy(file, replace_char(path, '.', '/') + 1, strlen(path) - 2);
    
    if (!file_exists(file))
    {
        char msg[50];
        sprintf(msg, "File '%s' does not exist.\n", file);
        free(file);
        error(msg);
    }

    free(file);*/
}

// Replaces char in string with new char.
static inline char *replace_char(const char *str, char which, char new)
{
    unsigned i, length = strlen(str);
    char *new_str = (char *) malloc(strlen(str));
    strcpy(new_str, str);

    for (i = 0; i < length; i++)
    {
        if (str[i] == which)
            new_str[i] = new;
    }

    return new_str;
}

// Checks that file exists.
static inline short file_exists(const char *file)
{
    FILE *f = fopen(file, "r");
    
    if (f != NULL)
        fclose(f);
    
    return f != NULL;
}

// Makes node for PROGRAM.
static void make_program(node *parent)
{
    node child = init_node(PROGRAM, 0);
    lex_t token = read_token();

    if (token.token != CLASS_T && token.token != PROTOCOL_T)
        error("Expected class declaration or protocol declaration.");

    while (token.token == PROTOCOL_T || token.token == CLASS_T)
    {
        line += 2;
        reverse_token(token);
        
        if (token.token == CLASS_T)
            make_classdecl(&child);

        else if (token.token == PROTOCOL_T)
            make_protocoldecl(&child);

        token = read_token();
    }

    line += 2;
    add_child(parent, child);

    if (!token.error)
        warning("File should end here. Everything past this point will be ignored.");
}

// Makes node for PROTOCOLDECL.
// Data is on form: x->y, where x is id of protocol and y is id of inherited protocol.
static void make_protocoldecl(node *parent)
{
    node child = init_node(PROTOCOLDECL, 200);
    lex_t token = read_token();

    if (token.token != PROTOCOL_T)
        error("Expected keyword 'protocol' here.");

    if ((token = read_token()).token != ID_T)
        error("Expected identifier after 'prototcol'.");

    strcpy(child.data, token.lexeme);

    if ((token = read_token()).token == INHERITS)
    {
        if ((token = read_token()).token != ID_T)
            error("Expected protocol inheritance identifier.");

        sprintf(child.data, "%s->%s", child.data, token.lexeme);
    }

    else
        reverse_token(token);

    line++;

    if ((token = read_token()).token != LBRACE_T)
        error("Expected left curly brace of protocol body.");

    line++;
    make_statements(&child);

    if (read_token().token != RBRACE_T)
        error("Expected right curly brace of protocol body.");

    line += 2;
    add_child(parent, child);
}

// Makes node for CLASSDECL.
static void make_classdecl(node *parent)
{

}

// Makes node for STATEMENTS.
static void make_statements(node *parent)
{

}