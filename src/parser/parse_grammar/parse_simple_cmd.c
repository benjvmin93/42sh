#include "../parser.h"

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
enum parser_status parse_simplecmd(struct ast_node **res, struct lexer *lexer)
{
    enum parser_status status_prefix = PARSER_UNEXPECTED_TOKEN;
    enum parser_status status;

    struct ast_node *cmd = ast_new(NODE_COMMAND);
    char **s = NULL;
    size_t i = 1;

    while (1)
    {
        status = parse_prefix(res, lexer);
        if (status_prefix == PARSER_UNEXPECTED_TOKEN && status == PARSER_OK)
            status_prefix = PARSER_OK;

        if (status != PARSER_OK)
            break;

        s = xrealloc(s, (i + 1) * sizeof(char *));
        s[i - 1] = strdup(lexer->current_tok->data);
        s[i++] = NULL;
    }

    enum parser_status status_elt = PARSER_UNEXPECTED_TOKEN;
    while (true)
    {
        status = parse_element(res, lexer);
        if (status_elt == PARSER_UNEXPECTED_TOKEN && status == PARSER_OK)
            status_elt = PARSER_OK;

        if (status != PARSER_OK)
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
        return PARSER_UNEXPECTED_TOKEN;
    }

    cmd->data.ast_cmd.argv = s;
    *res = cmd;

    return PARSER_OK;
}
