#include "../parser.h"

extern struct parse_ast *parser;

struct vector *append_node(struct vector *dest, size_t old_size)
{
    size_t size = parser->vector->size;
    for (size_t i = old_size; i < size; i++)
    {
        struct ast_node *ast = parser->vector->data[old_size];
        dest = vector_append(dest, ast);
        parser->vector = vector_remove(parser->vector, old_size);
    }

    return dest;
}

struct parse_ast *rule_if_then(struct lexer *lexer, struct ast_node *node_if)
{
    struct token *tok = lexer_peek(lexer);
    if (tok->type != TOKEN_THEN)
    {
        free_node(node_if);
        token_free(tok);
        return send_error(lexer, "then");
    }
    token_free(lexer_pop(lexer));
    token_free(tok);
    size_t old_vect_size = parser->vector->size;

    parser = parse_compound_list(lexer);

    if (parser->status != PARSER_OK)
    {
        free_node(node_if);
        return parser;
    }

    node_if->data.ast_if.then =
        append_node(node_if->data.ast_if.then, old_vect_size);

    return parser;
}

struct parse_ast *rule_if_else_elif(struct lexer *lexer,
                                    struct ast_node *node_if)
{
    struct token *tok = lexer_peek(lexer);

    if (tok->type == TOKEN_ELSE || tok->type == TOKEN_ELIF)
    {
        parser = parse_else_clause(lexer);
        token_free(tok);
        if (parser->status != PARSER_OK)
        {
            free_node(node_if);
            return parser;
        }

        node_if->data.ast_if.body =
            parser->vector->data[parser->vector->size - 1];
        parser->vector =
            vector_remove(parser->vector, parser->vector->size - 1);
    }
    else
        token_free(tok);

    return parser;
}

// Parse 'fi' inside parse_ruleif function
struct parse_ast *rule_if_fi(struct lexer *lexer, struct ast_node *node_if)
{
    struct token *tok = lexer_peek(lexer);
    if (tok->type != TOKEN_FI)
    {
        free_node(node_if);
        token_free(tok);
        return send_error(lexer, "fi");
    }

    parser->vector = vector_append(parser->vector, node_if);

    token_free(tok);
    token_free(lexer_pop(lexer));
    parser->status = PARSER_OK;

    return parser;
}
/**
 * rule_if: If compound_list Then compound_list [else_clause] Fi
 */

struct parse_ast *parse_rule_if(struct lexer *lexer)
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

    size_t old_vect_size = parser->vector->size;

    parser = parse_compound_list(lexer);

    node_if->data.ast_if.cond =
        append_node(node_if->data.ast_if.cond, old_vect_size);

    if (parser->status != PARSER_OK)
    {
        free_node(node_if);
        return parser;
    }
    else
        parser = rule_if_then(lexer, node_if);

    if (parser->status == PARSER_OK)
        parser = rule_if_else_elif(lexer, node_if);

    if (parser->status == PARSER_OK)
        parser = rule_if_fi(lexer, node_if);

    return parser;
}
