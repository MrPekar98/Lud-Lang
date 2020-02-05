#include <stdlib.h>
#include "token.h"

// Adds child to parent node.
void add_child(node *parent, node child)
{
	if (parent->children == 0)
		parent->children = (void **) malloc(sizeof(node));

	else
		parent->children = (void **) realloc(parent->children, sizeof(node) * (parent->children_count + 1));

	parent->children[parent->children_count] = &child;
	parent->children_count++;
}
