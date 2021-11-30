#include "ast.h"

#include <err.h>
#include <stdlib.h>

#include "../utils/alloc.h"

struct vector *ast_vector = NULL;

struct ast_node *ast_new_if(enum ast_type type)
{
    // Init the ast_node structure.
    struct ast_node *ast = xmalloc(sizeof(struct ast_node));
    ast->type = type;

    return ast;
}

struct ast_node *ast_new_else(enum ast_type type)
{
    type++;
    return NULL;
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
                           { NODE_COMMAND, &ast_new_cmd } };

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

// Free memory.
void free_node(struct ast_node *node)
{
    switch (node->type)
    {
    case NODE_COMMAND: {
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
        /*
case NODE_IF: {
ast_free(node->data.ast_if.body);
ast_free(node->data.ast_if.then);
ast_free(node->data.ast_if.cond);
}
case NODE_ELSE: {
ast_free(node->data.ast_else.body);
}*/
    default:
        break;
    }
}
