#include "../parser.h"

extern struct parse_ast *parser;

/**
 * command: simplecmd | shellcmd (redirection)* | funcdec (redirection)*
 */

int is_rule(struct lexer *lexer)
{
    char *exprs[] = { "{", "(", "for", "while", "until", "case", "if" };

    struct token *tok = lexer_peek(lexer);
    for (unsigned i = 0; i < sizeof(exprs) / sizeof(*exprs); i++)
    {
        if (!strcmp(tok->data, exprs[i]))
        {
            token_free(tok);
            return 1;
        }
    }

    token_free(tok);
    return 0;
}

struct parse_ast *parse_cmd(struct lexer *lexer)
{
    if (is_rule(lexer))
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
    }
    else
    {
        struct parse_ast *tmp = parser;
        parser = parse_simplecmd(lexer);
        if (parser->status == PARSER_OK)
            return parser;

        parser = tmp;
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
    }

    parser->status = PARSER_UNEXPECTED_TOKEN;
    
    return parser;
}
