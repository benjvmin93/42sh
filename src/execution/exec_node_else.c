#include "exec.h"

int exec_node_else(struct ast_node *ast)
{
    int res;
    struct ast_node *node = NULL;
    for (size_t i = 0; i < ast->data.ast_else.body->size; i++)
    {
        node = ast->data.ast_else.body->data[i];
        if (node)
            res = exec(node);
    }

    return res;
}