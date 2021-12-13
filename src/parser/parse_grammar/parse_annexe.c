#include "../parser.h"

extern struct parse_ast *parser;

void linebreak_while(struct lexer *lexer)
{
    struct token *tok = NULL;
    tok = lexer_peek(lexer);
    while (tok->type == TOKEN_LINE_BREAK)
    {
        token_free(tok);
        token_free(lexer_pop(lexer));
        tok = lexer_peek(lexer);
    }
    token_free(tok);
}

int quote_word(struct lexer *lexer)
{
    struct token *tok = lexer_peek(lexer);

    int index = 0;

    if (tok->data[index] == '\"')
    {
        index = strlen(tok->data);

        if (tok->data[index - 1] != '\"')
        {
            token_free(tok);
            return 0;
        }
    }
    else if (tok->data[index] == '\'')
    {
        index = strlen(tok->data);

        if (tok->data[index - 1] != '\'')
        {
            token_free(tok);
            return 0;
        }

        index = 1;

        for (int i = 1; i < index - 1; i++)
        {
            if (tok->data[i] == '\'')
            {
                token_free(tok);
                return 0;
            }
        }
    }

    token_free(tok);

    return 1;
}
