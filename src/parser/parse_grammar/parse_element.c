#include "../parser.h"

extern struct parse_ast *parser;

/**
 * element: WORD | redirection
 */

int is_redir(struct token *tok)
{
    int res = 0;

    struct clist *io =init_clist();
    struct clist *redir =init_clist();

    if (*tok->data >= '0' && *tok->data <= '9')
    {
        io = app_char(io, *tok->data);
        int i = 1;

        while(*(tok->data + i))
            redir = app_char(redir, *(tok->data + i++));

        if (token_type(redir->data) >= TOKEN_CHEVRON_RIGHT && token_type(redir->data) <= TOKEN_DOUBLE_CHEVRON_LEFT_DASH)
            res = 1;
    }

    free_clist(io);
    free_clist(redir);

    return res;
}

struct parse_ast *parse_element(struct lexer *lexer, int *is_word,
                                struct ast_node *cmd)
{
    struct token *tok = lexer_peek(lexer);
    if (tok->type >= TOKEN_WORD)
    {
        if (!quote_word(lexer))
        {
            struct clist *quote = init_clist();
            quote = app_char(quote, tok->data[0]);
            token_free(tok);
            parser = send_error(lexer, quote->data);
            free_clist(quote);
            return parser;
        }

        if (!is_redir(tok))
        {
            token_free(tok);
            token_free(lexer_pop(lexer));
            *is_word = 1;
            parser->status = PARSER_OK;
            return parser;
        }
    }

    if (tok->type == TOKEN_LINE_BREAK || tok->type == TOKEN_EOF)
    {
        token_free(tok);
        parser->status = PARSER_UNEXPECTED_TOKEN;
        return parser;
    }

    token_free(tok);
    parser = parse_redirection_elt(lexer, is_word, cmd);
    if (parser->status == PARSER_OK)
        return parser;

    return parser;
}
