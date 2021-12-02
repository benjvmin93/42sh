#include "../parser.h"

extern struct parse_ast *parser;

void handle_parse_error(void)
{
    switch (parser->status)
    {
    default:
        warnx("unexpected token");
    }
}
