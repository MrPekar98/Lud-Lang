#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define TOKENLENGTH 15

// Table of tokens.
enum token_t {CLASS, PROTOCOL, LPARAN, RPARAN, NUMBERLIT, BOOLLIT, STRINGLIT, VAR, PLUS, MINUS, TIMES, DIVISOR, MODULUS};

// Editor indentation.
static unsigned indentation = 4;

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
    token *tokenstream;         // Stream of tokens.
    size_t tokens;              // Amount of tokens in stream.
} token_line;

// Prototypes
token_line read_tokenline(FILE *file);
static inline unsigned amountof_tokens(const char *str);
token rec_token(const char *read);

// Setter for indentation to determine scope leves.
void setindent(token_line *stream, unsigned value)
{
    indentation = value;
}

// Returns stream of tokens for a whole line.
token_line read_tokenline(FILE *file)
{
    char space_str[TOKENLENGTH * 4];
    fscanf(file, "%[ ]", space_str);
    
    token_line line = {.level = strlen(space_str) / indentation};
    char c, *read_str = (char *) malloc(sizeof(char) * TOKENLENGTH * 10);

    // Indentation error check.
    if (strlen(space_str) % indentation != 0)
    {
        fscanf(file, "%s", read_str);
        printf("Indentation fault before: '%s'\n", read_str);
        exit(1);
    }

    fscanf(file, " %[ A-Za-z_0-9]", read_str);
    line.tokens = amountof_tokens(read_str);
    unsigned i, old = 0, counter = 0, length = strlen(read_str);
    line.tokenstream = (token *) malloc(sizeof(token) * line.tokens);
    char temp[TOKENLENGTH * 3];

    for (i = 0; i < length; i++)
    {
        if (i == length - 1)
        {
            sprintf(temp, "%s", read_str);
            line.tokenstream[counter] = rec_token(temp + old);
            break;
        }

        else if (read_str[i] == ' ')
        {
            sprintf(temp, "%s", read_str);
            temp[i] = '\0';
            line.tokenstream[counter++] = rec_token(temp + old);
            old = i + 1;
        }
    }

    free(read_str);

    return line;
}

// Returns amount of token in a line.
static inline unsigned amountof_tokens(const char *str)
{
    unsigned i, counter = 0, length = strlen(str);

    for (i = 0; i < length; i++)
    {
        if (str[i] == ' ')
            counter++;
    }

    return counter + 1;
}

// Recognizes token and returns token struct from a string.
token rec_token(const char *read)
{
    return (token) {};
}
