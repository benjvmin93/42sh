#include "../parser.h"

/**
 * funcdec: WORD '(' ')' ('\n')* shellcmd
 */

enum parser_status parse_funcdec(struct ast_node **res,
                                        struct lexer *lexer)
{
    UNUSED(res);
    struct token *tok = lexer_peek(lexer);

    if (tok->type != TOKEN_WORD)
    {
        token_free(tok);
        return PARSER_UNEXPECTED_TOKEN;
    }
    token_free(tok);
    token_free(lexer_pop(lexer));

    tok = lexer_peek(lexer);
    if (tok->type != TOKEN_LEFT_PARENTHESIS)
    {
        token_free(tok);
        return PARSER_UNEXPECTED_TOKEN;
    }

    token_free(lexer_pop(lexer));

    tok = lexer_peek(lexer);
    if (tok->type != TOKEN_RIGHT_PARENTHESIS)
    {
        token_free(tok);
        return PARSER_UNEXPECTED_TOKEN;
    }

    token_free(lexer_pop(lexer));

    tok = lexer_peek(lexer);
    if (tok->type == TOKEN_LINE_BREAK)
        linebreak_while(lexer);

    token_free(tok);
    enum parser_status status = parse_shellcmd(res, lexer);
    if (status != PARSER_OK)
        return PARSER_UNEXPECTED_TOKEN;

    return PARSER_OK;
}
