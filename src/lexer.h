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

} token_line;