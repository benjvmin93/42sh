#include "../parser.h"

extern struct parse_ast *parser;

/**
 * prefix: ASSIGNMENT_WORD | redirection
 */

struct parse_ast *parse_prefix(struct lexer *lexer)
{
    if (is_assignment_word(lexer))
    {
        token_free(lexer_pop(lexer));
        parser->status = PARSER_OK;
        return parser;
    }

    parser = parse_redirection(lexer);

    return parser;
}
