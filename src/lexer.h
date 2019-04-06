#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define TOKENLENGTH 15

// Table of tokens.
enum token_t = {};

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
    unsigned indent;            // Indentation size in editor.
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
    stream->indent = value;
}

// Returns stream of tokens for a whole line.
token_line read_tokenline(FILE *file)
{
    char space_str[TOKENLENGTH * 4];
    fscanf(file, "%[ ]", spaces_str;
    
    unsigned spaces = strlen(space_str);
    char c, read_char[TOKENLENGTH];
    int start_found = 0, counter = 0;

    /*while ((c = fgetc(file)) != '\n')
    {
        if (!start_found && c == ' ')
            spaces++;

        else if (!start_found && c != ' ')
        {
            start_found = 1;
            read_char[counter++] = c;
        }

        else
            read_char[counter++] = c;
    }*/
}

// Returns amount of token in a line.
static inline unsigned amountof_tokens(const char *str)
{

}

// Recognizes token and returns token struct from a string.
token rec_token(const char *read)
{

}
