#define LEX_LENGTH 50

// Enum for all tokens.
enum token_t {LPARAN, RPARAN, LBRACE, RBRACE, IMPORT, ID, LITERAL, PROTOCOL, CLASS, ARROW, RBRACKET, LBRACKET, USING, COLON, SEMICOLON, SYSTEM, ACCESSOR,
                STATIC, CONST, DATATYPE, VOID, EQUALS, DOT, COMMA, THIS, SUPER, CREATE, OR, AND, XOR, OPERATOR, MODULUS, IF, ELSE, FOR, WHILE, FOREACH,
                METHOD, CONSTRUCTOR, RETURN, NOT, BREAK, CONTINUE};

// Specifies a node type.
enum node_type {};

// Token structure. An error occured when token is -1.
typedef struct
{
    char lexeme[LEX_LENGTH];
    enum token_t token;
    int error;
} lex_t;

// AST node.
typedef struct 
{
    enum node_type type;
    void **children;
} node;
