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
   /* struct token *tok = lexer_peek(lexer);
    struct token* redir = NULL;

    if (*tok->data >= '0' && *tok->data <= '9')
    {
        redir = tok;
        token_free(tok);
        token_free(lexer_pop(lexer));
    }

    tok = lexer_peek(lexer);
    if (tok->type >= TOKEN_CHEVRON_RIGHT && tok->type <= TOKEN_CHEVRON_LEFT_RIGHT)
    {
        token_free(tok);
        token_free(lexer_pop(lexer));
    }
    else if (tok->type == TOKEN_DOUBLE_CHEVRON_LEFT || tok->type == TOKEN_DOUBLE_CHEVRON_LEFT_DASH)
    {
        token_free(tok);
        token_free(lexer_pop(lexer));
    }

    redir++;
*/

    UNUSED(lexer);




    // TODO redirection
    parser->status = PARSER_UNEXPECTED_TOKEN;
    return parser;
}
