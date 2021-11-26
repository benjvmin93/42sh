#ifndef AST_H
#define AST_H

#include "../lexer/token.h"
#include "../lexer/lexer.h"

enum ast_type
{
    NODE_LINEBREAK,
    NODE_SEMICOLON,
    NODE_AND,
    NODE_IF,
    NODE_ELSE,
    NODE_DOUBLE_AND,
    NODE_DOUBLE_PIPE,
    NODE_PIPE
};

struct ast_cmd
{
    char **argv;
};

struct ast_if
{
    struct ast *cond;
    struct ast *then;
    struct ast *body;
};

struct ast_simple_quote
{
    struct ast *cond;
    struct ast *body;
};

struct ast_else
{
    struct ast *body;
};

struct ast_elif
{
    struct ast *condition;
    struct ast *body;
};

struct ast_fi
{
    struct ast *body;
};

struct ast_semicolon
{
    struct ast *body;
};

struct ast_line_break
{
    struct ast *body;
};

struct ast_word
{
    struct ast *body;
};

struct ast
{
    enum ast_type type;
    void *data;
    struct ast *left;
    struct ast *right;
};

union ast_data
{
    struct ast_cmd ast_cmd;
    struct ast_if ast_if;
    struct ast_simple_quote ast_simple_quote;
    struct ast_else ast_else;
    struct ast_elif ast_elif;
    struct ast_fi ast_fi;
    struct ast_semicolon ast_semicolon;
    struct ast_line_break ast_line_break;
    struct ast_word ast_word;
};

struct ast_node
{
    enum ast_type type;
    union ast_data data;
};

struct ast *ast_new(enum ast_type type);
void ast_free(struct ast *ast);

#endif
