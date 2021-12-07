#include "../parser.h"

extern struct parse_ast *parser;

struct parse_ast *send_error(struct lexer *lexer, char *str)
{
        parser->status = PARSER_UNEXPECTED_TOKEN;
        token_free(lexer->current_tok);
        char *s = strdup(str);
        lexer->current_tok = token_new(s);
        return parser;
}

void handle_parse_error(struct lexer *lexer)
{
    switch (parser->status)
    {
    default:
        warnx("Syntax error. Expecting '%s'", lexer->current_tok->data);
    }
}
