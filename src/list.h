#include <stdlib.h>
#define LISTSTD 50

// Struct representing a list.
typedef struct
{
    void **x;               // Array of generic types.
    size_t size;            // Current size of list.
    size_t max;             // Max size until reallocation.
    unsigned int pointer;   // Pointer to first empty array location.
} list_t;

/* Prototypes */
list_t init();
void close(list_t *l);
size_t size(list_t l);
void add(list_t *l, void *element);
void add_by_index(list_t *l, unsigned index, void *element);
void *get(list_t l, unsigned index);
void **toarray(list_t l);
void sub(list_t *l, unsigned index);
list_t sublist(const list_t l, unsigned beginIndex, unsigned endIndex);
list_t filter(const list_t l, int (*boolfunc)(void *));

/* Functions */
/* Initializes array. This function must be called before use of list. */
list_t init() 
{
    return (list_t) {.x = malloc(sizeof(void *) * LISTSTD), .size = 0, .max = LISTSTD, .pointer = 0};
}

/* Closes list. This function should always be called after use of list. */
void close(list_t *l) 
{
    (*l).size = -1;
    (*l).pointer = -1;
    free((*l).x);
}

/* Returns size of list. */
size_t size(list_t l)
{
    return l.size;
}

/* Adds element to array. */
void add(list_t *l, void *element)
{    
    if ((*l).size == (*l).max)
    {
        (*l).max *= 2;
        (*l).x = realloc((*l).x, (*l).max);
    }

    (*l).x[(*l).pointer++] = element;
    (*l).size++;
}

/* Adds element into specific index position. */
void add_by_index(list_t *l, unsigned index, void *element)
{
    if ((*l).size == (*l).max)
    {
        (*l).max *= 2;
        (*l).x = realloc((*l).x, (*l).max);
    }

    int i = (*l).pointer++;
    for (i; i > index; i--)
    {
        (*l).x[i] = (*l).x[i - 1];
    }

    (*l).x[index] = element;
    (*l).size++;
}

/* Gets element by index. */
void *get(list_t l, unsigned index)
{
    return l.x[index];
}

/* Converts the list to void array. */
void **toarray(list_t l)
{
    void **result = malloc(sizeof(void *) * l.size);
    int i;

    for (i = 0; i < l.size; i++)
    {
        result[i] = l.x[i];
    }

    return result;
}

/* Removes element by index. */
void sub(list_t *l, unsigned index)
{
    if ((*l).size == 0)
        return;

    int i = index + 1;
    const size_t size = (*l).size--;
    (*l).pointer--;

    for (i; i < size; i++)
    {
        (*l).x[i - 1] = (*l).x[i];
    }
}

/* Creates a sub-list of a list. */
list_t sublist(const list_t l, unsigned begin, unsigned end)
{
    if (end >= l.size)
        return init();

    int i = begin;
    const size_t size = end - begin;
    list_t returnList = init();

    for (i; i <= end; i++)
    {
        add(&returnList, l.x[i]);
    }

    return returnList;
}

/* Function returns a list depending on boolean function. */
list_t filter(const list_t l, int (*boolfunc)(void *))
{
    list_t result = init(); 
    unsigned i;

    for (i = 0; i < l.size; i++)
    {
        if (boolfunc(get(l, i)))
            add(&result, get(l, i));
    }

    return result;
}