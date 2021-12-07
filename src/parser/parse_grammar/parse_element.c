#include "../parser.h"

extern struct parse_ast *parser;

/**
 * element: WORD | redirection
 */

struct parse_ast *parse_element(struct lexer *lexer)
{
    struct token *tok = lexer_peek(lexer);
    if (tok->type >= TOKEN_WORD)
    {
        token_free(tok);
        token_free(lexer_pop(lexer));
        parser->status = PARSER_OK;
        return parser;
    }

    token_free(tok);
    parser = parse_redirection(lexer);
    if (parser->status == PARSER_OK)
        return parser;

    return parser;
}
