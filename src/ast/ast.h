#ifndef AST_H
#define AST_H

#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include "../utils/vector/vector.h"

enum ast_type
{
    NODE_COMMAND,
    NODE_AND,
    NODE_IF,
    NODE_ELSE,
    NODE_DOUBLE_AND,
    NODE_DOUBLE_PIPE,
    NODE_PIPE
};

struct ast_cmd
{
    struct vector *cmd;
};

struct ast_if
{
    struct ast_node *cond;
    struct ast_node *then;
    struct ast_node *body;
};

struct ast_else
{
    struct ast_node *body;
};

union ast_data
{
    struct ast_cmd ast_cmd;
    struct ast_if ast_if;
    struct ast_else ast_else;
};

struct ast_node
{
    enum ast_type type;
    union ast_data data;
};

struct ast_node **ast_add(struct ast_node *node);
struct ast_node *ast_new(enum ast_type type);
void free_node(struct ast_node *node);

#endif
