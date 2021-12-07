#include "../parser.h"

extern struct parse_ast *parser;

/* rule_while: While compound_list do_group */

struct parse_ast *parse_rule_while(struct lexer *lexer)
{
    struct token *tok = lexer_peek(lexer);
    if (tok->type != TOKEN_WHILE)
    {
        token_free(tok);
        parser->status = PARSER_UNEXPECTED_TOKEN;
        return parser;
    }

    token_free(tok);
    token_free(lexer_pop(lexer));

    struct ast_node *node_while = ast_new(NODE_WHILE);
    parser = parse_compound_list(lexer);

    if (parser->status != PARSER_OK)
    {
        free_node(node_while);
        return parser;
    }
    else
    {
        node_while->data.ast_while.cond = parser->vector->data[parser->vector->size - 1];
        parser->vector = vector_remove(parser->vector, parser->vector->size - 1);

        parser = parse_do_group(lexer);
        if (parser->status != PARSER_OK)
        {
            free_node(node_while);
            return parser;
        }

        node_while->data.ast_while.body = parser->vector->data[parser->vector->size - 1];
        parser->vector = vector_remove(parser->vector, parser->vector->size - 1);
    }
    
    parser->vector = vector_append(parser->vector, node_while);
    parser->status = PARSER_OK;
    return parser;
}
