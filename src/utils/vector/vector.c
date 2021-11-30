#include "vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct vector *vector_init(size_t n, size_t size)
{
    struct vector *new = malloc(sizeof(struct vector));
    if (!new)
        return NULL;
    new->size = 0;
    new->capacity = n;
    new->size_data = size;
    new->data = malloc(new->capacity *new->size_data);
    if (!new->data)
        return NULL;
    return new;
}

void vector_destroy(struct vector *v)
{
    if (!v)
        return;
    free(v->data);
    free(v);
}

struct vector *vector_resize(struct vector *v, size_t n)
{
    if (!v)
        return NULL;

    if (n == v->capacity)
        return v;

    else if (n > v->capacity)
    {
        v->data = realloc(v->data, n * v->size_data);
        if (!v->data)
            return NULL;
    }
    else
    {
        void **new_arr = malloc(n * v->size_data);
        if (!new_arr)
            return NULL;

        for (size_t i = 0; i < n; i++)
        {
            new_arr[i] = v->data[i];
            free(v->data[i]);
        }
        free(v->data);
        v->data = new_arr;
        v->size = n;
    }

    v->capacity = n;
    return v;
}

struct vector *vector_append(struct vector *v, void *elt)
{
    if (!v)
        return NULL;

    if (v->size == v->capacity)
    {
        v->capacity *= 2;
        v->data = realloc(v->data, v->capacity * v->size_data);
    }

    v->data[v->size++] = elt;

    return v;
}

void vector_print(const struct vector *v)
{
    if (!v)
    {
        printf("\n");
        return;
    }
    for (size_t i = 0; i < v->size; i++)
    {
        if (i == v->size - 1)
            printf("%s", (char *)v->data[i]);
        else
            printf("%s,", (char *)v->data[i]);
    }
    printf("\n");
}

struct vector *vector_reset(struct vector *v, size_t n)
{
    if (!v)
        return NULL;

    for (size_t i = 0; i < v->size; i++)
        free(v->data[i]);
    free(v->data);
    v->data = malloc(n * v->size_data);
    if (!v->data)
        return NULL;

    v->size = 0;
    v->capacity = n;

    return v;
}

struct vector *vector_remove(struct vector *v, size_t i)
{
    if (!v)
        return NULL;

    if (v->size == 0)
        return NULL;

    if (i > v->size)
        return NULL;

    struct vector *new = vector_init(v->size, v->size_data);
    for (size_t j = 0; j < v->size - 1; j++)
    {
        if (j >= i)
            new = vector_append(new, v->data[j + 1]);
        else
            new = vector_append(new, v->data[j]);
    }

    vector_destroy(v);

    return new;
}
