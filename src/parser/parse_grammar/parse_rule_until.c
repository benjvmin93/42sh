#include "../parser.h"

enum parser_status parse_ruleuntil(struct ast_node **res, struct lexer *lexer)
{
    UNUSED(res);
    UNUSED(lexer);
    // TODO rule_until
    return PARSER_UNEXPECTED_TOKEN;
}
