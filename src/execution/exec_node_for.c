#include "exec.h"

extern struct variables *variables;

/**
struct ast_node *cpy_node_cmd()
{
    char **list = ast->data.ast_cmd.argv;
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

struct ast_node *cpy_node_andor(struct ast_node *ast)
{
    struct ast_node *ast_andor = ast_new(NODE_AND_OR);
    size_t size = ast->data.ast_andor.size;

    int *list = ast_andor->data.ast_andor.list_operator;
    for (size_t i = 0; i < size; i++)
        list[i] = ast->data.ast_andor.list_operator[i];
    //ast_andor->data.ast_andor.list_operator = list;

    for (size_t i = 0; i <= size; i++)
    {
        struct ast_node *ast_node = ast->data.ast_andor.list_ast_node->data[i];
        struct ast_node *cpy = cpy_node(ast_node);
        ast_andor->data.ast_andor.list_ast_node->data[i] = cpy;
    }

    ast_andor->data.ast_andor.list_ast_node->size =
ast->data.ast_andor.list_ast_node->size;
    ast_andor->data.ast_andor.list_ast_node->size_data =
ast->data.ast_andor.list_ast_node->size_data;
    ast_andor->data.ast_andor.list_ast_node->capacity =
ast->data.ast_andor.list_ast_node->capacity;

    return ast_andor;
}

struct ast_node *cpy_node(struct ast_node *ast)
{
    struct ast_node *tmp;
    switch(ast->type)
    {
        case NODE_COMMAND:
            tmp = cpy_node_cmd(ast);
            break;
        case NODE_AND_OR:
            tmp = cpy_node_andor(ast);
            break;
        default:
            tmp = NULL;
            break;
  *      case NODE_FOR:
            tmp = cpy_node_for(ast);
        case NODE_IF:
            tmp = cpy_node_if(ast);
        case NODE_WHILE:
            tmp = cpy_node_while(ast);
    }
    return tmp;
}
*/

int exec_node_for(struct ast_node *ast)
{
    int res;

    // struct ast_for *ast_for = ast->data.ast_for;
    char **args = ast->data.ast_for.cond->data.ast_cmd.argv;
    struct ast_node *ast_body = ast->data.ast_for.body;
    // struct ast_node *tmp = ast_body;
    size_t i = 0;

    for (char *curr = args[i]; args[i] != NULL; i++)
    {
        curr = args[i];

        struct var *new_var = xmalloc(sizeof(struct var));
        // tmp = cpy_node(ast_body);

        new_var->name = ast->data.ast_for.name->data.ast_cmd.argv[0];
        new_var->value = curr;

        variables->variables = vector_append(variables->variables, new_var);
        res = exec(ast_body);
        variables->variables =
            vector_remove(variables->variables, variables->variables->size - 1);

        free(new_var);
        // free_node(tmp);
    }

    ast++;
    return res;
}
