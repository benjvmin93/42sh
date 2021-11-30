#include "../parser.h"

/**
 * list: and_or  ((';' | '&')  and_or)*  [';' | '&']
 */

enum parser_status parse_list(struct ast_node **res, struct lexer *lexer)
{
    UNUSED(res);
    enum parser_status status = parse_and_or(res, lexer);
    if (status != PARSER_OK)
        return status;

    while (true)
    {
        // have a look at the type of the next token. Stop if not interested
        struct token *tok = lexer_peek(lexer);
        if (tok->type != TOKEN_SEMICOLON && tok->type != TOKEN_AND)
        {
            token_free(tok);
            return PARSER_OK;
        }
        token_free(tok);
        token_free(lexer_pop(lexer));

        // parse the stuff at the right
        if ((status = parse_and_or(res, lexer)) != PARSER_OK)
            break;
    }

    return PARSER_OK;
}
