#include "../../parser.h"

struct parse_ast *redir_double_chevron_right(struct lexer *lexer)
{
    parser->status = PARSER_UNEXPECTED_TOKEN;
    UNUSED(lexer);
    return parser;
}
