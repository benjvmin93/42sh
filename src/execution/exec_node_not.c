#include "exec.h"

int exec_node_not(struct ast_node *ast)
{
    int res = exec(ast->data.ast_not.ast_cmd);

    if (res == 0)
        return 1;
    else
        return 0;
}
