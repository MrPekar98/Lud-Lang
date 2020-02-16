#ifndef PARSER_H
#define PARSER_H

// Specifies a node type.
enum rule {START, VARABLE, IMPORTS, PROGRAM, PROTOCOLDECL, CLASSDECL, STATEMENTS, STATEMENT, SYSTEMCALL,
			VARDEC, EXPRESSION, EQUALS, ARITHEXPRESSION, BOOLXOR, BOOLAND, ARITHNUMBERSTRING, PLUSMINUS,
			MULTDIV, IFSTATEMENT, ELSE, FOR_EACH, WHILELOOP, FORLOOP, METHODDECL, CONSTRUC, CALL};

// AST node.
typedef struct
{
    enum rule type;
    void **children;
    unsigned children_count;
    char data[200];
    char modification[200];     // Might not be necessary. Can be used for optimization of AST.
} node;

// Prototype for adding a child to a node.
void add_child(node *parent, node child);
node parse();

#endif
