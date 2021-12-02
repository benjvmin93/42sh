#include "../parser.h"

extern struct parse_ast *parser;

/**
 * pipeline: ['!'] command ('|' ('\n')* command)*
 */

struct parse_ast *parse_pipeline(struct lexer *lexer)
{
    // TODO add not !
    parser = parse_cmd(lexer);
    if (parser->status != PARSER_OK)
        return parser;

    while (true)
    {
        struct token *tok = lexer_peek(lexer);
        if (tok->type != TOKEN_PIPE)
        {
            token_free(tok);
            parser->status = PARSER_OK;
            return parser;
        }
        token_free(tok);
        token_free(lexer_pop(lexer));
        while (true)
        {
            struct token *tok2 = lexer_peek(lexer);
            if (tok2->type != TOKEN_LINE_BREAK)
            {
                token_free(tok2);
                break;
            }
            token_free(tok2);
            token_free(lexer_pop(lexer));
        }
        parser = parse_cmd(lexer);
        if (parser->status != PARSER_OK)
            return parser;
    }
    return parser;
}
