#include "../parser.h"

void linebreak_while(struct lexer *lexer)
{
    // struct token *tok = lexer_peek(lexer);
    struct token *tok = NULL;
    while (true)
    {
        tok = lexer_peek(lexer);
        if (tok->type != TOKEN_LINE_BREAK)
        {
            token_free(tok);
            break;
        }
        token_free(tok);
        token_free(lexer_pop(lexer));
    }
}
