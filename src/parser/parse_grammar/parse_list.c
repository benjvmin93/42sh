#include "../parser.h"

extern struct parse_ast *parser;

struct parse_ast *parse_list_annexe(struct token *tok)
{
    token_free(tok);
    return parser;
}
/**
 * list: and_or  ((';' | '&')  and_or)*  [';' | '&']
 */

struct parse_ast *parse_list(struct lexer *lexer)
{
    struct parse_ast *parser = parse_and_or(lexer);
    if (parser->status != PARSER_OK)
        return parser;

    struct token *tok;
    while (true)
    {
        tok = lexer_peek(lexer);
        if (tok->type == TOKEN_EOF || tok->type == TOKEN_LINE_BREAK)
        {
            /*token_free(tok);
            return parser;*/
            return parse_list_annexe(tok);
        }

        token_free(tok);
        // have a look at the type of the next token. Stop if not interested
        tok = lexer_peek(lexer);
        if (tok->type != TOKEN_SEMICOLON && tok->type != TOKEN_AND)
        {
            parser->status = PARSER_UNEXPECTED_TOKEN;
            /*token_free(tok);
            return parser;*/
            return parse_list_annexe(tok);
        }

        token_free(lexer_pop(lexer));
        token_free(tok);

        tok = lexer_peek(lexer);
        if (tok->type == TOKEN_EOF || tok->type == TOKEN_LINE_BREAK)
        {
            /*token_free(tok);
            return parser;*/
            return parse_list_annexe(tok);
        }

        token_free(tok);
        parser = parse_and_or(lexer);

        // parse the stuff at the right
        if (parser->status != PARSER_OK)
        {
            // free_node(tmp);
            return parser;
        }
    }

    parser->status = PARSER_OK;
    return parser;
}
