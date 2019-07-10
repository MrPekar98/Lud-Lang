#define LEX_LENGTH 30

// Enum for all tokens.
enum token_t {LPARAN, RPARAN, LBRACE, RBRACE};

// Token structure.
typedef struct
{
    char lexeme[LEX_LENGTH];
    enum token_t token;
} lex_t;
