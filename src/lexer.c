#include <stdio.h>
#include "token.h"

// External .lud program file.
extern FILE *prog;

// Reads next token.
lex_t read_token()
{
    return (lex_t) {.token = RPARAN};
}
