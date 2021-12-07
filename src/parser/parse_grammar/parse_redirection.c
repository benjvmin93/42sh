#include "../parser.h"

extern struct parse_ast *parser;

int is_io_number(struct token *tok)
{
    char *io[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };

    for(unsigned i = 0; i < sizeof(io) / sizeof(*io); i++)
    {
        if (!strcmp(tok->data, io[i]))
            return 1;
    }
    return 0;
}

void parse_io(struct lexer *lexer, struct token *tok, struct ast_node *ast_redir)
{
    token_free(tok);
    token_free(lexer_pop(lexer));

    ast_redir->data.ast_redir.left = strdup(lexer->current_tok->data);

    tok = lexer_peek(lexer);
}

struct parse_ast *parse_redirection_annexe(struct lexer *lexer, struct token *tok, struct ast_node *ast_redir)
{
    token_free(tok);
    token_free(lexer_pop(lexer));

    ast_redir->data.ast_redir.redirection = strdup(lexer->current_tok->data);

    tok = lexer_peek(lexer);
    if (tok->type >= TOKEN_WORD)
    {
        token_free(tok);
        token_free(lexer_pop(lexer));

        ast_redir->data.ast_redir.right = strdup(lexer->current_tok->data);
        parser->vector = vector_append(parser->vector, ast_redir);
        return parser;
    }

    token_free(tok);
    free_node(ast_redir);
    parser->status = PARSER_UNEXPECTED_TOKEN;

    // TODO Handle wrong expression. 
    parser->status = PARSER_UNEXPECTED_TOKEN;
    return parser;
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

struct parse_ast *parse_redirection(struct lexer *lexer)
{

    struct token *tok = lexer_peek(lexer);

    struct ast_node *ast_redir = ast_new(NODE_REDIR);

    if (is_io_number(tok))
    {
        /*token_free(tok);
        token_free(lexer_pop(lexer));

        ast_redir->data.ast_redir.left = strdup(lexer->current_tok->data);

        tok = lexer_peek(lexer);*/
        parse_io(lexer, tok, ast_redir);
    }

    if (tok->type >= TOKEN_CHEVRON_RIGHT && tok->type <= TOKEN_DOUBLE_CHEVRON_LEFT_DASH)
    {
        /*token_free(tok);
        token_free(lexer_pop(lexer));

        ast_redir->data.ast_redir.redirection = strdup(lexer->current_tok->data);

        tok = lexer_peek(lexer);
        if (tok->type >= TOKEN_WORD)
        {
            token_free(tok);
            token_free(lexer_pop(lexer));

            ast_redir->data.ast_redir.right = strdup(lexer->current_tok->data);
            parser->vector = vector_append(parser->vector, ast_redir);
            return parser;
        }

        token_free(tok);
        free_node(ast_redir);
        parser->status = PARSER_UNEXPECTED_TOKEN;

        // TODO Handle wrong expression. 
        parser->status = PARSER_UNEXPECTED_TOKEN;
        return parser;*/
        return parse_redirection_annexe(lexer, tok, ast_redir);
    }

    free_node(ast_redir);

    token_free(tok);
    parser->status = PARSER_UNEXPECTED_TOKEN;
    return parser;
}
