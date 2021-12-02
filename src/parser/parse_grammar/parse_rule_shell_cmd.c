#include "../parser.h"

extern struct parse_ast *parser;

/**
 * Sous foction de shellcmd pour les rules
 */

struct parse_ast *parser_rules_shellcmd(struct lexer *lexer)
{
    struct token *tok = lexer_peek(lexer);
    if (tok->type == TOKEN_FOR)
    {
        parser = parse_rulefor(lexer);
        if (parser->status == PARSER_OK)
        {
            token_free(tok);
            token_free(lexer_pop(lexer));
            return parser;
        }
    }

    if (tok->type == TOKEN_WHILE)
    {
        parser = parse_rulewhile(lexer);
        if (parser->status == PARSER_OK)
        {
            token_free(tok);
            token_free(lexer_pop(lexer));
            return parser;
        }
    }

    if (tok->type == TOKEN_UNTIL)
    {
        parser = parse_ruleuntil(lexer);
        if (parser->status == PARSER_OK)
        {
            token_free(tok);
            token_free(lexer_pop(lexer));
            return parser;
        }
    }

    if (tok->type == TOKEN_CASE)
    {
        parser = parse_rulecase(lexer);
        if (parser->status == PARSER_OK)
        {
            token_free(tok);
            token_free(lexer_pop(lexer));
            return parser;
        }
    }

    if (tok->type == TOKEN_IF)
    {
        parser = parse_ruleif(lexer);
        if (parser->status == PARSER_OK)
        {
            token_free(tok);
            token_free(lexer_pop(lexer));
            return parser;
        }
    }

    token_free(tok);
    parser->status = PARSER_UNEXPECTED_TOKEN;
    return parser;
}
