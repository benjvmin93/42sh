#include "../parser.h"

extern struct parse_ast *parser;

void linebreak_while(struct lexer *lexer)
{
    // struct token *tok = lexer_peek(lexer);
    struct token *tok = NULL;
    tok = lexer_peek(lexer);
    while (tok->type != TOKEN_LINE_BREAK)
    {
        token_free(tok);
        token_free(lexer_pop(lexer));
        tok = lexer_peek(lexer);
    }
    token_free(tok);
}
