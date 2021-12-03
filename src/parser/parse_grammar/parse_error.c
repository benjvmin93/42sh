#include "../parser.h"

extern struct parse_ast *parser;

void handle_parse_error(struct lexer *lexer)
{
    switch (parser->status)
    {
    default:
        warnx("Syntax error. Expecting '%s'", lexer->current_tok->data);
    }
}
