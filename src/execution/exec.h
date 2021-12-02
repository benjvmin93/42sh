#ifndef EXEC_H
#define EXEC_H

#include <string.h>

#include "../ast/ast.h"
#include "../utils/vector/vector.h"

int exec_all(struct vector *vector);

int exec_node_command(struct ast_node *ast);
int exec_node_if(struct ast_node *ast);
int exec(struct ast_node *ast);

#endif
