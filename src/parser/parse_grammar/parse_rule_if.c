#include "../parser.h"

extern struct parse_ast *parser;

/**
 * rule_if: If compound_list Then compound_list [else_clause] Fi
 */

struct parse_ast *parse_ruleif(struct lexer *lexer)
{
    struct token *tok = lexer_peek(lexer);
    if (tok->type != TOKEN_IF && tok->type != TOKEN_ELIF)
    {
        token_free(tok);
        parser->status = PARSER_UNEXPECTED_TOKEN;
        return parser;
    }

    token_free(tok);
    token_free(lexer_pop(lexer));

    struct ast_node *node_if = ast_new(NODE_IF);
    parser = parse_compoundlist(lexer);

    if (parser->status != PARSER_OK)
        return parser;
    else
    {
        node_if->data.ast_if.cond = parser->vector->data[parser->vector->size - 1];
        parser->vector = vector_remove(parser->vector, parser->vector->size - 1);

        tok = lexer_peek(lexer);
        if (tok->type != TOKEN_THEN)
        {
            free_node(node_if);
            token_free(tok);
            parser->status = PARSER_UNEXPECTED_TOKEN;
            return parser;
        }
        token_free(lexer_pop(lexer));
        token_free(tok);
        parser = parse_compoundlist(lexer);
        if (parser->status != PARSER_OK)
        {
            free_node(node_if);
            return parser;
        }

        node_if->data.ast_if.then = parser->vector->data[parser->vector->size - 1];
        parser->vector = vector_remove(parser->vector, parser->vector->size - 1);
    }

    tok = lexer_peek(lexer);
    if (tok->type == TOKEN_ELSE || tok->type == TOKEN_ELIF)
    {
        parser = parse_else_clause(lexer);
        node_if->data.ast_if.body = parser->vector->data[parser->vector->size - 1];
        parser->vector = vector_remove(parser->vector, parser->vector->size - 1);
    }
    token_free(tok);
    tok = lexer_peek(lexer);
    if (tok->type != TOKEN_FI)
    {
        free_node(node_if);
        token_free(tok);
        parser->status = PARSER_UNEXPECTED_TOKEN;
        return parser;
    }

    parser->vector = vector_append(parser->vector, node_if);

    token_free(tok);
    token_free(lexer_pop(lexer));
    parser->status = PARSER_OK;
    return parser;
}
