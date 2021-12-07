#include "../../parser.h"

struct parse_ast *redir_chevron_left_and(struct lexer *lexer)
{
    parser->status = PARSER_UNEXPECTED_TOKEN;
    UNUSED(lexer);
    return parser;
}
