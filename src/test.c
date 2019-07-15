#include "symbol_table.h"
#include <assert.h>

// Line number.
unsigned long line;

// Prototypes
void test_error();
void test_table_element();
void test_table_insert();
void test_get_function();
void test_function_scope();
void test_get_class();

// Main function.
int main()
{
    // Running tests.
    //test_error();
    test_table_element();
    test_table_insert();
    test_get_function();
    test_function_scope();
    test_get_class();

    return 0;
}

// Test of error messaging. 
void test_error()
{
    table_error("Error test.");
}

// Test getting name from table_element.
void test_table_element()
{
    struct variable var = {.name = "Variable", .data_type = STRING};
    struct table_element element = {.type = VARIABLE, .element = &var};

    assert(strcmp(getname(element), "Variable") == 0);
    assert(element.type == VARIABLE);
}

// Tests insertion of element into symbol table.
void test_table_insert()
{
    symbol_table table = gettable();
    struct variable var1 = {.name = "Variable1", .data_type = STRING};
    struct variable var2 = {.name = "Variable2", .data_type = BOOL};
    struct table_element element1 = {.type = VARIABLE, .element = &var1};
    struct table_element element2 = {.type = VARIABLE, .element = &var2};

    table_insert(&table, element1, sizeof(element1), ++line);

    assert(table.open);
    assert(table.count == 1);
    assert(strcmp(getname(table.elements[0]), "Variable1") == 0);

    table_insert(&table, element2, sizeof(element2), ++line);

    assert(table.count == 2);
    assert(strcmp(getname(table.elements[0]), "Variable2") == 0);
}

// Tests information of get_function.
void test_get_function()
{
    struct variable var1 = {.name = "arg1", .data_type = NUM}, var2 = {.name = "arg2", .data_type = STRING};
    struct variable params[] = {var1, var2};
    struct function func = get_function(NONE, 0, VOID, "test_function", params, 2);

    assert(strcmp(func.name, "test_function") == 0);
    assert(func.return_type == VOID);
    assert(func.table.open);
    assert(func.paramater_count == 2);
    assert(sizeof(func.parameters[1]) == sizeof(var2));
}

// Tests elements in scope of function.
void test_function_scope()
{
    struct variable element = {.name = "Some element", .data_type = BOOL};
    struct function func = get_function(NONE, 0, VOID, "test_function", NULL, 0);
    struct table_element te = {.type = VARIABLE, .element = &element};

    table_insert(&func.table, te, sizeof(te), ++line);
    func.table.open = 0;

    assert(sizeof(func.parameters) == sizeof(struct variable *));
    assert(!func.table.open);
    assert(func.table.count == 1);
    assert(sizeof(func.table.elements[0]) == sizeof(te));
}

// Test information of get_class.
void test_get_class()
{
    struct variable var1 = get_variable(NONE, 0, 0, NUM, "var1", 0, NULL);
    struct variable var2 = get_variable(NONE, 0, 0, NUM, "var2", 0, NULL);
    struct variable vars[] = {var1, var2};
    struct function fun1 = get_function(NONE, 0, BOOL, "fun1", vars, 2);
    struct function fun2 = get_function(NONE, 0, NUM, "fun2", NULL, 0);
    struct function functions[] = {fun1, fun2};
    struct class cl = get_class("MyClass", vars, 2, functions, 2);

    assert(cl.open);
    assert(strcmp(cl.name, "MyClass") == 0);
    assert(cl.variable_amount == 2);
    assert(cl.function_amount == 2);
    assert(strcmp(cl.variables[1].name, var2.name) == 0);
    assert(strcmp(cl.functions[0].name, fun1.name) == 0);
}
