#include "../parser.h"

/**
 * simplemcd: (prefix)+ | (prefix)* (element)+
 */
enum parser_status parse_simplecmd(struct ast_node **res,
                                          struct lexer *lexer)
{
    enum parser_status status_prefix = PARSER_UNEXPECTED_TOKEN;
    enum parser_status status;
    
    struct ast_node *cmd = ast_new(NODE_COMMAND);
    char **s = cmd->data.ast_cmd.argv;
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

    if (!*s)
        fprintf(stderr, "Error when parsing prefix of command\n");
    
    enum parser_status status_elt = PARSER_UNEXPECTED_TOKEN;
    while (true)
    {
        status = parse_element(res, lexer);
        if (status_elt == PARSER_UNEXPECTED_TOKEN && status == PARSER_OK)
            status_elt = PARSER_OK;

        if (status != PARSER_OK)
            break;


        s = xrealloc(s, (i + 1) * sizeof(char *));
        s[i - 1] = strdup(lexer->current_tok->data);
        s[i++] = NULL;
    }

    if (status_prefix != PARSER_OK && status_elt != PARSER_OK)
    {
        free_node(cmd);
        return PARSER_UNEXPECTED_TOKEN;
    }

    ast_vector = vector_append(ast_vector, cmd);
    vector_print(ast_vector);

    return PARSER_OK;
}
