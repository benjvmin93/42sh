#include "../parser.h"

extern struct parse_ast *parser;

/* rule_until: Until compound_list do_group */

struct parse_ast *parse_rule_until(struct lexer *lexer)
{
    struct token *tok = lexer_peek(lexer);
    if (tok->type != TOKEN_UNTIL)
    {
        token_free(tok);
        parser->status = PARSER_UNEXPECTED_TOKEN;
        return parser;
    }

    token_free(tok);
    token_free(lexer_pop(lexer));

    struct ast_node *node_while = ast_new(NODE_UNTIL);
    size_t old_vect_size = parser->vector->size;
    parser = parse_compound_list(lexer);

    if (parser->status != PARSER_OK)
    {
        free_node(node_while);
        return parser;
    }
    else
    {
        node_while->data.ast_while.cond = append_node(node_while->data.ast_while.cond, old_vect_size);

        old_vect_size = parser->vector->size;
        parser = parse_do_group(lexer);
        if (parser->status != PARSER_OK)
        {
            free_node(node_while);
            return parser;
        }

        node_while->data.ast_while.body = append_node(node_while->data.ast_while.body, old_vect_size);
    }

    parser->vector = vector_append(parser->vector, node_while);
    parser->status = PARSER_OK;
    return parser;
}
