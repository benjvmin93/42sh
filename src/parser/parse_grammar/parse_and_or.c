#include "../parser.h"

extern struct parse_ast *parser;

/**
 * and_or: pipeline  (('&&' | '||') ('\n')* pipeline)*
 */

struct parse_ast *parse_and_or(struct lexer *lexer)
{
    parser = parse_pipeline(lexer);
    if (parser->status != PARSER_OK)
        return parser;
    struct token *tok = lexer_peek(lexer);

    if (tok->type != TOKEN_DOUBLE_AND && tok->type != TOKEN_DOUBLE_PIPE)
    {
        token_free(tok);
        return parser;
    }

    struct ast_node *ast_andor = ast_new(NODE_AND_OR);

    size_t size = ast_andor->data.ast_andor.size;
    struct vector *list_ast_node = ast_andor->data.ast_andor.list_ast_node;
    list_ast_node = vector_append(
        list_ast_node, parser->vector->data[parser->vector->size - 1]);
    parser->vector = vector_remove(parser->vector, parser->vector->size - 1);

    int is_pipeline = 0;
    while (true)
    {
        enum and_or operator= 0;

        if (tok->type == TOKEN_DOUBLE_AND)
            operator= AND;
        else if (tok->type == TOKEN_DOUBLE_PIPE)
            operator= OR;
        else
        {
            token_free(tok);
            parser->status = PARSER_OK;
            parser->vector = vector_append(parser->vector, ast_andor);
            return parser;
        }

        ast_andor->data.ast_andor.list_operator[size++] = operator;
        ast_andor->data.ast_andor.list_operator = xrealloc(
            ast_andor->data.ast_andor.list_operator, sizeof(int) * (size + 1));
        ast_andor->data.ast_andor.size = size;

        token_free(tok);
        token_free(lexer_pop(lexer));
        linebreak_while(lexer);

        parser = parse_pipeline(lexer);
        if (parser->status != PARSER_OK)
            break;

        list_ast_node = vector_append(
            list_ast_node, parser->vector->data[parser->vector->size - 1]);
        parser->vector =
            vector_remove(parser->vector, parser->vector->size - 1);

        is_pipeline = 1;
        tok = lexer_peek(lexer);
    }

    if (is_pipeline == 0)
        parser->status = PARSER_UNEXPECTED_TOKEN;

    parser->vector = vector_append(parser->vector, ast_andor);

    return parser;
}
