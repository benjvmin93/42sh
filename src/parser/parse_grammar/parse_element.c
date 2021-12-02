#include "../parser.h"

extern struct parse_ast *parser;

/**
 * element: WORD | redirection
 */

struct parse_ast *parse_element(struct lexer *lexer)
{
    struct token *tok = lexer_peek(lexer);
    if (tok->type >= TOKEN_WORD)
    {
        token_free(tok);
        token_free(lexer_pop(lexer));
        parser->status = PARSER_OK;
        return parser;
    }
    /*
    if (follow_elt(tok))    //TODO check les follow de element pour s'arreter au
    moment ou l'on rencontre le bon token {                       // EXEMPLE: if
    echo foo; then echo OK; elif echo bar then KO; else return 1 => WRONG SYNTAX
    BC NO SEMICOLON BETWEEN ELIF AND THEN. token_free(tok); return PARSER_OK;
    }*/
    token_free(tok);
    parser = parse_redirection(lexer);
    if (parser->status == PARSER_OK)
        return parser;

    return parser;
}
