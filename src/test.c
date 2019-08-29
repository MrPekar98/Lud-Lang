#include "symbol_table.h"
#include <assert.h>

// Line number.
unsigned line;

// Prototypes
void test_error();
void test_getname();
void test_name_existence();
void test_field_add();
void test_field_add_error();
void test_method_add();
void test_method_add_error();
void test_parameter_declaration();
void test_table_insertion();
void test_declared1();
void test_declared2();

// Main function.
int main()
{
    // Unit tests.
    //test_error();
    test_getname();
    test_name_existence();
    test_field_add();
    //test_field_add_error();
    test_method_add();
    //test_method_add_error();
    test_parameter_declaration();
    //test_table_insertion();

    // System tests.
    //test_declared1();
    //test_declared2();
    
    return 0;
}

// Test of error messaging. 
void test_error()
{
    table_error("Error test.");
}

// Test getting name of table_element.
void test_getname()
{
    struct function func = {.open = 1, .accessor = INTERNAL, .name = "testFunc", .return_type = NUM, .table = table_init()};
    struct table_element element = {.type = FUNCTION, .element = &func};

    assert(strcmp(getname(element), func.name) == 0);
}

// Test existence of name in array of table_elements.
void test_name_existence()
{
    struct variable var1 = {.name = (char *) malloc(11), .data_type = NUM}, var2 = {.name = (char *) malloc(11), .data_type = BOOL};
    struct function func = {.name = (char *) malloc(9), .open = 1, .return_type = NUM};
    struct table_element element1 = {.type = VARIABLE, .element = &var1}, element2 = {.type = VARIABLE, .element = &var2}, element3 = {.type = FUNCTION, .element = &func};
    struct table_element elements[3] = {element1, element2, element3};

    sprintf(var1.name, "%s", "variable 1");
    sprintf(var2.name, "%s", "variable 2");
    sprintf(func.name, "%s", "function");

    assert(element_name_exists(elements, 3, func.name));
    assert(element_name_exists(elements, 3, var1.name));
    assert(element_name_exists(elements, 3, var2.name));
}

// Tests addition of class field.
void test_field_add()
{
    struct class c = class_init("Test class");
    struct variable var1 = variable_init("Test variable 1", NUM, 0, NULL), var2 = variable_init("Test Variable 2", BOOL, 0, NULL);

    insert_field(&c, var1, 0);
    insert_field(&c, var2, 0);

    assert(strcmp(c.variables[0].name, var1.name) == 0);
    assert(strcmp(c.variables[1].name, var2.name) == 0);
}

// Tests insertion of already existing field.
void test_field_add_error()
{
    struct class c = class_init("Test class");
    struct variable var1 = variable_init("Test variable", NUM, 0, NULL), var2 = variable_init("Test variable", BOOL, 0, NULL);

    insert_field(&c, var1, 0);
    insert_field(&c, var2, 0);
}

// Tests addition of class method.
void test_method_add()
{
    struct class c = class_init("Test class");
    struct function func = function_init("Test function", BOOL);

    insert_method(&c, func, 0);
    assert(strcmp(c.functions[0].name, func.name) == 0);
}

// Tests insertion of already existing method.
void test_method_add_error()
{
    struct class c = class_init("Test class");
    struct function func1 = function_init("Test function", BOOL), func2 = function_init("Test function", NUM);

    insert_method(&c, func1, 0);
    insert_method(&c, func2, 0);
}

// Tests declaration of function parameters.
void test_parameter_declaration()
{
    struct function func = function_init("Test function", BOOL);
    struct variable var1 = variable_init("Test variable 1", STRING, 0, NULL), var2 = variable_init("Test variable 2", NUM, 0, NULL);

    add_parameter(&func, var1, 0);
    add_parameter(&func, var2, 0);
    
    assert(strcmp(var1.name, func.parameters[0].name) == 0);
    assert(strcmp(var2.name, func.parameters[1].name) == 0);
}

// Test insertion of element into symbol table.
void test_table_insertion()
{
    symbol_table table = table_init();
    struct variable var1 = variable_init("Variable 1", NUM, 0, NULL), var2 = variable_init("Variable 2", BOOL, 0, NULL), 
        var3 = variable_init("Variable 3", STRING, 0, NULL);
    struct table_element element1 = {.type = VARIABLE, .element = &var1}, element2 = {.type = VARIABLE, .element = &var2}, 
        element3 = {.type = VARIABLE, .element = &var3};

    table_insert(&table, element1, 0);
    table_insert(&table, element2, 0);
    table_insert(&table, element3, 0);

    assert(strcmp(((struct variable *) table.elements[0].element)->name, var1.name) == 0);
    assert(strcmp(((struct variable *) table.elements[1].element)->name, var2.name) == 0);
    assert(strcmp(((struct variable *) table.elements[2].element)->name, var3.name) == 0);
}

// TODO: table_insert() is still not finished.
// Tests that a name is already declared in first scope.
void test_declared1()
{
    symbol_table table = table_init();
    struct variable var1 = variable_init("Variable 1", NUM, 0, NULL), var2 = variable_init("Variable 2", BOOL, 0, NULL), 
        var3 = variable_init("Variable 1", STRING, 0, NULL);
    struct table_element element1 = {.type = VARIABLE, .element = &var1}, element2 = {.type = VARIABLE, .element = &var2}, 
        element3 = {.type = VARIABLE, .element = &var3};

    table_insert(&table, element1, 0);
    table_insert(&table, element2, 0);
    table_insert(&table, element3, 0);
}

// TODO: Use a function.
// Tests that a name is already declared in second scope of 3.
void test_declared2()
{

}
