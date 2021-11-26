#define _POSIX_C_SOURCE 200809L

#include "token.h"
#include "../utils/alloc.h"

#include <string.h>
#include <err.h>
#include <stdlib.h>

enum token_type token_type(char *str)
{
    static struct token_model exprs[] = { { "if", TOKEN_IF },
                                          { "then", TOKEN_THEN },
                                          { "else", TOKEN_ELSE },
                                          { "elif", TOKEN_ELIF },
                                          { "fi", TOKEN_FI },
                                          { ";", TOKEN_SEMICOLON },
                                          { ";;", TOKEN_DOUBLE_SEMICOLON },
                                          { "\n", TOKEN_LINE_BREAK },
                                          { "'", TOKEN_SIMPLE_QUOTE },
                                          { "&", TOKEN_AND },
                                          { "&&", TOKEN_DOUBLE_AND },
                                          { "|", TOKEN_PIPE },
                                          { "||", TOKEN_DOUBLE_PIPE },
                                          { "(", TOKEN_LEFT_PARENTHESIS },
                                          { ")", TOKEN_RIGHT_PARENTHESIS },
                                          { "{", TOKEN_LEFT_ACC },
                                          { "}", TOKEN_RIGHT_ACC },
                                          { "EOF", TOKEN_EOF } };

    for (unsigned i = 0; i < sizeof(exprs) / sizeof(*exprs); i++)
    {
        if (!strcmp(str, exprs[i].str))
            return exprs[i].token_type;
    }

    return TOKEN_WORD;
}

struct token *token_new(char *input)
{
    struct token *new = zalloc(sizeof(struct token));
    new->type = token_type(input);
    new->data = strdup(input);
    free(input);

    return new;
}

void token_free(struct token *token)
{
    free(token->data);
    free(token);
}
