// TODO: Ignore comments. When '#' has been read, ignore everything until '#' has been met again.

#include <stdio.h>
#include <string.h>
#include "token.h"

// Prototypes.
static int is_single_character_token(char c);
static int recognise(const char *buffer, unsigned length);
static int recognise_accessor(const char *buffer);
static int recognise_datatype(const char *buffer);
static int recognise_operator(const char *buffer);
static int is_string(const char *buffer, unsigned length);
static int is_char(const char *buffer, unsigned length);
static int is_bool(const char *buffer);
static int is_number(const char *buffer, unsigned length);

// External .lud program file.
extern FILE *prog;

// Reads next token.
lex_t read_token()
{
    int c;
    char buffer[100];
    unsigned counter = 0;
    lex_t t = {.error = 0};

    while ((c = fgetc(prog)) != -1)
    {
        if (counter == 0 && (c == ' ' || c == '\n'))
            continue;
        
        else if (c == ' ' || c == '\n')
            break;

        else if (is_single_character_token(c))
        {
            fseek(prog, ftell(prog) - 1, SEEK_SET);
            break;
        }
        
        buffer[counter++] = c;
    }
    
    int rec = recognise(buffer, counter);
    sprintf(t.lexeme, "%s", buffer);
    t.token = rec;

    if (rec == -1)
        t.error = 1;

    return t;
}

// Reverses toke reading by one.
void reverse_token(lex_t last_read)
{
    fseek(prog, ftell(prog) - strlen(last_read.lexeme) - 1, SEEK_SET);
}

// Determines whether a token can be next to another lexeme without a space.
static int is_single_character_token(char c)
{
    switch (c)
    {
        case '.': case ',': case '(': case ')': case '{': case '}': case '[': case ']': case ':': case ';': case '=': case '|': case '&': case '^':
            return 1;

        default:
            return 0;
    }
}

// Determines token recognised.
static int recognise(const char *buffer, unsigned length)
{
    if (is_string(buffer, length) || is_char(buffer, length) || is_bool(buffer) || is_number(buffer, length))
        return LITERAL;
    
    else if (length == 1)
    {
        if (recognise_operator(buffer) != -1)
            return recognise_operator(buffer);
        
        switch (buffer[0])
        {
            case '.':
                return DOT;

            case ',':
                return COMMA;

            case '(':
                return LPARAN;

            case ')':
                return RPARAN;

            case '{':
                return LBRACE;

            case '}':
                return RBRACE;

            case '[':
                return LBRACKET;

            case ']':
                return RBRACKET;

            case ':':
                return COLON;

            case ';':
                return SEMICOLON;

            case '=':
                return EQ;

            case '|':
                return OR;

            case '&':
                return AND;

            case '^':
                return XOR;

            default:
                return ID;
        }
    }

    else if (strcmp("method", buffer) == 0)
        return METHOD;

    else if (strcmp("import", buffer) == 0)
        return IMPORT;

    else if (strcmp("protocol", buffer) == 0)
        return PROTOCOL;

    else if (strcmp("class", buffer) == 0)
        return CLASS;

    else if (strcmp("->", buffer) == 0)
        return ARROW;

    else if (strcmp("using", buffer) == 0)
        return USING;

    else if (strcmp("system", buffer) == 0)
        return SYSTEM;

    else if (strcmp("static", buffer) == 0)
        return STATIC;

    else if (strcmp("const", buffer) == 0)
        return CONST;

    else if (strcmp("this", buffer) == 0)
        return THIS;

    else if (strcmp("super", buffer) == 0)
        return SUPER;

    else if (strcmp("create", buffer) == 0)
        return CREATE;

    else if (strcmp("if", buffer) == 0)
        return IF;

    else if (strcmp("else", buffer) == 0)
        return ELS;

    else if (strcmp("break", buffer) == 0)
        return BREAK;

    else if (strcmp("continue", buffer) == 0)
        return CONTINUE;

    else if (strcmp("for", buffer) == 0)
        return FOR;

    else if (strcmp("while", buffer) == 0)
        return WHILE;

    else if (strcmp("foreach", buffer) == 0)
        return FOREACH;

    else if (strcmp("constructor", buffer) == 0)
        return CONSTRUCTOR;

    else if (strcmp("return", buffer) == 0)
        return RETURN;

    else if (recognise_accessor(buffer) != -1)
        return recognise_accessor(buffer);

    else if (recognise_datatype(buffer) != -1)
        return recognise_datatype(buffer);

    return length > 0 ? ID : -1;
}

// Recognises accessors.
static int recognise_accessor(const char *buffer)
{
    if (strcmp("public", buffer) == 0 || strcmp("internal", buffer) == 0 || strcmp("private", buffer) == 0)
        return ACCESSOR;

    return -1;
}

// Recognises data types.
static int recognise_datatype(const char *buffer)
{
    if (strcmp("void", buffer) == 0 || strcmp("num", buffer) == 0 || strcmp("string", buffer) == 0 || strcmp("bool", buffer) == 0 || strcmp("char", buffer) == 0)
        return DATATYPE;

    return -1;
}

// Recognises operators.
static int recognise_operator(const char *buffer)
{
    switch (buffer[0])
    {
        case '%':
            return MODULUS;

        case '+': case '-': case '*': case '/':
            return OPERATOR;

        default:
            return -1;
    }
}

// Checks whether input is a string.
static int is_string(const char *buffer, unsigned length)
{
    unsigned i;

    for (i = 1; i < length - 1; i++)
    {
        if (buffer[i] == '\"')
            return 0;
    }

    return buffer[0] == '\"' && buffer[length - 1] == '\"';
}

// Checks whether input is a char.
static int is_char(const char *buffer, unsigned length)
{
    return length == 3 && buffer[0] == '\'' && buffer[length - 1] == '\'';
}

// Checks whether input is a bool.
static int is_bool(const char *buffer)
{
    return strcmp(buffer, "true") == 0 || strcmp(buffer, "false") == 0;
}

// Checks whether input is a number.
static int is_number(const char *buffer, unsigned length)
{
    unsigned i;
    int dot_exists = 0;

    for (i = 1; i < length - 1; i++)
    {
        if ((buffer[i] == '.' && dot_exists) || (buffer[i] < 48 || buffer[i] > 57))
            return 0;
        
        else if (buffer[i] == '.')
            dot_exists = 1;
    }

    return buffer[0] != '.' && buffer[length - 1] != '.' && buffer[0] > 47 && buffer[length - 1] < 58;
}
