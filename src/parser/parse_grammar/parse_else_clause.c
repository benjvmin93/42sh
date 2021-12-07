#include "../parser.h"

extern struct parse_ast *parser;

struct parse_ast *parse_else(struct lexer *lexer, struct token *tok)
{
    token_free(tok);
    token_free(lexer_pop(lexer));

    parser = parse_compound_list(lexer);

    return parser;
}

struct parse_ast *parse_elif(struct lexer *lexer, struct token *tok, struct ast_node *node_elif)
{
    node_elif->data.ast_if.cond = parser->vector->data[parser->vector->size - 1];
    parser->vector = vector_remove(parser->vector, parser->vector->size - 1);

    tok = lexer_peek(lexer);
    if (tok->type != TOKEN_THEN)
    {
        token_free(tok);
        free_node(node_elif);
        parser->status = PARSER_UNEXPECTED_TOKEN;
        return parser;
    }

    token_free(tok);
    token_free(lexer_pop(lexer));
    return parser;
}

void parse_else_elif(struct lexer *lexer, struct token *tok, struct ast_node *node_elif)
{
    if (tok->type == TOKEN_ELSE || tok->type == TOKEN_ELIF)
    {
        parser = parse_else_clause(lexer);
        node_elif->data.ast_if.body = parser->vector->data[parser->vector->size - 1];
        parser->vector = vector_remove(parser->vector, parser->vector->size - 1);
    }
}

struct parse_ast *parse_else_clause(struct lexer *lexer)
{
    struct token *tok = lexer_peek(lexer);
    if (tok->type == TOKEN_ELSE)
    {
        /*token_free(tok);
        token_free(lexer_pop(lexer));

        parser = parse_compound_list(lexer);

        return parser;*/
        return parse_else(lexer, tok);
    }

    if (tok->type == TOKEN_ELIF)
    {
        token_free(tok);
        token_free(lexer_pop(lexer));

        struct ast_node *node_elif = ast_new(NODE_IF);

        parser = parse_compound_list(lexer);
        if (parser->status != PARSER_OK)
            return parser;
        parser = parse_elif(lexer, tok, node_elif);
        if (parser->status != PARSER_OK)
            return parser;
/*
        node_elif->data.ast_if.cond = parser->vector->data[parser->vector->size - 1];
        parser->vector = vector_remove(parser->vector, parser->vector->size - 1);

        tok = lexer_peek(lexer);
        if (tok->type != TOKEN_THEN)
        {
            free_node(node_elif);
            token_free(tok);
            parser->status = PARSER_UNEXPECTED_TOKEN;
            return parser;
        }

        token_free(lexer_pop(lexer));
        token_free(tok);
*/
 
        parser = parse_compound_list(lexer);
        if (parser->status != PARSER_OK)
        {
            free_node(node_elif);
            return parser;
        }

        node_elif->data.ast_if.then = parser->vector->data[parser->vector->size - 1];
        parser->vector = vector_remove(parser->vector, parser->vector->size - 1);

        tok = lexer_peek(lexer);
        if (tok->type == TOKEN_ELSE || tok->type == TOKEN_ELIF)
        {
            parse_else_elif(lexer, tok, node_elif);
            /*
            parser = parse_else_clause(lexer);
            node_elif->data.ast_if.body = parser->vector->data[parser->vector->size - 1];
            parser->vector = vector_remove(parser->vector, parser->vector->size - 1);
            */
        }
        token_free(tok);
        
        parser->vector = vector_append(parser->vector, node_elif);
        parser->status = PARSER_OK;
        return parser;
    }

    token_free(tok);
    parser->status = PARSER_UNEXPECTED_TOKEN;

    return parser;
}
