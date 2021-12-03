#include "../parser.h"

struct parse_ast *parser = NULL;

struct parse_ast *init(void)
{
    parser = xmalloc(sizeof(struct parse_ast));
    parser->vector = vector_init(1, sizeof(struct ast_node));
    parser->status = PARSER_UNEXPECTED_TOKEN;

    return parser;
}

/**
 * input:  EOF | '\n' | list EOF | list '\n'
 */
struct parse_ast *parse(struct lexer *lexer)
{
    parser = init();

    // If we're at the end of file, there's no input
    struct token *tok = lexer->current_tok;
    if (tok->type == TOKEN_EOF)
    {
        parser->status = PARSER_OK;
        return parser;
    }

    if (tok->type == TOKEN_LINE_BREAK)
    {
        parser->status = PARSER_OK;
        return parser;
    }
    // try to parse an expression. if an error occured, free the
    // produced ast and return the same error code
    parser = parse_list(lexer);
    if (parser->status != PARSER_OK)
    {
        handle_parse_error(lexer);
        return parser;
    }
    // once parsing the expression is done, we should have
    // reached the end of file.
    tok = lexer_peek(lexer);
    if (tok->type == TOKEN_EOF)
    {
        token_free(tok);
        token_free(lexer_pop(lexer));
        parser->status = PARSER_OK;
        return parser;
    }
    if (tok->type == TOKEN_LINE_BREAK)
    {
        token_free(tok);
        token_free(lexer_pop(lexer));
        parser->status = PARSER_OK;
        return parser;
    }
    // if we didn't reach the end of file, it's an error
    token_free(tok);
    // printf("%s\n", lexer->input + lexer->pos);

    handle_parse_error(lexer);
    return parser;
}
