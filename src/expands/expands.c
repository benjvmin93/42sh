#define _DEFAULT_SOURCE

#include "expands.h"

#include <string.h>

#include "../utils/clist.h"

extern struct variables *variables;

char *expand_var(char *var, size_t index)
{
    struct clist *var_data = init_clist();

    if (var[index + 1] == '{')
        index++;
    for (size_t i = index + 1; var[i] && var[i] != '\"' && var[i] != '\''
         && var[i] != '}' && var[i] != '$';
         i++)
        var_data = app_char(var_data, var[i]);
    for (size_t j = 0; j < variables->variables->size; j++)
    {
        struct var *data = variables->variables->data[j];
        if (!strcmp(var_data->data, data->name))
        {
            free_clist(var_data);
            return data->value;
        }
    }
    free_clist(var_data);
    return "";
}

char **expands_args(char **list_word)
{
    int escape = 0;
    for (size_t i = 0; list_word[i]; i++)
    {
        struct clist *expand = init_clist();
        size_t j = 0;

        char quote = 0;
        if ((list_word[i][j] == '\"') || (list_word[i][j] == '\''))
        {
            quote = list_word[i][j];
            j++;
        }

        for (; list_word[i][j]; j++)
        {
            if (!list_word[i][j + 1] && list_word[i][j] == quote)
                break;
            if (list_word[i][j] == '$')
                break;
            if (!quote)
            {
                if (escape || list_word[i][j] != '\\')
                {
                    expand = app_char(expand, list_word[i][j]);
                    escape = 0;
                }
                else
                    escape = 1;
            }
            else
                expand = app_char(expand, list_word[i][j]);
        }

        for (; list_word[i][j]; j++)
        {
            if (!list_word[i][j + 1] && list_word[i][j] == quote)
                break;
            if (list_word[i][j] == '$')
            {
                struct clist *new_value = init_clist();
                new_value = app_str(new_value, expand_var(list_word[i], j));
                expand = app_str(expand, new_value->data);
                free_clist(new_value);
            }
        }
        free(list_word[i]);
        list_word[i] = strdup(expand->data);
        free_clist(expand);
    }

    return list_word;
}
