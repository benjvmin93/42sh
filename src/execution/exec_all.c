#include "exec.h"

int exec_all(struct ast_node *ast)
{
    if (!ast)
        return 0;
    switch (ast->type)
    {
    case NODE_COMMAND:
        return exec_node_command(ast);
    default:
        return 1;
    }
    return 0;
}
