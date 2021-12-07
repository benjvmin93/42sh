#include "clist.h"

#include <stdio.h>

struct clist *init_clist()
{
    struct clist *string = malloc(sizeof(struct clist));
    if (!string)
        return NULL;
    string->size = 1;
    string->data = malloc(sizeof(char) * 32);
    string->data[0] = '\0';
    string->size_mal = 32;

    return string;
}

void print_clist(struct clist *string)
{
    printf("%s", string->data);
}

struct clist *cp_list(struct clist *string)
{
    struct clist *newstring = init_clist();

    newstring = app_str(newstring, string->data);

    return newstring;
}

struct clist *app_str(struct clist *string, char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        string = app_char(string, str[i]);
    }

    return string;
}

struct clist *app_char(struct clist *string, char elt)
{
    if (string->size == string->size_mal)
    {
        string->data = realloc(string->data, 2 * string->size_mal);
        if (!string->data)
            return NULL;

        string->size_mal *= 2;
    }

    string->data[string->size - 1] = elt;
    string->data[string->size] = '\0';
    string->size++;

    return string;
}

void free_clist(struct clist *string)
{
    free(string->data);

    free(string);
}

struct clist *add_spacestr(struct clist *sentence, char *str)
{
    sentence = app_char(sentence, ' ');
    sentence = app_str(sentence, str);

    return sentence;
}

struct clist *new_clist(struct clist *str)
{
    struct clist *tmp = init_clist();

    free_clist(str);

    return tmp;
}
