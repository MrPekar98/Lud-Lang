#include "symbol_table.h"

// Global symbol table.
symbol_table table;

// Starting type checking function.
void type_check(node *root)
{
	table = table_init();
}
