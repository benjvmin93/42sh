#include "../parser.h"

extern struct parse_ast *parser;

/**
 * do_group: Do compound_list Done
 */

struct parse_ast *parse_do_group(struct lexer *lexer)
{
    struct token *tok = lexer_peek(lexer);
    if (tok->type != TOKEN_DO)
    {
        token_free(tok);
        return send_error(lexer, "do");
    }

    token_free(tok);
    token_free(lexer_pop(lexer));

    parser = parse_compound_list(lexer);
    if (parser->status != PARSER_OK)
        return parser;

    tok = lexer_peek(lexer);
    if (tok->type == TOKEN_DONE)
    {
        token_free(tok);
        token_free(lexer_pop(lexer));
        parser->status = PARSER_OK;
        return parser;
    }

    token_free(tok);
    return send_error(lexer, "done");
}
