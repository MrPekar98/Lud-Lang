#define TOKENLENGTH 15

// Token struct.
typedef struct
{
    char key[TOKENLENGTH];
    char value[TOKENLENGTH];
} token;

// Stream representing a whole read line of tokens.
typedef struct
{
    unsigned short level;       // Scope level found by reading amount of spaces before the first character in a line.
} token_line;
