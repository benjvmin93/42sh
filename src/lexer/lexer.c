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

char *token_process(char *token, struct lexer *lexer, size_t *i)
{
    token[*i] = lexer->input[lexer->pos];
    *i += 1;
    token = xrealloc(token, ((*i) + 1) * sizeof(char));
    token[*i] = 0;
    lexer->pos++;

    return token;
}

char *lex_quote(char *token, struct lexer *lexer)
{
    struct clist *res = init_clist();

    res = app_str(res, token);
    free(token);

    res = app_char(res, lexer->input[lexer->pos]);

    char quote = '\"';
    if (lexer->input[lexer->pos] == '\'')
        quote = '\'';

    lexer->pos++;
    for (; lexer->input[lexer->pos] && lexer->input[lexer->pos] != quote;
         lexer->pos++)
    {
        res = app_char(res, lexer->input[lexer->pos]);
    }

    res = app_char(res, lexer->input[lexer->pos++]);

    char *str = strdup(res->data);
    free_clist(res);

    return str;
}

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
        if (lexer->input[lexer->pos] == '\''
            || lexer->input[lexer->pos] == '\"')
        {
            token = lex_quote(token, lexer);
            break;
        }
        if (lexer->input[lexer->pos] == ' ')
            break;

        // CUT_TOKEN_1 SUBFUNCTION.
        if (lexer->input[lexer->pos] == ';'
                || lexer->input[lexer->pos] == '&'
                || lexer->input[lexer->pos] == '|')
        {
            if (*token)
                break;
            token = token_process(token, lexer, &i);

            if (lexer->input[lexer->pos]
                && lexer->input[lexer->pos - 1] == lexer->input[lexer->pos])
                token = token_process(token, lexer, &i);
            break;
        }
        if (lexer->input[lexer->pos] == '\n')
        {
            if (!*token)
                token = token_process(token, lexer, &i);
            break;
        }

        // CUT_TOKEN_2 SUBFUNCTION.
        if (lexer->input[lexer->pos] == '>' || lexer->input[lexer->pos] == '<')
        {
            token = token_process(token, lexer, &i);
            if (lexer->input[lexer->pos] == '<'
                || lexer->input[lexer->pos] == '>'
                || lexer->input[lexer->pos] == '&'
                || lexer->input[lexer->pos] == '|')
                    token = token_process(token, lexer, &i);
            break;
        }

        token = token_process(token, lexer, &i);
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
    lex->expecting_tok = NULL;
    // lex->pos += strlen(lex->current_tok->data);
    return lex;
}

void lexer_free(struct lexer *lexer)
{
    if (lexer->current_tok)
        token_free(lexer->current_tok);
    if (lexer->expecting_tok)
        token_free(lexer->expecting_tok);
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
    char *s = strdup("echo a <& cat -e >& file.txt");
    struct lexer *lexer = lexer_new(s);
    struct token *token = lexer_pop(lexer);
    while (token->type != TOKEN_EOF)
    {
        printf("token: '%s'\n", token->data);
        token_free(token);
        token = lexer_pop(lexer);
    }


    printf("%s\n", token->data);
    lexer_free(lexer);
}
*/
