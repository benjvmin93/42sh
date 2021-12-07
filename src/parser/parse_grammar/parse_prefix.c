#include "../parser.h"

extern struct parse_ast *parser;

/**
 * prefix: ASSIGNMENT_WORD | redirection
 */

struct parse_ast *parse_prefix(struct lexer *lexer)
{
    int assign = is_assignment_word(lexer);
    if (assign == 1)
    {
        parser->status = PARSER_OK;
        return parser;
    }
    else if (assign == -1)
    {
        parser->status = PARSER_UNEXPECTED_TOKEN;
        return parser;
    }
    else
        parser = parse_redirection(lexer);

    return parser;
}
