#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "../client/communication.h"

// Prototypes.
void test_comp_name_change();
void test_comp();
void test_update();

// Main function.
void main()
{
    test_comp_name_change();
    test_comp();
    test_update();
}

// Tests simple compilation communication without name change.
void test_comp_name_change()
{
    package start = {.url_specified = 1, .name = (char *) malloc(1), .source = (char *) malloc(14)};
    sprintf(start.source, "Hello, World!");

    void *buffer = tobuffer(start);
    package end = topackage(buffer);

    assert(strcmp(start.source, end.source) == 0);
    assert(start.is_renamed == end.is_renamed);
    assert(start.update == end.update);
    assert(start.just_compile == end.just_compile);

    free(start.source);
    free(end.source);
}

// Tests simple compilation communication with name change.
void test_comp()
{
    package start = {.url_specified = 1, .is_renamed = 1, .name = (char *) malloc(10), .source = (char *) malloc(14)};
    sprintf(start.source, "Hello, World!");
    sprintf(start.name, "Test name");

    void *buffer = tobuffer(start);
    package end = topackage(buffer);

    assert(strcmp(start.source, end.source) == 0);
    assert(strcmp(start.name, end.name) == 0);
    assert(start.is_renamed == end.is_renamed);
    assert(start.update == end.update);
    assert(start.just_compile == end.just_compile);

    free(start.source);
    free(end.source);
    free(start.name);
    free(end.name);
}

// Tests update request.
void test_update()
{
    package start = {.url_specified = 1, .update = 1, .just_compile = 1, .name = (char *) malloc(1), .source = (char *) malloc(1)};

    void *buffer = tobuffer(start);
    package end = topackage(buffer);

    assert(start.is_renamed == end.is_renamed);
    assert(start.update == end.update);
    assert(start.just_compile == end.just_compile);

    free(start.source);
    free(end.source);
}
