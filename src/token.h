// Must be compiled with parser.c.

#ifndef TOKEN_H
#define TOKEN_H

#include <stdlib.h>
#define LEX_LENGTH 50

// Enum for all tokens.
enum token_t {LPARAN_T, RPARAN_T, LBRACE_T, RBRACE_T, IMPORT_T, ID_T, PROTOCOL_T, CLASS_T, INHERITS, RBRACKET_T, LBRACKET_T, IMPLEMENTS, COLON_T, SEMICOLON_T, 
                SYSTEM_T, ACCESSOR_T, STATIC_T, CONST_T, EQ_T, DOT_T, COMMA_T, THIS_T, SUPER_T, CREATE_T, OR_T, AND_T, XOR_T, OPERATOR_T, MODULUS_T, IF_T, 
                ELSE_T, FOR_T, WHILE_T, FOREACH_T, METHOD_T, CONSTRUCTOR_T, RETURN_T, NOT_T, BREAK_T, CONTINUE_T, LITERAL_T, DATATYPE_T};

// Specifies a node type.
enum rule {START, VARABLE, IMPORTS, PROGRAM, PROTOCOLDECL, CLASSDECL, STATEMENTS, STATEMENT, SYSTEMCALL, VARDECL, ACCESSORS, DATATYPES, LITERALS, ASSIGNMENT, 
                EXPRESSION, EQUALS, ARITHEXPRESSION, BOOLXOR, BOOLAND, ARITHNUMBERSTRING, PLUSMINUS, MULTDIV, ARITHPOW, ARITHPARAN, ARRAYLITERAL, REFERENCE, 
                IFSTATEMENT, ELSE, FOR_EACH, WHILELOOP, FORLOOP, METHODDECL, CONSTRUC, CALL};

// Token structure. An error occured when token is -1.
typedef struct
{
    char lexeme[LEX_LENGTH];
    enum token_t token;
    int error;
} lex_t;

// AST node.
typedef struct
{
    enum rule type;
    void **children;
    unsigned children_count;
    char *data;
    char *modification;     // Might not be necessary.
} node;

// Constructor of node.
node init_node(enum rule type, size_t data_len);
void dispose_tree(node root);   // TODO: Implement function in code generation file.

#endif
