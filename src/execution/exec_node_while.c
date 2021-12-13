#include "exec.h"

int exec_node_while(struct ast_node *ast)
{
    struct ast_node *node = NULL;
    node = ast->data.ast_while.cond->data[0];
    int res = exec(node);
    // Execute cond while true or false depending if it a WHILE AST or a UNTIL
    // AST
    while ((ast->type == NODE_WHILE && !res)
           || (ast->type == NODE_UNTIL && res))
    {
        for (size_t i = 0; i < ast->data.ast_while.cond->size; i++)
        {
            node = ast->data.ast_while.cond->data[i];
            if (node)
                res = exec(node);
            if (res == 127)
                return 0;
        }

        for (size_t i = 0; i < ast->data.ast_while.body->size; i++)
            res = exec(ast->data.ast_while.body->data[i]);
    }
    return 0;
}
