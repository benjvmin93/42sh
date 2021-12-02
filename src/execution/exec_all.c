#include "exec.h"

int exec(struct ast_node *ast)
{
    switch (ast->type)
        {
        case NODE_COMMAND:
            return exec_node_command(ast);
        case NODE_IF:
            return exec_node_if(ast);
        default:
            return 1;
        }
}

int exec_all(struct vector *vector)
{
    int res = 0;
    for (size_t i = 0; i < vector->size; i++)
        res = exec(vector->data[i]);

    return res;
}
