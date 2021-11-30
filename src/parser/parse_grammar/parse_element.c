#include "../parser.h"

/**
 * element: WORD | redirection
 */

enum parser_status parse_element(struct ast_node **res,
                                        struct lexer *lexer)
{
    UNUSED(res);
    struct token *tok = lexer_peek(lexer);
    if (tok->type >= TOKEN_WORD)
    {
        token_free(tok);
        token_free(lexer_pop(lexer));
        return PARSER_OK;
    }
    /*
    if (follow_elt(tok))    //TODO check les follow de element pour s'arreter au
    moment ou l'on rencontre le bon token {                       // EXEMPLE: if
    echo foo; then echo OK; elif echo bar then KO; else return 1 => WRONG SYNTAX
    BC NO SEMICOLON BETWEEN ELIF AND THEN. token_free(tok); return PARSER_OK;
    }*/
    token_free(tok);
    enum parser_status status = parse_redirection(res, lexer);
    if (status == PARSER_OK)
        return status;

    return PARSER_UNEXPECTED_TOKEN;
}
