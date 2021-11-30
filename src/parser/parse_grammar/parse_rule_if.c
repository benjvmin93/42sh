#include "../parser.h"

/**
 * rule_if: If compound_list Then compound_list [else_clause] Fi
 */

enum parser_status parse_ruleif(struct ast_node **res, struct lexer *lexer)
{
    UNUSED(res);
    struct token *tok = lexer_peek(lexer);
    if (tok->type != TOKEN_IF && tok->type != TOKEN_ELIF)
    {
        token_free(tok);
        return PARSER_UNEXPECTED_TOKEN;
    }

    token_free(tok);
    token_free(lexer_pop(lexer));

    struct ast_node *node_if = ast_new(NODE_IF);

    enum parser_status status =
        parse_compoundlist(&node_if->data.ast_if.cond, lexer);
    if (status != PARSER_OK)
        return status;
    else
    {
        tok = lexer_peek(lexer);
        if (tok->type != TOKEN_THEN)
        {
            free_node(node_if);
            token_free(tok);
            return PARSER_UNEXPECTED_TOKEN;
        }
        token_free(lexer_pop(lexer));
        token_free(tok);
        status = parse_compoundlist(&node_if->data.ast_if.then, lexer);
        if (status != PARSER_OK)
        {
            free_node(node_if);
            return status;
        }
    }

    tok = lexer_peek(lexer);
    if (tok->type == TOKEN_ELSE || tok->type == TOKEN_ELIF)
        status = parse_else_clause(&node_if->data.ast_if.body, lexer);

    token_free(tok);
    tok = lexer_peek(lexer);
    if (tok->type != TOKEN_FI)
    {
        free_node(node_if);
        token_free(tok);
        return PARSER_UNEXPECTED_TOKEN;
    }

    token_free(tok);
    token_free(lexer_pop(lexer));
    return PARSER_OK;
}
