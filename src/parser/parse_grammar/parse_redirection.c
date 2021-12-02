#include "../parser.h"

extern struct parse_ast *parser;

/**
 * redirection: [IONUMBER] '>' WORD
        | [IONUMBER] '<' WORD
        | [IONUMBER] '>>' WORD
        | [IONUMBER] '<<' HEREDOC
        | [IONUMBER] '<<-' HEREDOC
        | [IONUMBER] '>&' WORD
        | [IONUMBER] '<&' WORD
        | [IONUMBER] '>|' WORD
        | [IONUMBER] '<>' WORD
 */

struct parse_ast *parse_redirection(struct lexer *lexer)
{
    UNUSED(lexer);
    // TODO redirection
    parser->status = PARSER_UNEXPECTED_TOKEN;
    return parser;
}
