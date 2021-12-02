#include "../parser.h"

extern struct parse_ast *parser;

/**
 * and_or: and_or  (('&&' | '||') ('\n')* pipeline)*
 */

struct parse_ast *parse_and_or(struct lexer *lexer)
{
    parser = parse_pipeline(lexer);
    if (parser->status != PARSER_OK)
        return parser;

    while (true)
    {
        struct token *tok = lexer_peek(lexer);
        if (tok->type != TOKEN_DOUBLE_AND && tok->type != TOKEN_DOUBLE_PIPE)
        {
            token_free(tok);
            parser->status = PARSER_OK;
            return parser;
        }

        token_free(lexer_pop(lexer));
        linebreak_while(lexer);
        parser = parse_pipeline(lexer);
        if (parser->status != PARSER_OK)
            return parser;
    }

    return parser;
}
