#include "../parser.h"

extern struct parse_ast *parser;

/**
 * shellcmd: '{' compound_list '}'
 *      | '(' compound_list ')'
 *      | rule_for
 *      | rule_while
 *      | rule_until
 *      | rule_case
 *      | rule_is
 */

struct parse_ast *parse_shellcmd(struct lexer *lexer)
{
    struct token *tok = lexer_peek(lexer);
    if (tok->type == TOKEN_LEFT_ACC)
    {
        token_free(lexer_pop(lexer));
        parser = parse_compoundlist(lexer);
        if (parser->status != PARSER_OK)
            return parser;

        tok = lexer_peek(lexer);
        if (tok->type == TOKEN_RIGHT_ACC)
        {
            token_free(lexer_pop(lexer));
            return parser;
        }
        else
        {
            token_free(tok);
            parser->status = PARSER_UNEXPECTED_TOKEN;
            return parser;
        }
    }

    if (tok->type == TOKEN_LEFT_PARENTHESIS)
    {
        token_free(lexer_pop(lexer));
        parser = parse_compoundlist(lexer);
        if (parser->status != PARSER_OK)
            return parser;

        tok = lexer_peek(lexer);
        if (tok->type == TOKEN_RIGHT_PARENTHESIS)
        {
            token_free(lexer_pop(lexer));
            return parser;
        }
        else
        {
            token_free(tok);
            parser->status = PARSER_UNEXPECTED_TOKEN;
            return parser;
        }
    }
    else
        token_free(tok);

    parser = parser_rules_shellcmd(lexer);

    return parser;
}
