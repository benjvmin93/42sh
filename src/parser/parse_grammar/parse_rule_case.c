#include "../parser.h"

extern struct parse_ast *parser;

struct parse_ast *parse_rule_case(struct lexer *lexer)
{
    UNUSED(lexer);
    // TODO rule_case
    parser->status = PARSER_UNEXPECTED_TOKEN;
    return parser;
}
