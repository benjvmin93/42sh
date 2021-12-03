#include "../parser.h"

extern struct parse_ast *parser;

/**
 * do_group: Do compound_list Done
 */

struct parse_ast *parse_dogroup(struct lexer *lexer)
{
    struct token *tok = lexer_peek(lexer);
    if (tok->type != TOKEN_DO)
    {
        token_free(tok);
        parser->status = PARSER_UNEXPECTED_TOKEN;
        token_free(lexer->current_tok);
        char *s = strdup("do");
        lexer->current_tok = token_new(s);
        return parser;
    }

    token_free(tok);
    token_free(lexer_pop(lexer));

    parser = parse_compoundlist(lexer);
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
    
    parser->status = PARSER_UNEXPECTED_TOKEN;
    token_free(lexer->current_tok);
    char *s = strdup("done");
    lexer->current_tok = token_new(s);

    return parser;
}
