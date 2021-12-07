#include "exec.h"

extern struct variables *variables;

struct ast_node *cpy_node(char **list)
{
    struct ast_node *cond = ast_new(NODE_COMMAND);
    char **c = NULL;
    size_t j = 1;
    while (list[j - 1] != NULL)
    {
            c = xrealloc(c, sizeof(char *) * (j + 1));
            c[j - 1] = strdup(list[j - 1]);
            c[j++] = NULL;
    }   
    cond->data.ast_cmd.argv = c;

    return cond;   
}

int exec_node_for(struct ast_node *ast)
{
    int res;
    
    //struct ast_for *ast_for = ast->data.ast_for;
    char **args = ast->data.ast_for.cond->data.ast_cmd.argv;
    struct ast_node *ast_body = ast->data.ast_for.body;
    
    size_t i = 0;

    for (char *curr = args[i]; args[i] != NULL; i++)
    {
        curr = args[i];

        struct var *new_var = xmalloc(sizeof(struct var));
        struct ast_node *tmp = cpy_node(ast_body->data.ast_cmd.argv);

        new_var->name = ast->data.ast_for.name->data.ast_cmd.argv[0];
        new_var->value = curr;

        variables->variables = vector_append(variables->variables, new_var);
        res = exec(tmp);
        variables->variables = vector_remove(variables->variables, variables->variables->size - 1);
        
        free(new_var);
        free_node(tmp);
    }

    ast++;
    return res;
}
