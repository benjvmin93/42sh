#include "../parser.h"


extern struct parse_ast *parser;
/**
 * list: and_or  ((';' | '&')  and_or)*  [';' | '&']
 */

struct parse_ast *parse_list(struct lexer *lexer)
{
    struct parse_ast *parser = parse_and_or(lexer);
    if (parser->status != PARSER_OK)
        return parser;

    while (true)
    {
        // have a look at the type of the next token. Stop if not interested
        struct token *tok = lexer_peek(lexer);
        if (tok->type != TOKEN_SEMICOLON && tok->type != TOKEN_AND)
        {
            token_free(tok);
            parser->status = PARSER_OK;
            return parser;
        }
        token_free(tok);
        token_free(lexer_pop(lexer));

        parser = parse_and_or(lexer);

        // parse the stuff at the right
        if (parser->status != PARSER_OK)
        {
            //free_node(tmp);
            break;
        }
    }

    parser->status = PARSER_OK;
    return parser;
}
