#include "../parser.h"

extern struct parse_ast *parser;
/*
int is_io_number(struct token *tok)
{
    char *io[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };

    for (unsigned i = 0; i < sizeof(io) / sizeof(*io); i++)
    {
        if (!strcmp(tok->data, io[i]))
            return 1;
    }
    return 0;
}
*/
int parse_io_number(struct lexer *lexer, struct ast_node *ast_redir)
{
    int res = 0;
    struct token *tok = lexer_peek(lexer);
    int index = 0;
    if (*tok->data >= '0' && *tok->data <= '9')
    {
        struct clist *io = init_clist();
        io = app_char(io, *tok->data);
        ast_redir->data.ast_redir.io_number = atoi(io->data);
        free_clist(io);
        index = 1;
    }

    struct clist *redir_tok = init_clist();

    while (*(tok->data + index))
        redir_tok = app_char(redir_tok, *(tok->data + index++));

    char *tmp = strdup(redir_tok->data);
    struct token *new_tok = token_new(tmp);

    if (new_tok->type >= TOKEN_CHEVRON_RIGHT
        && new_tok->type <= TOKEN_DOUBLE_CHEVRON_LEFT_DASH)
    {
        res = 1;
        token_free(lexer_pop(lexer));
        token_free(lexer->current_tok);
        lexer->current_tok = new_tok;
    }
    else
        token_free(new_tok);

    free_clist(redir_tok);
    token_free(tok);

    return res;
}

/**
 * redirection: [IONUMBER] '>' WORD
        | [IONUMBER] '<' WORD
        | [IONUMBER] '>>' WORD
        | [IONUMBER] '<<' HEREDOC
        | [IONUMBER] '<<-' HEREDOC
        | [IONUMBER] '>&' WORD
        | [IONUMBER] '<&' WORD
        | [IONUMBER] '>|' WORD
        | [IONUMBER] '<>' WORD
 */
// REDIR PREF
struct parse_ast *parse_redirection_pref(struct lexer *lexer)
{
    struct ast_node *ast_redir = ast_new(NODE_REDIR);

    if (parse_io_number(lexer, ast_redir))
    {
        ast_redir->data.ast_redir.redirection = strdup(lexer->current_tok->data);

        struct token *tok = lexer_peek(lexer);
        if (tok->type == TOKEN_WORD)
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

            token_free(tok);
            token_free(lexer_pop(lexer));
            ast_redir->data.ast_redir.right = strdup(lexer->current_tok->data);

            parser->vector = vector_append(parser->vector, ast_redir);
            parser->status = PARSER_OK;
            return parser;
        }
        else
        {
            struct parse_ast *pars = send_error(lexer, tok->data);
            token_free(tok);
            free_node(ast_redir);
            return pars;
        }
        token_free(tok);
    }

    free_node(ast_redir);
    parser->status = PARSER_UNEXPECTED_TOKEN;
    return parser;
}

// REDIR CMD
struct parse_ast *parse_redir_cmd(struct lexer *lexer)
{
    struct ast_node *ast_redir = ast_new(NODE_REDIR);

    if (parse_io_number(lexer, ast_redir))
    {
        ast_redir->data.ast_redir.redirection = strdup(lexer->current_tok->data);

        struct token *tok = lexer_peek(lexer);
        if (tok->type == TOKEN_WORD)
        {
            ast_redir->data.ast_redir.left =
                parser->vector->data[parser->vector->size - 1];
            parser->vector =
                vector_remove(parser->vector, parser->vector->size - 1);

            token_free(tok);
            token_free(lexer_pop(lexer));
            ast_redir->data.ast_redir.right = strdup(lexer->current_tok->data);

            return parser;
        }
        else
        {
            struct parse_ast *pars = send_error(lexer, tok->data);
            token_free(tok);
            free_node(ast_redir);
            return pars;
        }
        token_free(tok);
    }

    free_node(ast_redir);
    parser->status = PARSER_UNEXPECTED_TOKEN;
    return parser;
}

struct parse_ast *parse_redirection_while(struct lexer *lexer)
{
    while (true)
    {
        parser = parse_redir_cmd(lexer);
        if (parser->status != PARSER_OK)
            break;
        token_free(lexer_pop(lexer));
    }
    parser->status = PARSER_OK;
    return parser;
}

// REDIR ELEMENT
struct parse_ast *parse_redirection_elt(struct lexer *lexer, int *is_word,
                                        struct ast_node *cmd)
{
    struct ast_node *ast_redir = ast_new(NODE_REDIR);

    if (parse_io_number(lexer, ast_redir))
    {
        ast_redir->data.ast_redir.redirection = strdup(lexer->current_tok->data);

        *is_word = 2;
        if (cmd)
            ast_redir->data.ast_redir.left = cmd;
        struct token *tok = lexer_peek(lexer);
        if (tok->type == TOKEN_WORD)
        {
            token_free(tok);
            token_free(lexer_pop(lexer));
            ast_redir->data.ast_redir.right = strdup(lexer->current_tok->data);

            parser->vector = vector_append(parser->vector, ast_redir);
            parser->status = PARSER_OK;
            return parser;
        }
        else
        {
            struct parse_ast *pars = send_error(lexer, tok->data);
            token_free(tok);
            free_node(ast_redir);
            return pars;
        }
        token_free(tok);
    }

    free_node(ast_redir);
    parser->status = PARSER_UNEXPECTED_TOKEN;
    return parser;
}
