#include "exec.h"
#include "redirections/redirections.h"

int exec(struct ast_node *ast)
{
    if (!ast)
        return 0;

    switch (ast->type)
        {
        case NODE_COMMAND:
            return exec_node_command(ast);
        case NODE_IF:
            return exec_node_if(ast);
        case NODE_WHILE:
            return exec_node_while(ast);
        case NODE_UNTIL:
            return exec_node_while(ast);
        case NODE_FOR:
            return exec_node_for(ast);
        case NODE_REDIR:
            return exec_node_redirection(ast);
        case NODE_PIPELINE:
            return exec_node_pipeline(ast);
        case NODE_VAR:
            return exec_node_var(ast);
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
