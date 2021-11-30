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

enum parser_status parse_redirection(struct ast_node **res,
                                            struct lexer *lexer)
{
    UNUSED(res);
    UNUSED(lexer);
    // TODO redirection
    return PARSER_UNEXPECTED_TOKEN;
}

int is_assignment_word(struct lexer *lexer)
{
    struct token *tok = lexer_peek(lexer);
    if (!strcmp(tok->data, "echo"))
    {
        token_free(tok);
        return 1;
    }
    token_free(tok);

    return 0;
}
