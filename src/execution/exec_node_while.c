#include "exec.h"

int exec_node_while(struct ast_node *ast)
{
    int res = exec(ast->data.ast_while.cond);
     
    //Execute cond while true or false depending if it a WHILE AST or a UNTIL AST
    while ((ast->type == NODE_WHILE && !res) || (ast->type == NODE_UNTIL && res)) 
    {
        exec(ast->data.ast_while.body);
        res = exec(ast->data.ast_while.cond);
    }
    return res;
}
