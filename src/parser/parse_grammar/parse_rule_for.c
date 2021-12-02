#include "../parser.h"

extern struct parse_ast *parser;

struct parse_ast *parse_rulefor(struct lexer *lexer)
{
    UNUSED(lexer);
    // TODO rule_for
    parser->status = PARSER_UNEXPECTED_TOKEN;
    return parser;
}
