#ifndef COMPILER
#define COMPILER

#include "token.h"

enum error_t {ERROR, WARNING};

// General compiler prototypes.
lex_t read_token();
void reverse_token(lex_t last_read);
node parse();
node ast_link(node *restrict master, node *subs, unsigned count);
void type_check(node *root);

#endif