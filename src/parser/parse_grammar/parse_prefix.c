#include "../parser.h"

/**
 * prefix: ASSIGNMENT_WORD | redirection
 */

enum parser_status parse_prefix(struct ast_node **res,
                                       struct lexer *lexer)
{
    UNUSED(res);
    if (is_assignment_word(lexer))
    {
        token_free(lexer_pop(lexer));
        return PARSER_OK;
    }

    enum parser_status status = parse_redirection(res, lexer);
    if (status == PARSER_OK)
        return status;

    return PARSER_UNEXPECTED_TOKEN;
}
