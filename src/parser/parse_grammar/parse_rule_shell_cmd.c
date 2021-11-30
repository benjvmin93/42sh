#include "../parser.h"

/**
 * Sous foction de shellcmd pour les rules
 */

enum parser_status parser_rules_shellcmd(struct ast_node **res,
                                         struct lexer *lexer)
{
    UNUSED(res);
    struct token *tok = lexer_peek(lexer);
    if (tok->type == TOKEN_FOR)
    {
        enum parser_status status = parse_rulefor(res, lexer);
        if (status == PARSER_OK)
        {
            token_free(tok);
            token_free(lexer_pop(lexer));
            return status;
        }
    }

    if (tok->type == TOKEN_WHILE)
    {
        enum parser_status status = parse_rulewhile(res, lexer);
        if (status == PARSER_OK)
        {
            token_free(tok);
            token_free(lexer_pop(lexer));
            return status;
        }
    }

    if (tok->type == TOKEN_UNTIL)
    {
        enum parser_status status = parse_ruleuntil(res, lexer);
        if (status == PARSER_OK)
        {
            token_free(tok);
            token_free(lexer_pop(lexer));
            return status;
        }
    }

    if (tok->type == TOKEN_CASE)
    {
        enum parser_status status = parse_rulecase(res, lexer);
        if (status == PARSER_OK)
        {
            token_free(tok);
            token_free(lexer_pop(lexer));
            return status;
        }
    }

    if (tok->type == TOKEN_IF)
    {
        enum parser_status status = parse_ruleif(res, lexer);
        if (status == PARSER_OK)
        {
            token_free(tok);
            token_free(lexer_pop(lexer));
            return status;
        }
    }

    token_free(tok);
    return PARSER_UNEXPECTED_TOKEN;
}
