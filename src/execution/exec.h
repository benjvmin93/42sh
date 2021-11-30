#ifndef EXEC_H
#define EXEC_H

#include <string.h>

#include "../ast/ast.h"
#include "../utils/vector/vector.h"

int exec_all(struct ast_node *ast);

int exec_node_command(struct ast_node *ast);

#endif
