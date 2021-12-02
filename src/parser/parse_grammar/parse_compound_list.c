#include "../parser.h"

extern struct parse_ast *parser;

/**
 * compound_list: ('\n')* and_or ((';'|'&'|'\n') ('\n')* and_or)*
 * [('&'|';'|'\n') ('\n')*]
 */

struct parse_ast *parse_compoundlist(struct lexer *lexer)
{
    struct token *tok = lexer_peek(lexer);
    if (tok->type == TOKEN_LINE_BREAK)
    {
        linebreak_while(lexer);
    }

    token_free(tok);
    parser = parse_and_or(lexer);
    if (parser->status != PARSER_OK)
        return parser;

    // token_free(lexer_pop(lexer));
    while (true)
    {
        tok = lexer_peek(lexer);
        if (tok->type != TOKEN_SEMICOLON && tok->type != TOKEN_LINE_BREAK
            && tok->type != TOKEN_AND)
            break;

        if (tok->type == TOKEN_LINE_BREAK)
        {
            token_free(tok);
            linebreak_while(lexer);
        }
        token_free(tok);
        parser = parse_and_or(lexer);
        if (parser->status != PARSER_OK)
        {
            token_free(lexer_pop(lexer));
            parser->status = PARSER_OK;
            return parser;
        }
    }

    token_free(tok);
    parser->status = PARSER_OK;
    return parser;
}
