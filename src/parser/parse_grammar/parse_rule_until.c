#include "../parser.h"

extern struct parse_ast *parser;

struct parse_ast *parse_ruleuntil(struct lexer *lexer)
{
    UNUSED(lexer);
    // TODO rule_until
    parser->status = PARSER_UNEXPECTED_TOKEN;
    return parser;
}
