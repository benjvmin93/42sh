#include "../parser.h"

/**
 * input:  EOF | '\n' | list EOF | list '\n'
 */
enum parser_status parse(struct ast_node **res, struct lexer *lexer)
{
    UNUSED(res);
    // If we're at the end of file, there's no input
    struct token *tok = lexer->current_tok;
    if (tok->type == TOKEN_EOF)
    {
        *res = NULL;
        return PARSER_OK;
    }

    if (tok->type == TOKEN_LINE_BREAK)
        return PARSER_OK;
    // try to parse an expression. if an error occured, free the
    // produced ast and return the same error code
    enum parser_status status = parse_list(res, lexer);
    if (status != PARSER_OK)
    {
        printf("%s\n", lexer->input + lexer->pos);
        return handle_parse_error(status, res);
    }
    // once parsing the expression is done, we should have
    // reached the end of file.
    tok = lexer_peek(lexer);
    if (tok->type == TOKEN_EOF)
    {
        token_free(tok);
        token_free(lexer_pop(lexer));
        return PARSER_OK;
    }
    if (tok->type == TOKEN_LINE_BREAK)
    {
        token_free(tok);
        token_free(lexer_pop(lexer));
        return PARSER_OK;
    }
    // if we didn't reach the end of file, it's an error
    token_free(tok);
    printf("%s\n", lexer->input + lexer->pos);
    return handle_parse_error(PARSER_UNEXPECTED_TOKEN, res);
}
