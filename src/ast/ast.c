#include "ast.h"

#include <err.h>
#include <stdlib.h>

#include "../utils/alloc.h"

struct ast_node *ast_new_if(enum ast_type type)
{
    // Init the ast_node structure.
    struct ast_node *ast = xmalloc(sizeof(struct ast_node));
    ast->type = type;
    ast->data.ast_if.cond = NULL;
    ast->data.ast_if.then = NULL;
    ast->data.ast_if.body = NULL;

    return ast;
}

struct ast_node *ast_new_else(enum ast_type type)
{
    type++;
    return NULL;
}

struct ast_node *ast_new_while(enum ast_type type)
{
    struct ast_node *ast = xmalloc(sizeof(struct ast_node));
    ast->type = type;
    ast->data.ast_while.cond = NULL;
    ast->data.ast_while.body = NULL;
    
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

struct function funs[] = { { NODE_IF, &ast_new_if },
                           { NODE_ELSE, &ast_new_else },
                           { NODE_COMMAND, &ast_new_cmd },
                           { NODE_WHILE, &ast_new_while },
                           { NODE_UNTIL, &ast_new_while },
                           { NODE_FOR, &ast_new_for } };

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
}

void free_node_if(struct ast_node *node)
{
    struct ast_if ast = node->data.ast_if;

    if (ast.body)
        free_node(ast.body);
    if (ast.cond)
        free_node(ast.cond);
    if (ast.then)
        free_node(ast.then);
    free(node);
}

void free_node_while(struct ast_node *node)
{
    struct ast_while ast = node->data.ast_while;

    if (ast.body)
        free_node(ast.body);
    if (ast.cond)
        free_node(ast.cond);
    free(node);
}

void free_node_for(struct ast_node *node)
{
    struct ast_for ast = node->data.ast_for;
    
    if (ast.name)
        free_node(ast.name);
    if (ast.body)
        free_node(ast.body);
    if (ast.cond)
        free_node(ast.cond);
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
    case NODE_IF:
        free_node_if(node);
        break;
    case NODE_WHILE:
        free_node_while(node);
        break;
    case NODE_UNTIL:
        free_node_while(node);
        break;
    case NODE_FOR:
        free_node_for(node);
    default:
        break;
    }
}
