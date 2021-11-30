#include "../parser.h"

enum parser_status parse_rulecase(struct ast_node **res,
                                         struct lexer *lexer)
{
    UNUSED(res);
    UNUSED(lexer);
    // TODO rule_case
    return PARSER_UNEXPECTED_TOKEN;
}
