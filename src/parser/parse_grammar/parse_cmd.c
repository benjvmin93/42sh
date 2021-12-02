#include "../parser.h"

extern struct parse_ast *parser;

/**
 * command: simplecmd | shellcmd (redirection)* | funcdec (redirection)*
 */

struct parse_ast *parse_cmd(struct lexer *lexer)
{
    parser = parse_shellcmd(lexer);
    if (parser->status == PARSER_OK)
    {
        while (true)
        {
            parser = parse_redirection(lexer);
            if (parser->status != PARSER_OK)
                break;
            token_free(lexer_pop(lexer));
        }
        parser->status = PARSER_OK;
        return parser;
    }

    parser = parse_simplecmd(lexer);
    if (parser->status == PARSER_OK)
        return parser;
        
    parser = parse_funcdec(lexer);
    if (parser->status == PARSER_OK)
    {
        while (true)
        {
            parser = parse_redirection(lexer);
            if (parser->status != PARSER_OK)
                break;
            token_free(lexer_pop(lexer));
        }
        parser->status = PARSER_OK;
        return parser;
    }

    



    parser->status = PARSER_UNEXPECTED_TOKEN;
    return parser;
}
