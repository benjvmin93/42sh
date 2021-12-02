#include "exec.h"

int exec_node_if(struct ast_node *ast)
{
    int res = exec(ast->data.ast_if.cond);

    if (!res)
        res = exec(ast->data.ast_if.then);
    else
        res = exec(ast->data.ast_if.body);

    return res;
}
