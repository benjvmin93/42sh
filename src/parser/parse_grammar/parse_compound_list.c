#include "../parser.h"

extern struct parse_ast *parser;


int is_keyword(struct token *token)
{
    char *exprs[] = { "if", "then", "else", "elif", "fi", "do", "!", "!", "while", "case", "for", "{", "}", "done", "until", "esac"};

    for (unsigned i = 0; i < sizeof(exprs) / sizeof(*exprs); i++)
    {
        if (!strcmp(token->data, exprs[i]))
        {
            return 1;
        }
    }

    return 0;
}

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

    while (true)
    {
        tok = lexer_peek(lexer);
        if (tok->type != TOKEN_SEMICOLON && tok->type != TOKEN_LINE_BREAK
            && tok->type != TOKEN_AND)
            break;
        token_free(tok);
        token_free(lexer_pop(lexer));

        tok = lexer_peek(lexer);
        if (tok->type == TOKEN_LINE_BREAK)
        {
            token_free(tok);
            linebreak_while(lexer);
        }
        else
            token_free(tok);

        tok = lexer_peek(lexer);
        if (is_keyword(tok))
            break;

        token_free(tok);
        parser = parse_and_or(lexer);
        if (parser->status != PARSER_OK)
        {
            //token_free(lexer_pop(lexer));
            parser->status = PARSER_OK;
            return parser;
        }
    }

    token_free(tok);
    
    parser->status = PARSER_OK;
    return parser;
}
