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
    unsigned count;
} symbol_table;

// Variable element.
struct variable
{
    char *name;
    enum accessor_t accesor;
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
    short open;
};

// Prototypes.
int element_name_exists(struct table_element *elements, unsigned length, char *name);
char *getname(struct table_element element);
symbol_table gettable();
void table_insert(symbol_table *table, struct table_element element, size_t element_size, unsigned line_number);
struct function get_function(enum accessor_t accessor, int is_static, enum type_t return_type, char *name, struct variable *parameters, unsigned parameter_amount);
struct class get_class(char *name, struct variable *variables, unsigned var_amount, struct function *functions, unsigned func_amount);
int exists(const char *name, int is_class);
struct table_element get(const char *name, int is_class_instance);

// Terminates program and prints error.
void table_error(char *msg)
{
    printf("%s\n", msg);
    exit(1);
}

// Checks that a name exists in array.
int element_name_exists(struct table_element *elements, unsigned length, char *name)
{
    unsigned i;

    for (i = 0; i < length; i++)
    {
        switch (elements[i].type)
        {
            case FUNCTION:
                if (strcmp(((struct function *) elements[i].element)->name, name) == 0)
                    return 1;

            case VARIABLE:
                if (strcmp(((struct variable *) elements[i].element)->name, name) == 0)
                    return 1;

            case CLASS: case PROTOCOL:
                if (strcmp(((struct class *) elements[i].element)->name, name) == 0)
                    return 1;
        }
    }

    return 0;
}

// Gets name from table_element if it has a name. Caller is responsible of freeing this heap pointer.
char *getname(struct table_element element)
{
    char *name;
    
    switch (element.type)
    {
        case VARIABLE:
            name = (char *) malloc(sizeof(char) * strlen(((struct variable *) element.element)->name));
            sprintf(name, "%s", ((struct variable *) element.element)->name);
            break;

        case FUNCTION:
            name = (char *) malloc(sizeof(char) * strlen(((struct function *) element.element)->name));
            sprintf(name, "%s", ((struct function *) element.element)->name);
            break;

        case CLASS: case PROTOCOL:
            name = (char *) malloc(sizeof(char) * strlen(((struct class *) element.element)->name));
            sprintf(name, "%s", ((struct class *) element.element)->name);
            break;

        default:
            name = NULL;
    }

    return name;
}

// Gets instance of symbol table.
symbol_table gettable()
{
    symbol_table table = {.open = 1};
    return table;
}

// Inserts element into table.
void table_insert(symbol_table *table, struct table_element element, size_t element_size, unsigned line_number)
{
    char *name = getname(element);

    if (name != NULL && element_name_exists(table->elements, table->count, name))
    {
        sprintf(name, "Line %d: '%s' is already declared.", line_number, name);
        table_error(name);
    }

    else if (element.element == NULL)
    {
        sprintf(name, "Line %d: Element '%s' is unspecified.", line_number, name);
        table_error(name);
    }

    free(name);
    struct table_element *elmts = table->elements;
    table->elements = (struct table_element *) malloc(sizeof(elmts) + element_size);
    
    unsigned i;
    for (i = 0; i < table->count; i++)
    {
        table->elements[i] = elmts[i];
    }

    table->elements[table->count++] = element;
}

// Gets instance of function.
struct function get_function(enum accessor_t accessor, int is_static, enum type_t return_type, char *name, struct variable *parameters, unsigned parameter_amount)
{
    struct function func = {.name = (char *) malloc(sizeof(char) * strlen(name)), .parameters = (struct variable *) malloc(sizeof(struct variable) * parameter_amount)};
    func.return_type = return_type;
    func.is_static = is_static;
    func.accessor = accessor;
    func.paramater_count = parameter_amount;
    func.table.open = 1;
    sprintf(func.name, "%s", name);
    unsigned i;

    // Copying parameters.
    for (i = 0; i < parameter_amount; i++)
    {
        func.parameters[i] = parameters[i];
    }

    return func;
}

// Returns instance of class.
struct class get_class(char *name, struct variable *variables, unsigned var_amount, struct function *functions, unsigned func_amount)
{
    unsigned i;
    struct class result = {.open = 1, .variable_amount = var_amount, .function_amount = func_amount, .name = (char *) malloc(sizeof(char) * strlen(name))};
    result.variables = (struct variable *) malloc(sizeof(struct variable) * var_amount);
    result.functions = (struct function *) malloc(sizeof(struct function) * func_amount);
    strcpy(result.name, name);

    for (i = 0; i < var_amount; i++)
    {
        result.variables[i] = variables[i];
    }

    for (i = 0; i < func_amount; i++)
    {
        result.functions[i] = functions[i];
    }

    return result;
}

// Checks whether a table element is declared.
int exists(const char *name, int is_class)
{
    return 0;
}

// Returns table_element of element.
struct table_element get(const char *name, int is_class_instance)
{
    
}
