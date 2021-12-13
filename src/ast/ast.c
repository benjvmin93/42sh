#include "ast.h"

#include <err.h>
#include <stdlib.h>

#include "../utils/alloc.h"

struct ast_node *ast_new_var(enum ast_type type)
{
    // Init the ast_node structure.
    struct ast_node *ast = xmalloc(sizeof(struct ast_node));
    ast->type = type;
    ast->data.ast_var.name = NULL;
    ast->data.ast_var.value = NULL;

    return ast;
}

struct ast_node *ast_new_pipeline(enum ast_type type)
{
    // Init the ast_node structure.
    struct ast_node *ast = xmalloc(sizeof(struct ast_node));
    ast->type = type;
    ast->data.ast_pipeline.argv = vector_init(1, sizeof(struct ast_node));

    return ast;
}

struct ast_node *ast_new_redir(enum ast_type type)
{
    // Init the ast_node structure.
    struct ast_node *ast = xmalloc(sizeof(struct ast_node));
    ast->type = type;
    ast->data.ast_redir.left = NULL;
    ast->data.ast_redir.redirection = NULL;
    ast->data.ast_redir.right = NULL;
    ast->data.ast_redir.io_number = -1;

    return ast;
}

struct ast_node *ast_new_if(enum ast_type type)
{
    // Init the ast_node structure.
    struct ast_node *ast = xmalloc(sizeof(struct ast_node));
    ast->type = type;
    ast->data.ast_if.cond = vector_init(1, sizeof(struct ast_node));
    ast->data.ast_if.then = vector_init(1, sizeof(struct ast_node));
    ast->data.ast_if.body = NULL;

    return ast;
}

struct ast_node *ast_new_else(enum ast_type type)
{
    struct ast_node *ast = xmalloc(sizeof(struct ast_node));
    ast->type = type;
    ast->data.ast_else.body = vector_init(1, sizeof(struct ast_node));

    return ast;
}

struct ast_node *ast_new_while(enum ast_type type)
{
    struct ast_node *ast = xmalloc(sizeof(struct ast_node));
    ast->data.ast_while.cond = vector_init(1, sizeof(struct ast_node));
    ast->data.ast_while.body = vector_init(1, sizeof(struct ast_node));
    ast->type = type;

    return ast;
}

struct ast_node *ast_new_for(enum ast_type type)
{
    struct ast_node *ast = xmalloc(sizeof(struct ast_node));
    ast->type = type;
    ast->data.ast_for.name = NULL;
    ast->data.ast_for.cond = NULL;
    ast->data.ast_for.body = NULL;

    return ast;
}

struct ast_node *ast_new_not(enum ast_type type)
{
    struct ast_node *ast = xmalloc(sizeof(struct ast_node));
    ast->type = type;
    ast->data.ast_not.ast_cmd = NULL;

    return ast;
}

struct ast_node *ast_new_andor(enum ast_type type)
{
    struct ast_node *ast = xmalloc(sizeof(struct ast_node));
    ast->type = type;
    ast->data.ast_andor.list_ast_node = vector_init(1, sizeof(struct ast_node));
    ast->data.ast_andor.list_operator = xmalloc(sizeof(int));
    ast->data.ast_andor.size = 0;

    return ast;
}

// Creates a new ast_cmd inside a struct ast_node
struct ast_node *ast_new_cmd(enum ast_type type)
{
    // Init the ast_node structure.
    struct ast_node *ast = xmalloc(sizeof(struct ast_node));
    ast->type = type;

    // Allocates the ast_cmd structure
    // and init a command vector of size 1 inside that structure.
    // ast->data = xmalloc(sizeof(struct ast_cmd));
    ast->data.ast_cmd.argv = NULL;

    return ast;
}

struct function
{
    enum ast_type type;
    struct ast_node *(*fun)(enum ast_type);
};

struct function funs[] = {
    { NODE_IF, &ast_new_if },        { NODE_ELSE, &ast_new_else },
    { NODE_COMMAND, &ast_new_cmd },  { NODE_WHILE, &ast_new_while },
    { NODE_UNTIL, &ast_new_while },  { NODE_FOR, &ast_new_for },
    { NODE_REDIR, &ast_new_redir },  { NODE_NOT, &ast_new_not },
    { NODE_AND_OR, &ast_new_andor }, { NODE_VAR, &ast_new_var }
};

// Iterate through the function struct array funs
// in order to find the specific ast_init function according to the type.
struct ast_node *ast_new(enum ast_type type)
{
    for (unsigned i = 0; i < sizeof(funs) / sizeof(*funs); i++)
    {
        if (type == funs[i].type)
            return funs[i].fun(type);
    }

    return NULL;
}

void free_node_var(struct ast_node *node)
{
    struct ast_var ast = node->data.ast_var;

    if (ast.name)
        free(ast.name);
    if (ast.value)
        free(ast.value);
    free(node);
}

void free_node_pipeline(struct ast_node *node)
{
    struct vector *vect = node->data.ast_pipeline.argv;
    for (size_t i = 0; i < vect->size; i++)
    {
        struct ast_node *tmp = vect->data[i];
        free_node(tmp);
    }

    free(vect->data);
    free(vect);
    free(node);
}

void free_node_cmd(struct ast_node *node)
{
    char **argv = node->data.ast_cmd.argv;
    if (argv)
    {
        while (*argv)
        {
            char *tmp = *argv;
            free(tmp);
            argv++;
        }
    }
    free(node->data.ast_cmd.argv);
    free(node);
    node = NULL;
}

void free_node_if(struct ast_node *node)
{
    struct ast_if ast = node->data.ast_if;

    if (ast.cond)
        vector_destroy(ast.cond);
    if (ast.then)
        vector_destroy(ast.then);
    if (ast.body)
        free_node(ast.body);
    free(node);
}

void free_node_else(struct ast_node *node)
{
    struct ast_else ast = node->data.ast_else;

    if (ast.body)
        vector_destroy(ast.body);
    free(node);
}

void free_node_while(struct ast_node *node)
{
    struct ast_while ast = node->data.ast_while;

    if (ast.body)
        vector_destroy(ast.body);
    if (ast.cond)
        vector_destroy(ast.cond);
    free(node);
}

void free_node_for(struct ast_node *node)
{
    struct ast_for ast = node->data.ast_for;

    if (ast.name)
        free_node(ast.name);
    if (ast.cond)
        free_node(ast.cond);
    if (ast.body)
        free_node(ast.body);
    free(node);
}

void free_node_redir(struct ast_node *node)
{
    struct ast_redir ast = node->data.ast_redir;

    if (ast.left)
        free_node(ast.left);
    if (ast.redirection)
        free(ast.redirection);
    if (ast.right)
        free(ast.right);
    free(node);
}

void free_node_andor(struct ast_node *node)
{
    struct ast_andor ast = node->data.ast_andor;

    if (ast.list_ast_node)
        vector_destroy(ast.list_ast_node);

    free(ast.list_operator);
    free(node);
}

// Free memory.
void free_node(struct ast_node *node)
{
    if (!node)
        return;

    switch (node->type)
    {
    case NODE_COMMAND:
        free_node_cmd(node);
        break;
    case NODE_AND_OR:
        free_node_andor(node);
        break;
    case NODE_IF:
        free_node_if(node);
        break;
    case NODE_ELSE:
        free_node_else(node);
        break;
    case NODE_WHILE:
        free_node_while(node);
        break;
    case NODE_FOR:
        free_node_for(node);
        break;
    case NODE_UNTIL:
        free_node_while(node);
        break;
    case NODE_REDIR:
        free_node_redir(node);
        break;
    case NODE_PIPELINE:
        free_node_pipeline(node);
        break;
    case NODE_VAR:
        free_node_var(node);
    default:
        break;
    }
}
