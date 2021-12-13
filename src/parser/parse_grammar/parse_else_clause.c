#include "../parser.h"

extern struct parse_ast *parser;

struct parse_ast *parse_else(struct lexer *lexer, struct token *tok)
{
    struct ast_node *node_else = ast_new(NODE_ELSE);

    size_t old_vect_size = parser->vector->size;

    token_free(tok);
    token_free(lexer_pop(lexer));

    parser = parse_compound_list(lexer);

    if (parser->status != PARSER_OK)
    {
        free_node(node_else);
        return parser;
    }
    node_else->data.ast_else.body =
        append_node(node_else->data.ast_else.body, old_vect_size);

    parser->vector = vector_append(parser->vector, node_else);

    return parser;
}

struct parse_ast *parse_elif(struct lexer *lexer, struct token *tok,
                             struct ast_node *node_elif)
{
    tok = lexer_peek(lexer);
    if (tok->type != TOKEN_THEN)
    {
        token_free(tok);
        free_node(node_elif);
        parser->status = PARSER_EXPECTING_TOKEN;
        return send_error(lexer, "then");
    }

    token_free(tok);
    token_free(lexer_pop(lexer));
    return parser;
}

void parse_else_elif(struct lexer *lexer, struct token *tok,
                     struct ast_node *node_elif)
{
    if (tok->type == TOKEN_ELSE || tok->type == TOKEN_ELIF)
    {
        parser = parse_else_clause(lexer);
        if (parser->status != PARSER_OK)
            return;

        node_elif->data.ast_if.body =
            parser->vector->data[parser->vector->size - 1];
        parser->vector =
            vector_remove(parser->vector, parser->vector->size - 1);
    }
}

struct parse_ast *parse_else_clause(struct lexer *lexer)
{
    struct token *tok = lexer_peek(lexer);
    if (tok->type == TOKEN_ELSE)
        return parse_else(lexer, tok);

    if (tok->type == TOKEN_ELIF)
    {
        token_free(tok);
        token_free(lexer_pop(lexer));

        struct ast_node *node_elif = ast_new(NODE_IF);
        size_t old_vect_size = parser->vector->size;
        parser = parse_compound_list(lexer);
        if (parser->status != PARSER_OK)
        {
            free_node(node_elif);
            return parser;
        }

        node_elif->data.ast_if.cond =
            append_node(node_elif->data.ast_if.cond, old_vect_size);

        parser = parse_elif(lexer, tok, node_elif);

        if (parser->status != PARSER_OK)
            return parser;

        old_vect_size = parser->vector->size;
        parser = parse_compound_list(lexer);

        if (parser->status != PARSER_OK)
        {
            free_node(node_elif);
            return parser;
        }

        node_elif->data.ast_if.then =
            append_node(node_elif->data.ast_if.then, old_vect_size);

        tok = lexer_peek(lexer);
        if (tok->type == TOKEN_ELSE || tok->type == TOKEN_ELIF)
            parse_else_elif(lexer, tok, node_elif);
        token_free(tok);

        parser->vector = vector_append(parser->vector, node_elif);
        parser->status = PARSER_OK;
        return parser;
    }

    token_free(tok);
    if (parser->status != PARSER_EXPECTING_TOKEN)
        parser->status = PARSER_UNEXPECTED_TOKEN;

    return parser;
}
