#include "../parser.h"

extern struct parse_ast *parser;

struct parse_ast *parse_rulewhile(struct lexer *lexer)
{
    UNUSED(lexer);
    // TODO rule_while
    parser->status = PARSER_UNEXPECTED_TOKEN;
    return parser;
}
