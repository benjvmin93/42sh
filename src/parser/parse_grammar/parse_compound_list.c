#include "../parser.h"

/**
 * compound_list: ('\n')* and_or ((';'|'&'|'\n') ('\n')* and_or)*
 * [('&'|';'|'\n') ('\n')*]
 */

enum parser_status parse_compoundlist(struct ast_node **res,
                                      struct lexer *lexer)
{
    UNUSED(res);
    struct token *tok = lexer_peek(lexer);
    if (tok->type == TOKEN_LINE_BREAK)
    {
        linebreak_while(lexer);
    }

    token_free(tok);
    enum parser_status status = parse_and_or(res, lexer);
    if (status != PARSER_OK)
        return status;

    // token_free(lexer_pop(lexer));
    while (true)
    {
        tok = lexer_peek(lexer);
        if (tok->type != TOKEN_SEMICOLON && tok->type != TOKEN_LINE_BREAK
            && tok->type != TOKEN_AND)
            break;

        if (tok->type == TOKEN_LINE_BREAK)
        {
            token_free(tok);
            linebreak_while(lexer);
        }
        token_free(tok);
        enum parser_status status = parse_and_or(res, lexer);
        if (status != PARSER_OK)
        {
            token_free(lexer_pop(lexer));
            return PARSER_OK;
        }
    }

    token_free(tok);
    return PARSER_OK;
}
