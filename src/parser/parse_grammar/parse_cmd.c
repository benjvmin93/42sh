#include "../parser.h"

/**
 * command: simplecmd | shellcmd (redirection)* | funcdec (redirection)*
 */

enum parser_status parse_cmd(struct ast_node **res, struct lexer *lexer)
{
    UNUSED(res);
    enum parser_status status = parse_simplecmd(res, lexer);
    if (status == PARSER_OK)
        return status;

    status = parse_shellcmd(res, lexer);
    if (status == PARSER_OK)
    {
        while (true)
        {
            status = parse_redirection(res, lexer);
            if (status != PARSER_OK)
                break;
            token_free(lexer_pop(lexer));
        }
        return PARSER_OK;
    }

    status = parse_funcdec(res, lexer);
    if (status == PARSER_OK)
    {
        while (true)
        {
            status = parse_redirection(res, lexer);
            if (status != PARSER_OK)
                break;
            token_free(lexer_pop(lexer));
        }
        return PARSER_OK;
    }

    return PARSER_UNEXPECTED_TOKEN;
}
