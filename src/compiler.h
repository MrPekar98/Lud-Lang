#ifndef COMPILER
#define COMPILER

// General compiler prototypes.
lex_t read_token();
void reverse_token(lex_t last_read);
node parse();
void type_check(node *root);

#endif