#include "../parser.h"


enum parser_status parse_else_clause(struct ast_node **res,
                                            struct lexer *lexer)
{
    UNUSED(res);
    enum parser_status status;

    struct token *tok = lexer_peek(lexer);
    if (tok->type == TOKEN_ELSE)
    {
        token_free(tok);
        token_free(lexer_pop(lexer));

        status = parse_compoundlist(res, lexer);

        return status;
    }

    if (tok->type == TOKEN_ELIF)
    {
        token_free(tok);
        token_free(lexer_pop(lexer));

        status = parse_compoundlist(res, lexer);
        if (status != PARSER_OK)
            return PARSER_UNEXPECTED_TOKEN;

        tok = lexer_peek(lexer);
        if (tok->type != TOKEN_THEN)
        {
            token_free(tok);
            return PARSER_UNEXPECTED_TOKEN;
        }

        token_free(tok);
        token_free(lexer_pop(lexer));
        status = parse_compoundlist(res, lexer);
        if (status != PARSER_OK)
            return PARSER_UNEXPECTED_TOKEN;

        tok = lexer_peek(lexer);
        if (tok->type == TOKEN_ELSE || tok->type == TOKEN_ELIF)
        {
            token_free(tok);
            status = parse_else_clause(res, lexer);
            return status;
        }
        token_free(tok);

        return PARSER_OK;
    }

    token_free(tok);
    return PARSER_UNEXPECTED_TOKEN;
}
