#include "exec.h"

int exec_node_if(struct ast_node *ast)
{
    int res;
    struct ast_node *node = NULL;
    for (size_t i = 0; i < ast->data.ast_if.cond->size; i++)
    {
        node = ast->data.ast_if.cond->data[i];
        if (node)
            res = exec(node);
    }
    if (!res)
    {
        for (size_t i = 0; i < ast->data.ast_if.then->size; i++)
            res = exec(ast->data.ast_if.then->data[i]);
    }
    else
        res = exec(ast->data.ast_if.body);
    return res;
}
