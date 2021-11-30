#include "../parser.h"

/**
 * rule_if: If compound_list Then compound_list [else_clause] Fi
 */

enum parser_status parse_ruleif(struct ast_node **res,
                                       struct lexer *lexer)
{
    UNUSED(res);
    struct token *tok = lexer_peek(lexer);
    if (tok->type != TOKEN_IF && tok->type != TOKEN_ELIF)
    {
        token_free(tok);
        return PARSER_UNEXPECTED_TOKEN;
    }

    token_free(tok);
    token_free(lexer_pop(lexer));

    enum parser_status status = parse_compoundlist(res, lexer);
    if (status != PARSER_OK)
        return status;
    else
    {
        // token_free(lexer_pop(lexer));
        tok = lexer_peek(lexer);
        if (tok->type != TOKEN_THEN)
        {
            token_free(tok);
            return PARSER_UNEXPECTED_TOKEN;
        }
        token_free(lexer_pop(lexer));
        token_free(tok);
        status = parse_compoundlist(res, lexer);
        if (status != PARSER_OK)
            return status;
    }
    // token_free(lexer_pop(lexer));

    tok = lexer_peek(lexer);
    if (tok->type == TOKEN_ELSE || tok->type == TOKEN_ELIF)
        status = parse_else_clause(res, lexer);

    token_free(tok);
    tok = lexer_peek(lexer);
    if (tok->type != TOKEN_FI)
    {
        token_free(tok);
        return PARSER_UNEXPECTED_TOKEN;
    }

    token_free(tok);
    token_free(lexer_pop(lexer));
    return PARSER_OK;
}
