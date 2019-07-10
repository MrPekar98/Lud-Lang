// Data types and symbol table element types.
enum type_t {NUM, STRING, VOID, BOOL};
enum element_type {FUNCTION, VARIABLE, CLASS, PROTOCOL, SCOPE};

// Symbol table collection.
typedef struct
{
    short open;
    void **elements;
} symbol_table;

// Variable element.
struct variable
{
    char *name;
    enum type_t data_type;
    enum element_type type;
};

// Function element.
struct function
{
    enum type_t return_type;
    char *name;
    struct variable *parameters;
    symbol_table table;
    enum element_type type;
};

// Class element.
struct class
{
    char *name;
    struct variable *variables;
    struct function *functions;
    enum element_type type;
    short open;
};

// Gets instance of symbol table.¨
symbol_table get_instance()
{
    symbol_table table;
    table.open = 1;

    return table;
}
