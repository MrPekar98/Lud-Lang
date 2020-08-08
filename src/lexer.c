#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "compiler.h"

// Prototypes.
static void skip_comment();
static int is_single_character_token(char c);
static int recognise(const char *buffer, unsigned length);
static int recognise_accessor(const char *buffer);
static int recognise_datatype(const char *buffer);
static int recognise_operator(const char *buffer);
static int is_string(const char *buffer, unsigned length);
static int is_char(const char *buffer, unsigned length);
static int is_bool(const char *buffer);
static int is_number(const char *buffer, unsigned length);
static int is_address(const char *buffer, unsigned length);
static inline short is_hexa_character(char h);

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
        if (counter == 0 && c == '#')
        {
            skip_comment();
            continue;
        }

        else if (counter == 0 && (c == ' ' || c == '\n'))
            continue;

        else if (counter == 0 && c == '"')
        {
            buffer[counter++] = c;
            
            while ((c = fgetc(prog)) != -1 && c != '"')
            {
                buffer[counter++] = c;
            }

            buffer[counter++] = '"';
            break;
        }

        else if (c == ' ' || c == '\n' || c == '#')
            break;

        else if (is_single_character_token(c) && counter > 0)
        {
            fseek(prog, ftell(prog) - 1, SEEK_SET);
            break;
        }

        else if (is_single_character_token(c) && counter == 0)
        {
            buffer[counter++] = c;
            break;
        }

        buffer[counter++] = c;
    }

    sprintf(t.lexeme, "%s", buffer);
    t.lexeme[counter] = '\0';
    t.token = recognise(t.lexeme, counter);

    if (t.token == -1)
        t.error = 1;

    return t;
}

// Reverses toke reading by one.
void reverse_token(lex_t last_read)
{
    fseek(prog, ftell(prog) - strlen(last_read.lexeme) - 1, SEEK_SET);
}

// Skips everything until end of comment is met.
static void skip_comment()
{
    int c;
    while ((c = fgetc(prog)) != '#' && c != -1);
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
    if (is_string(buffer, length) || is_char(buffer, length) || is_bool(buffer) || is_number(buffer, length) || is_address(buffer, length) || strcmp(buffer, "null") == 0)
        return LITERAL_T;

    else if (length == 1)
    {
        if (recognise_operator(buffer) != -1)
            return OPERATOR_T;

        switch (buffer[0])
        {
            case '.':
                return DOT_T;

            case ',':
                return COMMA_T;

            case '(':
                return LPARAN_T;

            case ')':
                return RPARAN_T;

            case '{':
                return LBRACE_T;

            case '}':
                return RBRACE_T;

            case '[':
                return LBRACKET_T;

            case ']':
                return RBRACKET_T;

            case ':':
                return COLON_T;

            case ';':
                return SEMICOLON_T;

            case '=':
                return EQ_T;

            case '|':
                return OR_T;

            case '&':
                return AND_T;

            case '^':
                return XOR_T;

            default:
                return ID_T;
        }
    }

    else if (strcmp("import", buffer) == 0)
        return IMPORT_T;

    else if (strcmp("protocol", buffer) == 0)
        return PROTOCOL_T;

    else if (strcmp("class", buffer) == 0)
        return CLASS_T;

    else if (strcmp("inherits", buffer) == 0)
        return INHERITS;

    else if (strcmp("implements", buffer) == 0)
        return IMPLEMENTS;

    else if (strcmp("system", buffer) == 0)
        return SYSTEM_T;

    else if (strcmp("static", buffer) == 0)
        return STATIC_T;

    else if (strcmp("const", buffer) == 0)
        return CONST_T;

    else if (strcmp("this", buffer) == 0)
        return THIS_T;

    else if (strcmp("super", buffer) == 0)
        return SUPER_T;

    else if (strcmp("create", buffer) == 0)
        return CREATE_T;

    else if (strcmp("if", buffer) == 0)
        return IF_T;

    else if (strcmp("else", buffer) == 0)
        return ELSE_T;

    else if (strcmp("break", buffer) == 0)
        return BREAK_T;

    else if (strcmp("continue", buffer) == 0)
        return CONTINUE_T;

    else if (strcmp("for", buffer) == 0)
        return FOR_T;

    else if (strcmp("while", buffer) == 0)
        return WHILE_T;

    else if (strcmp("foreach", buffer) == 0)
        return FOREACH_T;

    else if (strcmp("constructor", buffer) == 0)
        return CONSTRUCTOR_T;

    else if (strcmp("destructor", buffer) == 0)
        return DESTRUCTOR_T;

    else if (strcmp("return", buffer) == 0)
        return RETURN_T;

    else if (strcmp("abstract", buffer) == 0)
        return ABSTRACT_T;

    else if (strcmp("namespace", buffer) == 0)
        return NAMESPACE_T;

    else if (recognise_accessor(buffer) != -1)
        return ACCESSOR_T;

    else if (recognise_datatype(buffer) != -1)
        return recognise_datatype(buffer);

    return length > 0 ? ID_T : -1;
}

// Recognises accessors.
static int recognise_accessor(const char *buffer)
{
    if (strcmp("public", buffer) == 0 || strcmp("internal", buffer) == 0 || strcmp("private", buffer) == 0)
        return ACCESSOR_T;

    return -1;
}

// Recognises data types.
static int recognise_datatype(const char *buffer)
{
    if (strcmp("address", buffer) == 0 || strcmp("void", buffer) == 0 || strcmp("num", buffer) == 0 || strcmp("string", buffer) == 0 || strcmp("bool", buffer) == 0 || strcmp("char", buffer) == 0)
        return DATATYPE_T;

    return -1;
}

// Recognises operators.
static int recognise_operator(const char *buffer)
{
    switch (buffer[0])
    {
        case '%':
            return MODULUS_T;

        case '+': case '-': case '*': case '/':
            return OPERATOR_T;

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

// Checks whether input is an address.
static int is_address(const char *buffer, unsigned length)
{
	unsigned i;

	for (i = 2; i < length; i++)
	{
		if (!is_hexa_character(buffer[i]))
			return 0;
	}

	return buffer[0] == '0' && (buffer[1] == 'x' || buffer[1] == 'X');
}

// Checks that character is a hexadecimal character.
static inline short is_hexa_character(char h)
{
    if (isdigit(h))
        return 1;

    switch (h)
    {
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
            return 1;

        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
            return 1;

        default:
            return 0;
    }
}