#define _DEFAULT_SOURCE
#define _XOPEN_SOURCE 500

#include "lexer.h"

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/alloc.h"
#include "../utils/clist.h"
#include "token.h"

char *cut_token(struct lexer *lexer)
{
    size_t index = lexer->pos;
    if (!lexer->input[lexer->pos])
        return strdup("EOF");
    while (lexer->input[lexer->pos] == ' ')
    {
        lexer->pos++;
        index++;
    }

    size_t i = 0;
    char *token = zalloc(sizeof(char));
    while (lexer->input[lexer->pos])
    {
        if (lexer->input[lexer->pos] == ' ')
        {
            break;
        }
        if (lexer->input[lexer->pos] == ';' || lexer->input[lexer->pos] == '&'
            || lexer->input[lexer->pos] == '|')
        {
            if (*token)
                break;
            token[i++] = lexer->input[lexer->pos];
            token = xrealloc(token, (i + 1) * sizeof(char));
            token[i] = 0;
            lexer->pos++;

            if (lexer->input[lexer->pos]
                && lexer->input[lexer->pos - 1] == lexer->input[lexer->pos])
            {
                token[i++] = lexer->input[lexer->pos];
                token = xrealloc(token, (i + 1) * sizeof(char));
                token[i] = 0;
                lexer->pos++;
            }
            break;
        }
        if (lexer->input[lexer->pos] == '\n')
        {
            if (*token)
                break;
            else
            {
                token[i++] = lexer->input[lexer->pos];
                token = xrealloc(token, (i + 1) * sizeof(char));
                token[i] = 0;
                lexer->pos++;
                break;
            }
        }

        token[i++] = lexer->input[lexer->pos];
        token = xrealloc(token, (i + 1) * sizeof(char));
        token[i] = 0;
        lexer->pos++;
    }

    lexer->pos = index;
    return token;
}

struct lexer *lexer_new(char *input)
{
    struct lexer *lex = xmalloc(sizeof(struct lexer));
    lex->input = strdup(input);
    free(input);
    lex->pos = 0;
    lex->current_tok = token_new(cut_token(lex));
    // lex->pos += strlen(lex->current_tok->data);
    return lex;
}

void lexer_free(struct lexer *lexer)
{
    if (lexer->current_tok)
        token_free(lexer->current_tok);
    free(lexer->input);
    free(lexer);
}

struct token *lexer_peek(struct lexer *lexer)
{
    char *token = NULL;
    struct token *tok = lexer->current_tok;
    if (tok && tok->type == TOKEN_EOF)
    {
        char *eof = strdup("EOF");
        return token_new(eof);
    }
    else
    {
        token = cut_token(lexer);

        tok = token_new(token);
        return tok;
    }
}

struct token *lexer_pop(struct lexer *lexer)
{
    struct token *token = lexer->current_tok;

    if (token->type == TOKEN_EOF)
    {
        lexer->current_tok = NULL;
        return token;
    }
    char *tok = cut_token(lexer);
    lexer->current_tok = token_new(tok);

    lexer->pos += strlen(lexer->current_tok->data);

    return token;
}
/*
int main(void)
{
    struct lexer *lexer = lexer_new("if echo 'ok'; then echo ko;; fi && echo
foobar ||"); struct token *token = lexer_pop(lexer); while (token->type !=
TOKEN_EOF)
    {
        printf("%s\n", token->data);
        token_free(token);
        token = lexer_pop(lexer);
    }


    printf("%s\n", token->data);
    lexer_free(lexer);
}
*/
