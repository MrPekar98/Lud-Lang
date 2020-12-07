#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "compiler.h"

// External line number variable.
extern unsigned long line;

// Prototypes.
static inline void parse_check(lex_t read, enum token_t token, const char *msg, enum error_t err_war);
static inline void add_child(node *parent, const node *child);
static void make_namespace(node *parent);
static void make_import(node *parent);
static void check_path(const char *path);
static inline char *replace_char(const char *str, char which, char new);
static inline short file_exists(const char *file);
static void make_program(node *parent);
static void make_protocoldecl(node *parent);
static void make_classdecl(node *parent);
static void make_classprotaccessors(node *parent);
static void make_members(node *parent);
static void make_constructor(node *parent);
static void make_destructor(node *parent);
static void make_methoddecl(node *parent);
static void parse_parameters(node *method);
static void make_vardecl(node *parent);
static void make_var(node *parent);
static void make_datatype(node *parent);
static void make_statements(node *parent);
static void make_expression(node *parent);

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

// Check function for lexed token matching argument token.
static inline void parse_check(lex_t read, enum token_t token, const char *msg, enum error_t err_war)
{
    if (read.token != token)
    {
        if (err_war == ERROR)
            error(msg);

        else
            warning(msg);
    }
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
    return (node) {.type = type, .children_count = 0, .data = (char *) malloc(data_len)};
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

// TODO: node should be a pointer.
// Adds child to parent.
static inline void add_child(node *parent, const node *child)
{
    if (parent->children_count == 0)
        parent->children = (void **) malloc(sizeof(node));

    else
        parent->children = (void **) realloc(parent->children, sizeof(node) * (parent->children_count + 1));

    parent->children[parent->children_count] = malloc(sizeof(node));
    memcpy(parent->children[parent->children_count++], child, sizeof(node));
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
    parse_check(namespace, LITERAL_T, "Expected string literal after namespace keyword.", ERROR);

    node child = init_node(NAMESPACE, strlen(namespace.lexeme));
    strcpy(child.data, namespace.lexeme);
    add_child(parent, &child);
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

        parse_check(token = read_token(), LITERAL_T, "Following import statement comes string literal.", ERROR);
        check_path(token.lexeme);

        node child = init_node(IMPORTS, strlen(token.lexeme));
        strcpy(child.data, token.lexeme);
        add_child(parent, &child);
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
    add_child(parent, &child);

    if (!token.error)
        warning("File should end here. Everything past this point will be ignored.");
}

// Makes node for PROTOCOLDECL.
// Data is on form: x->y, where x is id of protocol and y is id of inherited protocol.
static void make_protocoldecl(node *parent)
{
    node child = init_node(PROTOCOLDECL, 200);
    lex_t token = read_token();

    parse_check(token, PROTOCOL_T, "Expected keyword 'protocol' here.", ERROR);
    parse_check(token = read_token(), ID_T, "Expected identifier after 'prototcol'.", ERROR);
    strcpy(child.data, token.lexeme);

    if ((token = read_token()).token == INHERITS)
    {
        parse_check(token = read_token(), ID_T, "Expected protocol inheritance identifier.", ERROR);
        sprintf(child.data, "%s->%s", child.data, token.lexeme);
    }

    else
        reverse_token(token);

    line++;
    parse_check(token = read_token(), LBRACE_T, "Expected left curly brace of protocol body.", ERROR);
    line++;

    make_classprotaccessors(&child);
    parse_check(read_token(), RBRACE_T, "Expected right curly brace of protocol body.", ERROR);

    line += 2;
    add_child(parent, &child);
}

// Makes node for CLASSDECL.
// Data is on form: x->y:i,j,k, where x is id of class, y is id of inherited class, and i, j, and k are protocols.
static void make_classdecl(node *parent)
{
    node child = init_node(CLASSDECL, 200);
    lex_t token = read_token();

    parse_check(token, CLASS_T, "Expected keyword 'class' here.", ERROR);
    parse_check(token = read_token(), ID_T, "Expected identifier after 'class'.", ERROR);
    strcpy(child.data, token.lexeme);

    if ((token = read_token()).token == INHERITS)
    {
        parse_check(token = read_token(), ID_T, "Expected class inheritance identifier.", ERROR);
        sprintf(child.data, "%s->%s", child.data, token.lexeme);
    }

    if (token.token == IMPLEMENTS)
    {
        parse_check(token = read_token(), ID_T, "Expected class polymorphism identifier.", ERROR);
        sprintf(child.data, "%s:%s", child.data, token.lexeme);

        while ((token = read_token()).token == COMMA_T)
        {
            parse_check(token = read_token(), ID_T, "Expected class polymorphism identifier.", ERROR);
            sprintf(child.data, "%s,%s", child.data, token.lexeme);
        }

        reverse_token(token);
    }

    line++;
    parse_check(token = read_token(), LBRACE_T, "Expected left curly brace of class body.", ERROR);
    line++;

    make_classprotaccessors(&child);
    parse_check(read_token(), RBRACE_T, "Expected right curly brace of class body.", ERROR);

    line += 2;
    add_child(parent, &child);
}

// Makes node for CLASSPROTACCESSORS.
static void make_classprotaccessors(node *parent)
{
    node child = init_node(CLASSPROTSTATEMENTS, 0);
    lex_t token;

    while ((token = read_token()).token == ACCESSOR_T)
    {
        make_members(&child);
        line++;
    }

    reverse_token(token);
    add_child(parent, &child);
}

// Makes node for MEMBERS.
static void make_members(node *parent)
{
    lex_t token = read_token();
    node child = init_node(MEMBERS, 0);

    switch (token.token)
    {
        case CONSTRUCTOR_T:
            reverse_token(token);
            make_constructor(&child);
            break;

        case DESTRUCTOR_T:
        reverse_token(token);
            make_destructor(&child);
            break;

        case ABSTRACT_T:
            reverse_token(token);
            make_methoddecl(&child);
            break;

        case CONST_T: case VOLATILE_T:
            reverse_token(token);
            make_vardecl(&child);
            break;

        case STATIC_T:
            reverse_token(token);
            // TODO: Figure out whether we reached a <VarDecl> or <MethodDecl>.
            break;

        default:
            error("Expected variable, method, constructor or destructor declaration.");
    }

    add_child(parent, &child);
}

// Makes node for CONSTRUCTOR.
static void make_constructor(node *parent)
{
    node child = init_node(CONSTRUCTOR, 0);
    lex_t token;

    parse_check(read_token(), CONSTRUCTOR_T, "Expected 'constructor'.", ERROR);
    parse_check(read_token(), LPARAN_T, "Expected left parenthesis following 'constructor' keyword.", ERROR);
    parse_parameters(&child);
    parse_check(read_token(), RPARAN_T,"Expected right parenthesis as end of constructor parameter list.", ERROR);
    line++;

    parse_check(read_token(), LBRACE_T, "Expected left curly brace for constructor body.", ERROR);
    line++;
    make_statements(&child);
    line++;
    parse_check(read_token(), RBRACE_T, "Expected right curly brace for contructor body.", ERROR);

    line++;
    add_child(parent, &child);
}

// Parses parameters in constructor, destructor, and methods.
static void parse_parameters(node *method)
{
    lex_t token;

    if ((token = read_token()).token == ID_T)
    {
        reverse_token(token);
        make_var(method);
        parse_check(read_token(), COLON_T, "Expected ':' to annotate datatype to parameter.", ERROR);
        make_datatype(method);
    }

    else
    {
        reverse_token(token);
        return;
    }

    while ((token = read_token()).token == COMMA_T)
    {
        make_var(method);
        parse_check(read_token(), COLON_T, "Expected ':' to annotate datatype to parameter.", ERROR);
        make_datatype(method);
    }

    reverse_token(token);
}

// Makes node for DESTRUCTOR.
static void make_destructor(node *parent)
{
    node child = init_node(DESTRUCTOR, 0);
    
    parse_check(read_token(), DESTRUCTOR_T, "Expected 'destructor'.", ERROR);
    parse_check(read_token(), LPARAN_T, "Expected left parenthesis following 'destructor' keyword.", ERROR);
    parse_parameters(&child);
    parse_check(read_token(), RPARAN_T, "Expected right parenthesis as end of destructor parameter list.", ERROR);

    line++;
    parse_check(read_token(), LBRACE_T, "Expected left curly brace for destructor body.", ERROR);
    line++;
    make_statements(&child);
    line++;
    parse_check(read_token(), RBRACE_T, "Expected right curly brace for destructor body.", ERROR);

    line++;
    add_child(parent, &child);
}

// Makes node for METHODDECL.
// Data field contains modifiers on the form 'x,y'.
static void make_methoddecl(node *parent)
{
    lex_t token = read_token();
    node child = init_node(METHODDECL, 200);
    short is_void = 0;

    if (token.token == ABSTRACT_T)
    {
        strcpy(child.data, token.lexeme);
        token = read_token();
    }

    if (token.token == STATIC_T)
    {
        sprintf(child.data, "%s,%s", child.data, token.lexeme);
        token = read_token();
    }

    parse_check(token, ID_T, "Expected method identifier.", ERROR);
    reverse_token(token);
    make_var(&child);
    parse_check(read_token(), LPARAN_T, "Expected left parenthesis following method identifier.", ERROR);
    parse_parameters(&child);
    parse_check(read_token(), RPARAN_T, "Expected right parenthesis following method parameter list.", ERROR);
    parse_check(read_token(), COLON_T, "Expected colon following method parameter list.", ERROR);

    if ((token = read_token()).token == DATATYPE_T && strcmp(token.lexeme, "void") == 0)
    {
        node datatype = init_node(DATATYPES, strlen(token.lexeme));
        strcpy(datatype.data, token.lexeme);
        add_child(&child, &datatype);
        is_void = 1;
    }

    parse_check(token, DATATYPE_T, "Expected datatype following ':' for method.", ERROR);
    reverse_token(token);
    make_datatype(&child);
    line++;

    parse_check(read_token(), LBRACE_T, "Expected left curly brace for method body.", ERROR);
    line++;
    make_statements(&child);
    line++;

    // TODO: Remember to add child here.
    if (!is_void)
    {
        parse_check(read_token(), RETURN_T, "Expected return statement.", ERROR);
        make_expression(&child);
    }

    parse_check(read_token(), RBRACE_T, "Expected right curly brace for method body.", ERROR);
    add_child(parent, &child);
    line += 2;
}

// TODO: Maybe add identifier and datatype in data field.
// Makes node for VARDECL.
static void make_vardecl(node *parent)
{

}

// TODO: Error when indexing array.
// Makes node for VARIABLE.
static void make_var(node *parent)
{
    lex_t token = read_token();
    node child = init_node(VARABLE, strlen(token.lexeme));

    parse_check(token, ID_T, "Expected variable identifier.", ERROR);
    strcpy(child.data, token.lexeme);
    add_child(parent, &child);
}

// Makes node for DATATYPES.
static void make_datatype(node *parent)
{
    lex_t token = read_token();
    node child = init_node(DATATYPES, strlen(token.lexeme));

    parse_check(token, DATATYPE_T, "Expected datatype.", ERROR);
    strcpy(child.data, token.lexeme);
    add_child(parent, &child);
}

// TODO: Add statements in tree for type checker to check upon.
// Makes node for STATEMENTS.
static void make_statements(node *parent)
{

}

// TODO: Must make children for every rule (because of type checker). This can be optimized in a latter stage.
// Makes node for EXPRESSION.
static void make_expression(node *parent)
{

}
