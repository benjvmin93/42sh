#include "../parser.h"

enum parser_status parse_rulewhile(struct ast_node **res, struct lexer *lexer)
{
    UNUSED(res);
    UNUSED(lexer);
    // TODO rule_while
    return PARSER_UNEXPECTED_TOKEN;
}
