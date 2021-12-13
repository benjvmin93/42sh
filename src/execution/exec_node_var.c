#include "exec.h"

extern struct variables *variables;

int exec_node_var(struct ast_node *ast)
{
    char *name = ast->data.ast_var.name;
    char *value = strdup(ast->data.ast_var.value);

    for (size_t j = 0; j < variables->variables->size; j++)
    {
        struct var *data = variables->variables->data[j];
        char *cmp = data->name;
        if (!strcmp(name, cmp))
        {
            char *tmp[] = { value, NULL };
            char **new_str_exp = expands_args(tmp);

            free(data->value);
            data->value = new_str_exp[0];
            return 0;
        }
    }

    struct var *new_var;
    new_var = xmalloc(sizeof(struct var));
    new_var->name = strdup(name);

    char *tmp[] = { value, NULL };
    char **new_str_exp = expands_args(tmp);
    new_var->value = new_str_exp[0];

    variables->variables = vector_append(variables->variables, new_var);
    variables->is_assignment = 1;
    return 0;
}
