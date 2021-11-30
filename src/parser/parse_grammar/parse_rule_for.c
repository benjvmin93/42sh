#include "../parser.h"

enum parser_status parse_rulefor(struct ast_node **res,
                                        struct lexer *lexer)
{
    UNUSED(res);
    UNUSED(lexer);
    // TODO rule_for
    return PARSER_UNEXPECTED_TOKEN;
}
