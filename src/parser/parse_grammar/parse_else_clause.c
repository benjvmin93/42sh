#include "../parser.h"

extern struct parse_ast *parser;

struct parse_ast *parse_else_clause(struct lexer *lexer)
{
    struct token *tok = lexer_peek(lexer);
    if (tok->type == TOKEN_ELSE)
    {
        token_free(tok);
        token_free(lexer_pop(lexer));

        parser = parse_compoundlist(lexer);

        return parser;
    }

    if (tok->type == TOKEN_ELIF)
    {
        token_free(tok);
        token_free(lexer_pop(lexer));

        parser = parse_compoundlist(lexer);
        if (parser->status != PARSER_OK)
            return parser;

        tok = lexer_peek(lexer);
        if (tok->type != TOKEN_THEN)
        {
            token_free(tok);
            parser->status = PARSER_UNEXPECTED_TOKEN;
            return parser;
        }

        token_free(tok);
        token_free(lexer_pop(lexer));
        parser = parse_compoundlist(lexer);
        if (parser->status != PARSER_OK)
            return parser;

        tok = lexer_peek(lexer);
        if (tok->type == TOKEN_ELSE || tok->type == TOKEN_ELIF)
        {
            token_free(tok);
            parser = parse_else_clause(lexer);
            return parser;
        }
        token_free(tok);

        parser->status = PARSER_OK;
        return parser;
    }

    token_free(tok);
    parser->status = PARSER_UNEXPECTED_TOKEN;

    return parser;
}
