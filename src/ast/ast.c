#include "ast.h"

#include <err.h>
#include <stdlib.h>

#include "../utils/alloc.h"

struct ast *ast_new_if(enum ast_type type)
{
    type++;
    return NULL;
}

struct ast *ast_new_else(enum ast_type type)
{
    type++;
    return NULL;
}

struct function
{
    enum ast_type type;
    struct ast *(*fun)(enum ast_type);
};

struct function funs[] = {
    { NODE_IF, &ast_new_if },
    { NODE_ELSE, &ast_new_else }
};

struct ast *ast_new(enum ast_type type)
{
    //If type is a unique ast
    for (unsigned i = 0; i < sizeof(funs) / sizeof(*funs); i++)
    {
        if (type == funs[i].type)
            return funs[i].fun(type);
    }

    //If type is a basic node
    struct ast *new = zalloc(sizeof(struct ast));
    new->type = type;
    return new;
}

void ast_free(struct ast *ast)
{
    if (ast == NULL)
        return;

    ast_free(ast->left);
    ast->left = NULL;

    ast_free(ast->right);
    ast->right = NULL;

    free(ast);
}
