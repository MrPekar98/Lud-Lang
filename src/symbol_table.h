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
int variable_exists(struct variable *variables, unsigned length, const char *name);
int function_exists(struct function *functions, unsigned length, const char *name);
void insert_field(struct class *c, struct variable var, unsigned line_number);
void insert_method(struct class *c, struct function func, unsigned line_number);
struct table_element get(symbol_table table, const char *name, unsigned line_number);
struct class get_class(symbol_table table, const char *name, unsigned line_number);
symbol_table table_scope(symbol_table table, unsigned scope);
symbol_table class_to_table(struct class c);
unsigned innermost_scope_level(symbol_table table);

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

        case SCOPE:
            result = (char *) malloc(1);
            sprintf(result, "");
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
    struct function func = {.name = (char *) malloc(strlen(name)), .return_type = return_type};
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

// TODO: Fix random segmentation fault.
// Inserts element into table in given scope.
void table_insert(symbol_table *table, struct table_element element, unsigned line_number)
{
    if (element_name_exists(table->elements, table->element_count, getname(element)))
    {
        char msg[100];
        sprintf(msg, "Line %d: '%s' has already been declared.\n", line_number, getname(element));
        table_error(msg);
    }

    table->elements = (struct table_element *) realloc(table->elements, sizeof(struct table_element) + sizeof(struct table_element) * table->element_count);
    table->elements[table->element_count++] = element;
}

// TODO: Segmentation fault happens. Seems like the formal table parameter is not the same as its actual table parameter.
// Checks for declaration of symbol table element in all open scopes, where lowest scope is the given scope 'table'.
int is_declared(symbol_table table, const char *name)
{    
    unsigned i, j, scopes = innermost_scope_level(table);

    //printf("Element type scope = %d\n", table_scope(table, 2).elements[1].type);

    for (i = 0; i <= scopes; i++)
    {
        symbol_table table = table_scope(table, i);

        // for (j = 0; j < table.element_count; j++)
        // {
        //     if (strcmp(name, getname(table.elements[j])) == 0)
        //         return 1;
        // }

        // for (j = 0; j < table.element_count; j++)
        // {
        //     printf("Name: %s\n", getname(table.elements[j]));
        // }

        // getname(table.elements[j]);
        // printf("Elements: %d\n\n", table.element_count);
        //printf("Type: %d\n", table.elements[0].type);
        //printf("Type: %d\n", table.elements[1].type);
        //printf("Elements: %d\n\n", table.element_count);
    }

    return 0;
}

// Checks whether variable exists in array of variables.
int variable_exists(struct variable *variables, unsigned length, const char *name)
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
int function_exists(struct function *functions, unsigned length, const char *name)
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
    if (variable_exists(c->variables, c->variable_amount, var.name))
    {
        char msg[100];
        sprintf(msg, "Line %d: Field '%s' was already declared in '%s'\n", line_number, var.name, c->name);
        table_error(msg);
    }

    c->variables = (struct variable *) realloc(c->variables, sizeof(struct variable) * c->variable_amount + sizeof(struct variable));
    c->variables[c->variable_amount++] = var;
}

// Inserts method into class.
void insert_method(struct class *c, struct function func, unsigned line_number)
{
    if (function_exists(c->functions, c->function_amount, func.name))
    {
        char msg[100];
        sprintf(msg, "Line %d: Method '%s' was already declared in '%s'\n", line_number, func.name, c->name);
        table_error(msg);
    }

    c->functions = (struct function *) realloc(c->functions, sizeof(struct function) * c->function_amount + sizeof(struct function));
    c->functions[c->function_amount++] = func;
}

// TODO: Finish this!
// Returns table_element of element.
struct table_element get(symbol_table table, const char *name, unsigned line_number)
{
    if (!is_declared(table, name))
    {
        char msg[100];
        sprintf(msg, "Line %d: '%s' has not been declared.\n", line_number, name);
        table_error(msg);
    }
}

// Returns class.
struct class get_class(symbol_table table, const char *name, unsigned line_number)
{

}

// Returns table in argument specified level.
symbol_table table_scope(symbol_table table, unsigned scope)
{
    unsigned level = 0, i, j;
    int found = 0;
    symbol_table current = table;

    for (i = 0; i < current.element_count; i++)
    {        
        if (level == scope)
            break;
        
        switch (current.elements[i].type)
        {
            case SCOPE:                
                if (((symbol_table *) current.elements[i].element)->open)
                {
                    current = *((symbol_table *) current.elements[i].element);
                    found = 1;
                }

                break;

            case FUNCTION:  
                if (((struct function *) current.elements[i].element)->table.open)
                {
                    current = ((struct function *) current.elements[i].element)->table;
                    found = 1;
                }

                break;

            case CLASS: case PROTOCOL:
                if (((struct class *) current.elements[i].element)->open)
                {
                    level++;

                    if (level == scope)
                        return class_to_table(*((struct class *) current.elements[i].element));
                    
                    for (j = 0; j < ((struct class *) current.elements[i].element)->function_amount; j++)
                    {
                        if (((struct class *) current.elements[i].element)->functions[j].table.open)
                        {
                            current = ((struct class *) current.elements[i].element)->functions[j].table;
                            found = 1;
                            break;
                        }
                    }
                }
        }

        if (found)
        {
            i = -1;
            level++;
            found = 0;
        }
    }

    return current;
}

// Converts class into symbol_table.
symbol_table class_to_table(struct class c)
{
    symbol_table table = table_init();
    unsigned i;

    for (i = 0; i < c.variable_amount; i++)
    {
        table_insert(&table, (struct table_element) {.type = VARIABLE, .element = &c.variables[i]}, 0);
    }

    for (i = 0; i < c.function_amount; i++)
    {
        table_insert(&table, (struct table_element) {.type = FUNCTION, .element = &c.functions[i]}, 0);
    }

    return table;
}

// Returns level of the inner most open scope.
unsigned innermost_scope_level(symbol_table table)
{
    unsigned level = 0, i, j;
    int found = 0;
    symbol_table current = table;

    for (i = 0; i < current.element_count; i++)
    {        
        switch (current.elements[i].type)
        {
            case SCOPE:                
                if (((symbol_table *) current.elements[i].element)->open)
                {
                    current = *((symbol_table *) current.elements[i].element);
                    found = 1;
                }

                break;

            case FUNCTION:  
                if (((struct function *) current.elements[i].element)->table.open)
                {
                    current = ((struct function *) current.elements[i].element)->table;
                    found = 1;
                }

                break;

            case CLASS: case PROTOCOL:
                if (((struct class *) current.elements[i].element)->open)
                {
                    level++;
                    
                    for (j = 0; j < ((struct class *) current.elements[i].element)->function_amount; j++)
                    {
                        if (((struct class *) current.elements[i].element)->functions[j].table.open)
                        {
                            current = ((struct class *) current.elements[i].element)->functions[j].table;
                            found = 1;
                            break;
                        }
                    }
                }
        }

        if (found)
        {
            i = -1;
            level++;
            found = 0;
        }
    }

    return level;
}
