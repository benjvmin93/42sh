#include "../parser.h"

/**
 * and_or: and_or  (('&&' | '||') ('\n')* pipeline)*
 */

enum parser_status parse_and_or(struct ast_node **res, struct lexer *lexer)
{
    UNUSED(res);
    enum parser_status status = parse_pipeline(res, lexer);
    if (status != PARSER_OK)
        return status;

    while (true)
    {
        struct token *tok = lexer_peek(lexer);
        if (tok->type != TOKEN_DOUBLE_AND && tok->type != TOKEN_DOUBLE_PIPE)
        {
            token_free(tok);
            return PARSER_OK;
        }

        token_free(lexer_pop(lexer));
        while (true)
        {
            struct token *tok2 = lexer_peek(lexer);
            if (tok2->type != TOKEN_LINE_BREAK)
            {
                token_free(tok2);
                break;
            }
            token_free(tok2);
            token_free(lexer_pop(lexer));
        }
        if ((status = parse_pipeline(res, lexer)) != PARSER_OK)
            return status;
    }
}
