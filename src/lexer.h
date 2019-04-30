#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define TOKENLENGTH 15

// Table of tokens.
enum token_t {CLASS, PROTOCOL, LPARAN, RPARAN, ASSIGN, PLUS, MINUS, TIMES, DIVISOR, MODULUS, NUMBERLIT, BOOLLIT, STRINGLIT, ID};

// Editor indentation.
static unsigned indentation = 4;

// Token struct.
typedef struct
{
    enum token_t key;
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
static inline token rec_token(const char *read);
char *substring(unsigned from, unsigned to, const char *str);
int isnumberlit(const char *str);
int isboollit(const char *str);
int isstringlit(const char *str);
int isid(const char *str);
int iswhite_space(const char* str);

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
    if (strlen(space_str) % indentation != 0 && strlen(space_str) > 1)
    {
        fscanf(file, "%s", read_str);
        printf("Indentation fault before: '%s'\n", read_str);
        exit(1);
    }

    // TODO: Read other necessary characters.
    // Reads until a line containing other characters than spaces is found.
    do
    {
        fscanf(file, "%[A-Za-z_=+-*/()\",. 0-9]\n", read_str);
    }
    while (iswhite_space(read_str) && !feof(file));

    line.tokens = amountof_tokens(read_str);
    unsigned i, old = 0, counter = 0, length = strlen(read_str);
    line.tokenstream = (token *) malloc(sizeof(token) * line.tokens);
    char temp[TOKENLENGTH * 10];

    for (i = 0; i < length; i++)
    {
        // When token is a string.
        if (read_str[i] == '"')
        {
            unsigned start = i++;
            
            for (; i < length; i++)
            {
                if (read_str[i] == '"')
                    break;
            }

            // TODO: Counter counts way to high.
            if (i = length - 1 || read_str[i + 1] == ' ')
            {
                strcpy(temp, substring(start, ++i, read_str));
                line.tokenstream[counter++] = rec_token(temp);
                old = ++i;
            }

            // Error.
            else
            {
                printf("Missing space in following string:\n'%s'\n", read_str);
                exit(1);
            }
            
        }
        
        else if (i == length - 1)
        {
            strcpy(temp, substring(old, length - 1, read_str));
            line.tokenstream[counter] = rec_token(temp);
            break;
        }

        else if (read_str[i] == ' ')
        {
            strcpy(temp, substring(old, i - 1, read_str));
            line.tokenstream[counter++] = rec_token(temp);
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
        if (str[i] == '"')
        {
            i++;
            
            for (; i < length; i++)
            {
                if (str[i] == '"')
                    break;
            }

            counter++;
        }
        
        else if (str[i] == ' ')
            counter++;
    }

    return counter + 1;
}

// Recognizes token string and returns corresponding token struct.
static inline token rec_token(const char *read)
{
    token t;
    strcpy(t.value, read);
    
    if (strcmp(read, "class") == 0)
        t.key = CLASS;

    else if (strcmp(read, "protocol") == 0)
        t.key = PROTOCOL;

    else if (strcmp(read, "(") == 0)
        t.key = LPARAN;
    
    else if (strcmp(read, ")") == 0)
        t.key = RPARAN;

    else if (strcmp(read, "=") == 0)
        t.key = ASSIGN;

    else if (strcmp(read, "+") == 0)
        t.key = PLUS;

    else if (strcmp(read, "-") == 0)
        t.key = MINUS;

    else if (strcmp(read, "*") == 0)
        t.key = TIMES;

    else if (strcmp(read, "/") == 0)
        t.key = DIVISOR;

    else if (read[0] == '%')
        t.key = MODULUS;

    else if (isnumberlit(read))
        t.key = NUMBERLIT;

    else if (isboollit(read))
        t.key = BOOLLIT;

    else if (isstringlit(read))
        t.key = STRINGLIT;

    else if (isid(read))
        t.key = ID;

    return t;
}

// Returns substring of string.
char *substring(unsigned from, unsigned to, const char *str)
{
    if (to >= strlen(str))
        return NULL;

    const unsigned length = strlen(str);
    unsigned i, counter = 0;
    char *result = (char *) calloc(strlen(str), sizeof(char));

    for (i = from; i <= to; i++, counter++)
    {
        result[counter] = str[i];
    }

    return result;
}

// Checks string for being a number literal.
int isnumberlit(const char *str)
{
    const unsigned short START = '0', END = '9';
    const size_t length = strlen(str);
    unsigned i;

    for (i = 0; i < length; i++)
    {
        if (!(str[i] >= START && str[i] <= END))
            return 0;
    }

    return 1;
}

// Checks string for being a boolean literal.
int isboollit(const char *str)
{
    if (strcmp(str, "true") == 0 || strcmp(str, "false") == 0)
        return 1;

    return 0;
}

// Checks string for being a string literal.
int isstringlit(const char *str)
{
    if (str[0] == '"' && str[strlen(str) - 1] == '"')
        return 1;

    return 0;
}

// Checks string for being a string literal.
int isid(const char *str)
{
    unsigned i, length = strlen(str);

    for (i = 1; i < length; i++)
    {
        if (!(isalpha(str[i]) || isdigit(str[i]) || str[i] == '_'))
            return 0;
    }

    if (str[0] == '_' || isalpha(str[0]))
        return 1;

    return 0;
}

// Checks string for being only white-space.
int iswhite_space(const char* str)
{
    unsigned i;
    size_t length = strlen(str);

    for (i = 0; i < length; i++)
    {
        if (str[i] != ' ')
            return 0;
    }

    return 1;
}
