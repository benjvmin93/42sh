#include "../parser.h"

extern struct parse_ast *parser;

struct parse_ast *send_error(struct lexer *lexer, char *str)
{
    parser->status = PARSER_EXPECTING_TOKEN;
    char *s = strdup(str);
    lexer->expecting_tok = token_new(s);
    return parser;
}

void handle_parse_error(struct lexer *lexer, int index_error)
{
    struct clist *s = init_clist();
    if (lexer->current_tok->type == TOKEN_EOF)
    {
        s = app_str(s, "end of file");
    }
    else if (lexer->current_tok->type == TOKEN_LINE_BREAK)
    {
        s = app_str(s, "`newline'");
    }
    else
    {
        s = app_char(s, '"');
        s = app_str(s, lexer->current_tok->data);
        s = app_char(s, '"');
    }

    if (lexer->expecting_tok)
    {
        if ((!strcmp(lexer->expecting_tok->data, "\""))
            || (!strcmp(lexer->expecting_tok->data, "\'")))
        {
            warnx("%d: Syntax error: Unterminated quoted string", index_error);
            free_clist(s);
            return;
        }
    }

    if (parser->status == PARSER_EXPECTING_TOKEN)
        warnx("%d: Syntax error: %s unexpected (expecting \"%s\")", index_error,
              s->data, lexer->expecting_tok->data);
    else
        warnx("%d: Syntax error: %s unexpected", index_error, s->data);

    free_clist(s);
}
