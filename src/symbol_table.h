#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Data types and symbol table element types.
enum type_t {NUM, STRING, VOID, BOOL};
enum element_type {FUNCTION, VARIABLE, CLASS, PROTOCOL, SCOPE};
enum accessor_t {PRIVATE, INTERNAL, PUBLIC, NONE};

// General symbol table element.
struct table_element
{
    enum element_type type;
    void *element;
};

// Symbol table collection.
typedef struct
{
    short open;
    struct table_element *elements;
    unsigned element_count;
} symbol_table;

// Variable element.
struct variable
{
    char *name;
    enum accessor_t accessor;
    int is_static;
    int is_const;
    enum type_t data_type;
    int is_class_intance;
    char *class_name;
};

// Function element.
struct function
{
    int open;
    enum accessor_t accessor;
    int is_static;
    enum type_t return_type;
    char *name;
    struct variable *parameters;
    unsigned paramater_count;
    symbol_table table;
};

// Class element.
struct class
{
    char *name;
    struct variable *variables;
    struct function *functions;
    unsigned variable_amount, function_amount;
    int open;
};

// Prototypes.
int element_name_exists(struct table_element *elements, unsigned length, const char *name);
static inline char *getname(struct table_element element);
symbol_table table_init();
struct class class_init(const char *name);
struct variable variable_init(const char *name, enum type_t type, int is_class_instance, const char *class_name);
struct function function_init(const char *name, enum type_t return_type);
void add_parameter(struct function *func, struct variable var, unsigned line_number);
void table_insert(symbol_table *table, struct table_element element, unsigned line_number);
int is_declared(symbol_table table, const char *name);
int variable_exists(struct variable *variables, size_t length, const char *name);
int function_exists(struct function *functions, size_t length, const char *name);
void insert_field(struct class *c, struct variable var, unsigned line_number);
void insert_method(struct class *c, struct function func, unsigned line_number);
struct table_element get(symbol_table table, const char *name, unsigned line_number);
struct class get_class(symbol_table table, const char *name, unsigned line_number);
symbol_table get_table_level(symbol_table table);
unsigned innermost_level(symbol_table table);

// Terminates program and prints error.
void table_error(char *msg)
{
    printf("%s\n", msg);
    exit(1);
}

// Checks that a name exists in array.
int element_name_exists(struct table_element *elements, unsigned length, const char *name)
{
    unsigned i;

    for (i = 0; i < length; i++)
    {
        char *element_name = getname(elements[i]);

        if (element_name != NULL && strcmp(element_name, name) == 0)
            return 1;

        if (element_name != NULL)
            free(element_name);
    }

    return 0;
}

// Gets name from table_element if it has a name. Caller is responsible of freeing this heap pointer.
static inline char *getname(struct table_element element)
{    
    char *result = NULL;
    
    switch (element.type)
    {
        case FUNCTION:
            result = (char *) malloc(strlen(((struct function *) element.element)->name));
            sprintf(result, "%s", ((struct function *) element.element)->name);
            break;

        case VARIABLE:
            result = (char *) malloc(strlen(((struct variable *) element.element)->name));
            sprintf(result, "%s", ((struct variable *) element.element)->name);
            break;

        case CLASS: case PROTOCOL:
            result = (char *) malloc(strlen(((struct class *) element.element)->name));
            sprintf(result, "%s", ((struct class *) element.element)->name);
            break;
    }

    return result;
}

// Gets instance of symbol table.
symbol_table table_init()
{
    symbol_table table = {.open = 1};
    return table;
}

// Gets instance of class.
struct class class_init(const char *name)
{
    struct class c = {.open = 1, .name = (char *) malloc(strlen(name))};
    c.variables = (struct variable *) malloc(sizeof(struct variable));
    c.functions = (struct function *) malloc(sizeof(struct function));
    sprintf(c.name, "%s", name);

    return c;
}

// Gets instance of variable.
struct variable variable_init(const char *name, enum type_t type, int is_class_instance, const char *class_name)
{
    struct variable var = {.name = (char *) malloc(strlen(name)), .data_type = type};
    sprintf(var.name, "%s", name);

    if (is_class_instance)
    {
        var.class_name = (char *) malloc(strlen(class_name));
        sprintf(var.class_name, "%s", class_name);
    }

    return var;
}

// Gets instance of function.
struct function function_init(const char *name, enum type_t return_type)
{
    struct function func = {.open = 1, .name = (char *) malloc(strlen(name)), .return_type = return_type};
    func.parameters = (struct variable *) malloc(sizeof(struct variable));
    func.table.open = 1;
    sprintf(func.name, "%s", name);

    return func;
}

// Adds parameter to function.
void add_parameter(struct function *func, struct variable var, unsigned line_number)
{
    unsigned i;

    for (i = 0; i < func->paramater_count; i++)
    {
        if (strcmp(var.name, func->parameters[i].name) == 0)
        {
            char msg[100];
            sprintf(msg, "Line %d: Parameter '%s' has already been declared.\n", line_number, var.name);
            table_error(msg);
        }
    }

    func->parameters = (struct variable *) realloc(func->parameters, sizeof(struct variable) * func->paramater_count + sizeof(struct variable));
    func->parameters[func->paramater_count++] = var;
}

// TODO: Finish this!
// Inserts element into table in given scope.
void table_insert(symbol_table *table, struct table_element element, unsigned line_number)
{
    const size_t size = element.type == (FUNCTION ? sizeof(struct function) : 
                        element.type == VARIABLE ? sizeof(struct variable) : 
                        element.type == CLASS || element.type == PROTOCOL ? sizeof(struct class) : 
                        sizeof(symbol_table)) + sizeof(int);

    if (element_name_exists(table->elements, table->element_count, getname(element)))
    {
        char msg[100];
        sprintf(msg, "Line %d: '%s' has already been declared.\n", line_number, getname(element));
        table_error(msg);
    }

    // TODO: Dalej...
}

// Checks for declaration of symbol table element in all open scopes, where lowest scope is the given scope 'table'.
int is_declared(symbol_table table, const char *name)
{
    if (!table.open)
        return 0;
    
    unsigned i;
    symbol_table current = table;

    for (i = 0; i < current.element_count; i++)
    {
        if (i == 0 && element_name_exists(table.elements, table.element_count, name))
            return 1;

        
        switch (current.elements[i].type)
        {
            case SCOPE:                
                if (((symbol_table *) current.elements[i].element)->open)
                {
                    current = *((symbol_table *) current.elements[i].element);
                    i = -1;
                    continue;
                }

            case FUNCTION:
                if (strcmp(((struct function *) current.elements[i].element)->name, name) == 0)
                    return 1;
                
                else if (((struct function *) current.elements[i].element)->table.open)
                {
                    current = ((struct function *) current.elements[i].element)->table;
                    i = -1;
                    continue;
                }

            case CLASS: case PROTOCOL:
                if (((struct class *) current.elements[i].element)->open)
                {
                    if (variable_exists(((struct class *) current.elements[i].element)->variables, ((struct class *) current.elements[i].element)->variable_amount, name) ||
                        function_exists(((struct class *) current.elements[i].element)->functions, ((struct class *) current.elements[i].element)->function_amount, name))
                        return 1;

                    // TODO: Check for open function scopes.

                    i = -1;
                    continue;
                }
        }
    }
}

// Checks whether variable exists in array of variables.
int variable_exists(struct variable *variables, size_t length, const char *name)
{
    unsigned i;

    for (i = 0; i < length; i++)
    {
        if (strcmp(name, variables[i].name) == 0)
            return 1;
    }

    return 0;
}

// Checks whether function exists in array of functions.
int function_exists(struct function *functions, size_t length, const char *name)
{
    unsigned i;

    for (i = 0; i < length; i++)
    {
        if (strcmp(name, functions[i].name) == 0)
            return 1;
    }

    return 0;
}

// Inserts field into class.
void insert_field(struct class *c, struct variable var, unsigned line_number)
{
    unsigned i;

    for (i = 0; i < c->variable_amount; i++)
    {
        if (strcmp(c->variables[i].name, var.name) == 0)
        {
            char msg[100];
            sprintf(msg, "Line %d: Field '%s' was already declared in '%s'\n", line_number, var.name, c->name);
            table_error(msg);
        }
    }

    c->variables = (struct variable *) realloc(c->variables, sizeof(struct variable) * c->variable_amount + sizeof(struct variable));
    c->variables[c->variable_amount++] = var;
}

// Inserts method into class.
void insert_method(struct class *c, struct function func, unsigned line_number)
{
    unsigned i;

    for (i = 0; i < c->function_amount; i++)
    {
        if (strcmp(c->functions[i].name, func.name) == 0)
        {
            char msg[100];
            sprintf(msg, "Line %d: Method '%s' was already declared in '%s'\n", line_number, func.name, c->name);
            table_error(msg);
        }
    }

    c->functions = (struct function *) realloc(c->functions, sizeof(struct function) * c->function_amount + sizeof(struct function));
    c->functions[c->function_amount++] = func;
}

// Returns table_element of element.
struct table_element get(symbol_table table, const char *name, unsigned line_number)
{
    
}

// Returns class.
struct class get_class(symbol_table table, const char *name, unsigned line_number)
{

}

// Returns table in argument specified level.
symbol_table get_table_level(symbol_table table)
{

}

// Returns level of the inner most open scope.
unsigned innermost_level(symbol_table table)
{
    return 0;
}
