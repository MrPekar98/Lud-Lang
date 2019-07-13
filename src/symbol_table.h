#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Data types and symbol table element types.
enum type_t {NUM, STRING, VOID, BOOL};
enum element_type {FUNCTION, VARIABLE, CLASS, PROTOCOL, SCOPE};

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
    enum type_t data_type;
};

// Function element.
struct function
{
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
    short open;
};

// Prototypes.
int element_name_exists(struct table_element *elements, unsigned length, char *name);
char *getname(struct table_element element);
symbol_table gettable();
void table_insert(symbol_table *table, struct table_element element, size_t element_size, unsigned line_number);
struct function getfunction(enum type_t return_type, char *name, struct variable *parameters, unsigned parameter_amount);

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
struct function get_function(enum type_t return_type, char *name, struct variable *parameters, unsigned parameter_amount)
{
    struct function func = {.name = (char *) malloc(sizeof(char) * strlen(name)), .parameters = (struct variable *) malloc(sizeof(struct variable) * parameter_amount)};
    func.return_type = return_type;
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
