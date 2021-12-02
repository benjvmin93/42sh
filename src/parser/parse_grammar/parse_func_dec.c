#include "../parser.h"

extern struct parse_ast *parser;

/**
 * funcdec: WORD '(' ')' ('\n')* shellcmd
 */

struct parse_ast *parse_funcdec(struct lexer *lexer)
{
    struct token *tok = lexer_peek(lexer);

    if (tok->type != TOKEN_WORD)
    {
        token_free(tok);
        parser->status = PARSER_UNEXPECTED_TOKEN;
        return parser;
    }
    token_free(tok);
    token_free(lexer_pop(lexer));

    tok = lexer_peek(lexer);
    if (tok->type != TOKEN_LEFT_PARENTHESIS)
    {
        token_free(tok);
        parser->status = PARSER_UNEXPECTED_TOKEN;
        return parser;
    }

    token_free(lexer_pop(lexer));

    tok = lexer_peek(lexer);
    if (tok->type != TOKEN_RIGHT_PARENTHESIS)
    {
        token_free(tok);
        parser->status = PARSER_UNEXPECTED_TOKEN;
        return parser;
    }

    token_free(lexer_pop(lexer));

    tok = lexer_peek(lexer);
    if (tok->type == TOKEN_LINE_BREAK)
        linebreak_while(lexer);

    token_free(tok);
    parser = parse_shellcmd(lexer);

    return parser;
}
