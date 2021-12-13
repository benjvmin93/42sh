#ifndef AST_H
#define AST_H

#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include "../utils/vector/vector.h"

enum ast_type
{
    NODE_COMMAND,
    NODE_AND_OR,
    NODE_IF,
    NODE_ELSE,
    NODE_DOUBLE_AND,
    NODE_DOUBLE_PIPE,
    NODE_NOT,
    NODE_WHILE,
    NODE_UNTIL,
    NODE_FOR,
    NODE_REDIR,
    NODE_PIPELINE,
    NODE_VAR
};

struct ast_var
{
    char *name;
    char *value;
};

struct ast_pipeline
{
    struct vector *argv;
};

struct ast_redir
{
    struct ast_node *left;
    char *redirection;
    char *right;
    int io_number;
};

struct ast_cmd
{
    char **argv;
};

enum and_or
{
    AND,
    OR,
};

/**
 * Struct andor handle && and || operators
 * @list_ast_node contains all ast_node to execute
 * @list_operator contains all enum AND_OR to know behavior
 */
struct ast_andor
{
    struct vector *list_ast_node;
    int *list_operator;
    size_t size;
};

struct ast_not
{
    struct ast_node *ast_cmd;
};

struct ast_if
{
    struct vector *cond;
    struct vector *then;
    struct ast_node *body;
};

struct ast_else
{
    struct vector *body;
};

struct ast_while
{
    struct vector *cond;
    struct vector *body;
};

struct ast_for
{
    struct ast_node *name;
    struct ast_node *cond;
    struct ast_node *body;
};

union ast_data
{
    struct ast_cmd ast_cmd;
    struct ast_if ast_if;
    struct ast_else ast_else;
    struct ast_while ast_while;
    struct ast_for ast_for;
    struct ast_redir ast_redir;
    struct ast_pipeline ast_pipeline;
    struct ast_var ast_var;
    struct ast_not ast_not;
    struct ast_andor ast_andor;
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
