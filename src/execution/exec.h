#ifndef EXEC_H
#define EXEC_H

#include <string.h>

#include "../ast/ast.h"
#include "../expands/expands.h"
#include "../utils/vector/vector.h"

int exec_all(struct vector *vector);

int exec_node_command(struct ast_node *ast);
int exec_node_if(struct ast_node *ast);
int exec_node_else(struct ast_node *ast);
int exec_node_while(struct ast_node *ast);
int exec_node_redirection(struct ast_node *ast);
int exec_node_for(struct ast_node *ast);
int exec_node_pipeline(struct ast_node *ast);
int exec_node_var(struct ast_node *ast);
int exec_node_not(struct ast_node *ast);
int exec_node_andor(struct ast_node *ast);
int exec(struct ast_node *ast);

struct ast_node *cpy_node(struct ast_node *ast);

#endif
