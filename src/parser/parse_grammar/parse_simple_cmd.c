#include "../parser.h"

extern struct parse_ast *parser;

int is_assignment_word(struct lexer *lexer)
{
    char *exprs[] = { "echo", "ls" };

    struct token *tok = lexer_peek(lexer);
    for (unsigned i = 0; i < sizeof(exprs) / sizeof(*exprs); i++)
    {
        if (!strcmp(tok->data, exprs[i]))
        {
            token_free(tok);
            return 1;
        }
    }

    token_free(tok);
    return 0;
}

/**
 * simplemcd: (prefix)+ | (prefix)* (element)+
 */
struct parse_ast *parse_simplecmd(struct lexer *lexer)
{
    int status_prefix = PARSER_UNEXPECTED_TOKEN;

    struct ast_node *cmd = ast_new(NODE_COMMAND);
    char **s = NULL;
    size_t i = 1;

    while (1)
    {
        parser = parse_prefix(lexer);
        if (status_prefix == PARSER_UNEXPECTED_TOKEN
            && parser->status == PARSER_OK)
            status_prefix = PARSER_OK;

        if (parser->status != PARSER_OK)
            break;

        s = xrealloc(s, (i + 1) * sizeof(char *));
        s[i - 1] = strdup(lexer->current_tok->data);
        s[i++] = NULL;
    }

    int status_elt = PARSER_UNEXPECTED_TOKEN;
    while (true)
    {
        parser = parse_element(lexer);
        if (status_elt == PARSER_UNEXPECTED_TOKEN
            && parser->status == PARSER_OK)
            status_elt = PARSER_OK;

        if (parser->status != PARSER_OK)
            break;

        if (s)
        {
            s = xrealloc(s, (i + 1) * sizeof(char *));
            s[i - 1] = strdup(lexer->current_tok->data);
            s[i++] = NULL;
        }
    }

    if (status_prefix != PARSER_OK && status_elt != PARSER_OK)
    {
        free_node(cmd);
        free(s);
        parser->status = PARSER_UNEXPECTED_TOKEN;
        return parser;
    }

    cmd->data.ast_cmd.argv = s;
    parser->vector = vector_append(parser->vector, cmd);
    parser->status = PARSER_OK;
    return parser;
}
