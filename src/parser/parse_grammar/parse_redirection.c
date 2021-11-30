#include "../parser.h"

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

enum parser_status parse_redirection(struct ast_node **res, struct lexer *lexer)
{
    UNUSED(res);
    UNUSED(lexer);
    // TODO redirection
    return PARSER_UNEXPECTED_TOKEN;
}
