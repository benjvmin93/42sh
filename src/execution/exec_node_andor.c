#include "exec.h"

int exec_node_andor(struct ast_node *ast)
{
    struct ast_andor ast_andor = ast->data.ast_andor;
    struct ast_node *ast_exec = ast_andor.list_ast_node->data[0];
    int res = exec(ast_exec);

    for (size_t i = 0; i < ast_andor.size; i++)
    {
        if (res == 0 && ast_andor.list_operator[i] == OR)
            continue;
        if (res != 0 && ast_andor.list_operator[i] == AND)
            continue;

        ast_exec = ast_andor.list_ast_node->data[i + 1];
        res = exec(ast_exec);
    }

    return res;
}