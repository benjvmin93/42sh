#include "../parser.h"

extern struct parse_ast *parser;

/**
 * pipeline: ['!'] command ('|' ('\n')* command)*
 */

struct parse_ast *parse_pipeline(struct lexer *lexer)
{
    struct ast_node *ast_not = NULL;
    struct token *tok = lexer_peek(lexer);
    if (tok->type == TOKEN_NOT)
    {
        ast_not = ast_new(NODE_NOT);
        token_free(lexer_pop(lexer));
    }

    token_free(tok);

    parser = parse_cmd(lexer);
    if (ast_not != NULL && parser->status == PARSER_OK)
    {
        ast_not->data.ast_not.ast_cmd =
            parser->vector->data[parser->vector->size - 1];
        parser->vector =
            vector_remove(parser->vector, parser->vector->size - 1);
        parser->vector = vector_append(parser->vector, ast_not);
    }

    if (parser->status != PARSER_OK)
        return parser;

    tok = lexer_peek(lexer);
    if (tok->type != TOKEN_PIPE)
    {
        token_free(tok);
        parser->status = PARSER_OK;
        return parser;
    }
    // token_free(tok);
    token_free(lexer_pop(lexer));

    struct ast_node *pipeline = ast_new(NODE_PIPELINE);

    pipeline->data.ast_pipeline.argv =
        vector_append(pipeline->data.ast_pipeline.argv,
                      parser->vector->data[parser->vector->size - 1]);
    parser->vector = vector_remove(parser->vector, parser->vector->size - 1);

    while (true)
    {
        if (tok->type != TOKEN_PIPE)
        {
            token_free(tok);
            parser->status = PARSER_OK;
            parser->vector = vector_append(parser->vector, pipeline);
            return parser;
        }
        token_free(tok);
        token_free(lexer_pop(lexer));
        while (true)
        {
            struct token *tok2 = lexer_peek(lexer);
            if (tok2->type != TOKEN_LINE_BREAK)
            {
                token_free(tok2);
                break;
            }
            token_free(tok2);
            token_free(lexer_pop(lexer));
        }
        parser = parse_cmd(lexer);
        if (parser->status != PARSER_OK)
        {
            token_free(tok);
            return parser;
        }

        pipeline->data.ast_pipeline.argv =
            vector_append(pipeline->data.ast_pipeline.argv,
                          parser->vector->data[parser->vector->size - 1]);
        parser->vector =
            vector_remove(parser->vector, parser->vector->size - 1);
        tok = lexer_peek(lexer);
    }
    token_free(tok);

    parser->vector = vector_append(parser->vector, pipeline);
    return parser;
}
