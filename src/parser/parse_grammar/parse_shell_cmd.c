#include "../parser.h"

/**
 * shellcmd: '{' compound_list '}'
 *      | '(' compound_list ')'
 *      | rule_for
 *      | rule_while
 *      | rule_until
 *      | rule_case
 *      | rule_is
 */

enum parser_status parse_shellcmd(struct ast_node **res,
                                         struct lexer *lexer)
{
    UNUSED(res);
    struct token *tok = lexer_peek(lexer);
    if (tok->type == TOKEN_LEFT_ACC)
    {
        token_free(lexer_pop(lexer));
        enum parser_status status = parse_compoundlist(res, lexer);
        if (status != PARSER_OK)
            return status;

        tok = lexer_peek(lexer);
        if (tok->type == TOKEN_RIGHT_ACC)
        {
            token_free(lexer_pop(lexer));
            return status;
        }
        else
        {
            token_free(tok);
            return PARSER_UNEXPECTED_TOKEN;
        }
    }

    if (tok->type == TOKEN_LEFT_PARENTHESIS)
    {
        token_free(lexer_pop(lexer));
        enum parser_status status = parse_compoundlist(res, lexer);
        if (status != PARSER_OK)
            return status;

        tok = lexer_peek(lexer);
        if (tok->type == TOKEN_RIGHT_PARENTHESIS)
        {
            token_free(lexer_pop(lexer));
            return status;
        }
        else
        {
            token_free(tok);
            return PARSER_UNEXPECTED_TOKEN;
        }
    }
    else
        token_free(tok);

    if (parser_rules_shellcmd(res, lexer) == PARSER_OK)
        return PARSER_OK;

    return PARSER_UNEXPECTED_TOKEN;
}
