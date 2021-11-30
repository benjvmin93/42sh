static enum parser_status parse_ruleif(struct ast **res, struct lexer *lexer)
{
    UNUSED(res);
    struct token *tok = lexer_peek(lexer);
    if (tok->type != TOKEN_IF && tok->type != TOKEN_ELIF)
    {
        token_free(tok);
        return PARSER_UNEXPECTED_TOKEN;
    }

    // create a ast if and add root;
    struct ast_node *ast_if = ast_new(NODE_IF);

    token_free(tok);
    token_free(lexer_pop(lexer));

    enum parser_status status =
        parse_compoundlist(ast_if->data.ast_if.cond, lexer);
    if (status != PARSER_OK)
        return status;
    else
    {
        tok = lexer_peek(lexer);
        if (tok->type != TOKEN_THEN)
        {
            free_node(ast_if);
            token_free(tok);
            return PARSER_UNEXPECTED_TOKEN;
        }
        token_free(lexer_pop(lexer));
        token_free(tok);
        status = parse_compoundlist(ast_if->ast_if.then, lexer);
        if (status != PARSER_OK)
        {
            free_node(ast_if);
            return status;
        }
    }

    tok = lexer_peek(lexer);
    if (tok->type == TOKEN_ELSE || tok->type == TOKEN_ELIF)
        status = parse_else_clause(ast_if->ast_if.body, lexer);
    else
        ast_if->ast_if->body = NULL;

    token_free(tok);
    tok = lexer_peek(lexer);
    if (tok->type != TOKEN_FI)
    {
        free_node(ast_if);
        token_free(tok);
        return PARSER_UNEXPECTED_TOKEN;
    }

    res = vector_append(res, ast_if);

    token_free(tok);
    token_free(lexer_pop(lexer));
    return PARSER_OK;
}

static enum parser_status parse_else_clause(struct ast_node **res,
                                            struct lexer *lexer)
{
    UNUSED(res);
    enum parser_status status;

    struct token *tok = lexer_peek(lexer);
    if (tok->type == TOKEN_ELSE)
    {
        token_free(tok);
        token_free(lexer_pop(lexer));

        status = parse_compoundlist(res, lexer);

        return status;
    }

    if (tok->type == TOKEN_ELIF)
    {
        struct ast_node *ast_elif = ast_new(NODE_IF);
        token_free(tok);
        token_free(lexer_pop(lexer));

        status = parse_compoundlist(ast_elif->, lexer);
        if (status != PARSER_OK)
            return PARSER_UNEXPECTED_TOKEN;

        tok = lexer_peek(lexer);
        if (tok->type != TOKEN_THEN)
        {
            token_free(tok);
            return PARSER_UNEXPECTED_TOKEN;
        }

        token_free(tok);
        token_free(lexer_pop(lexer));
        status = parse_compoundlist(res, lexer);
        if (status != PARSER_OK)
            return PARSER_UNEXPECTED_TOKEN;

        tok = lexer_peek(lexer);
        if (tok->type == TOKEN_ELSE || tok->type == TOKEN_ELIF)
        {
            token_free(tok);
            status = parse_else_clause(res, lexer);
            return status;
        }
        token_free(tok);

        return PARSER_OK;
    }

    token_free(tok);
    return PARSER_UNEXPECTED_TOKEN;
}
