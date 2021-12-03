#include "../parser.h"

extern struct parse_ast *parser;

/**
 * rule_for : 
 For WORD ([';']|[('\n')* 'in' (WORD)* (';'|'\n')]) ('\n')* do_group
 */

struct parse_ast *parse_rulefor(struct lexer *lexer)
{
    struct token *tok = lexer_peek(lexer);
    if (tok->type != TOKEN_FOR)
    {
        token_free(tok);
        parser->status = PARSER_UNEXPECTED_TOKEN;
        return parser;
    }

    token_free(tok);
    token_free(lexer_pop(lexer));

    struct ast_node *node_for = ast_new(NODE_FOR);

    tok = lexer_peek(lexer);
    if (tok->type != TOKEN_WORD)
    {
        token_free(tok);
        parser->status = PARSER_UNEXPECTED_TOKEN;
        return parser;
    }

    struct ast_node *name = ast_new(NODE_COMMAND);
    char **s = NULL;
    size_t i = 1;
    while (tok->data[i - 1])
    {
        s = xrealloc(s, sizeof(char *) * (i + 1));
        s[i - 1] = strdup(tok->data);
        s[i++] = NULL;
    }
    name->data.ast_cmd.argv = s;
    node_for->data.ast_for.name = name;

    token_free(tok);
    token_free(lexer_pop(lexer));

    tok = lexer_peek(lexer);
    if (tok->type == TOKEN_SEMICOLON)
    {
        token_free(tok);
        token_free(lexer_pop(lexer));
    }
    else if (tok->type == TOKEN_LINE_BREAK || !strcmp(tok->data, "in"))
    {
        if (tok->type == TOKEN_LINE_BREAK)
        {
            token_free(tok);
            linebreak_while(lexer);
        }

        if (strcmp(tok->data, "in"))
        {
            token_free(tok);
            parser->status = PARSER_UNEXPECTED_TOKEN;
            free_node(node_for);
            return parser;
        }

        token_free(tok);

        tok = lexer_peek(lexer);
        if (tok->type == TOKEN_WORD)
        {
            struct ast_node *cond = ast_new(NODE_COMMAND);
            char **c = NULL;
            size_t j = 1;
            while (tok->type == TOKEN_WORD)
            {
                while (tok->data[j - 1])
                {
                    c = xrealloc(c, sizeof(char *) * (j + 1));
                    c[j +- 1] = strdup(tok->data);
                    c[j++] = NULL;
                }
                token_free(tok);
                token_free(lexer_pop(lexer));
                tok = lexer_peek(lexer);
            }
            cond->data.ast_cmd.argv = c;
            node_for->data.ast_for.cond = cond;
        }

        if (tok->type != TOKEN_SEMICOLON && tok->type != TOKEN_LINE_BREAK)
        {
            token_free(tok);
            free_node(node_for);
            parser->status = PARSER_UNEXPECTED_TOKEN;
            return parser;
        }
    }

    tok = lexer_peek(lexer);
    if (tok->type == TOKEN_LINE_BREAK)
    {
        token_free(tok);
        linebreak_while(lexer);
    }

    parser = parse_dogroup(lexer);
    if (parser->status != PARSER_OK)
    {
        free_node(node_for);
        return parser;
    }

    parser->status = PARSER_OK;
    return parser;
}
