#include "../parser.h"

extern struct parse_ast *parser;
extern struct variables *variables;

static char *my_strndup(const char *s, size_t n) {
    char *p;
    size_t n1;

    for (n1 = 0; n1 < n && s[n1] != '\0'; n1++)
        continue;
    p = malloc(n + 1);
    if (p != NULL) {
        memcpy(p, s, n1);
        p[n1] = '\0';
    }
    return p;
}

int is_assignment_word(struct lexer *lexer)
{
    struct token *tok = lexer_peek(lexer);
    char *str = tok->data;

    if (strchr(str, '=') == NULL)
    {
        token_free(tok);
        return 0;
    }

    struct ast_node *ast_var = ast_new(NODE_VAR);

    size_t len = strlen(str);
    size_t i = 0;

    for (; i < len && str[i] != '='; i++)
        continue;

    if (i == 0)
    {
        free_node(ast_var);
        token_free(tok);
        return -1;
    }

    ast_var->data.ast_var.name = my_strndup(str, i++);
    size_t double_quote = 1;
    if (str[i] == '\"' || str[i] == '\'')
    {
        i++;
        double_quote = -1;
    }
    ast_var->data.ast_var.value = my_strndup(str + i, strlen(str) - i + double_quote);


    parser->vector = vector_append(parser->vector, ast_var);

    token_free(lexer_pop(lexer));
    token_free(tok);

    return 1;
}

/**
 * simplemcd: (prefix)+ | (prefix)* (element)+
 */
struct parse_ast *parse_simple_cmd(struct lexer *lexer)
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
    }

    int status_elt = PARSER_UNEXPECTED_TOKEN;
    struct token *tok = lexer_peek(lexer);

    if (status_prefix == PARSER_UNEXPECTED_TOKEN)
    {
        if (is_keyword(tok))
        {
            token_free(tok);
            parser->status = PARSER_UNEXPECTED_TOKEN;
            free_node(cmd);
            return parser;
        }
    }

    while (tok->type != TOKEN_SEMICOLON)
    {
        token_free(tok);
        tok = NULL;
        parser = parse_element(lexer);
        if (status_elt == PARSER_UNEXPECTED_TOKEN
            && parser->status == PARSER_OK)
            status_elt = PARSER_OK;

        if (parser->status != PARSER_OK)
            break;

        s = xrealloc(s, (i + 1) * sizeof(char *));
        s[i - 1] = strdup(lexer->current_tok->data);
        s[i++] = NULL;
        tok = lexer_peek(lexer);
    }
    if (tok)
        token_free(tok);

    if (status_prefix != PARSER_OK && status_elt != PARSER_OK)
    {
        free_node(cmd);
        free(s);
        parser->status = PARSER_UNEXPECTED_TOKEN;
        return parser;
    }

    if (status_elt == PARSER_OK)
    {
        cmd->data.ast_cmd.argv = s;
        parser->vector = vector_append(parser->vector, cmd);
    }
    else
        free_node(cmd);

    parser->status = PARSER_OK;
    return parser;
}
