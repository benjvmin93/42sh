#include <stdio.h>
#include <stdlib.h>

#include "vector.h"

int main(void)
{
    struct vector *v = vector_init(5, sizeof(char *));
    printf("size=%ld, capacity=%ld\n\n", v->size, v->capacity);

    v = vector_resize(v, 10);

    v = vector_append(v, "foo");
    vector_print(v);
    printf("size=%ld, capacity=%ld\n\n", v->size, v->capacity);

    v = vector_append(v, "bar");
    vector_print(v);
    printf("size=%ld, capacity=%ld\n\n", v->size, v->capacity);

    for (size_t i = 0; i < 11; i++)
    {
        v = vector_append(v, "test");
        vector_print(v);
        printf("size=%ld, capacity=%ld\n\n", v->size, v->capacity);
    }

    char *s = malloc(6 * sizeof(char));
    s[0] = 'H';
    s[1] = 'e';
    s[2] = 'l';
    s[3] = 'l';
    s[4] = 'o';
    s[5] = 0;

    v = vector_append(v, s);

    vector_print(v);

    vector_destroy(v);
}
