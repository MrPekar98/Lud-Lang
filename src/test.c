#include "symbol_table.h"
#include <assert.h>

// Prototypes
void test_error();
void test_table_element();
void test_table_insert();

// Main function.
int main()
{    
    // Running tests.
    //test_error();
    test_table_element();
    test_table_insert();

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
    symbol_table table = get_table();
    struct variable var1 = {.name = "Variable1", .data_type = STRING};
    struct variable var2 = {.name = "Variable2", .data_type = BOOL};
    struct table_element element1 = {.type = VARIABLE, .element = &var1};
    struct table_element element2 = {.type = VARIABLE, .element = &var2};

    table_insert(&table, element1, sizeof(element1));

    assert(table.open);
    assert(table.count == 1);
    assert(strcmp(getname(table.elements[0]), "Variable1") == 0);

    table_insert(&table, element2, sizeof(element2));

    assert(table.count == 2);
    assert(strcmp(getname(table.elements[0]), "Variable2") == 0);
}
