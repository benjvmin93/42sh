static enum parser_status parse_dogroup(struct ast **res;, struct lexer *lexer)
{
    UNUSED(res);
    struct token *tok = lexer_peek(lexer);
    // TODO add TOKEN_DO at token type and lex it
    if (tok != TOKEN_DO)
    {
        token_free(tok);
        return PARSER_UNEXPECTED_TOKEN;
    }
    token_free(lexer_pop(tok));

    enum parser_status status = parse_compoundlist(res, lexer);
    if (status != PARSE_OK)
        return PARSER_UNEXPECTED_TOKEN;

    // TODO add TOKEN_DONE at token type and lex it
    if (tok != TOKEN_DONE)
    {
        token_free(tok);
        return PARSER_UNEXPECTED_TOKEN;
    }
    token_free(lexer_pop(tok));

    return PARSER_OK;
}

static enum parser_status parse_rulefor(struct ast **res, struct lexer *lexer)
{
    UNUSED(res);
    UNUSED(lexer);
    // TODO rule_for
    return PARSER_UNEXPECTED_TOKEN;
}

static enum parser_status parse_rulewhile(struct ast **res, struct lexer *lexer)
{
    UNUSED(res);
    struct token *tok = lexer_peek(lexer);
    if (tok->type != TOKEN_WHILE)
    {
        token_free(tok);
        return PARSER_UNEXPECTED_TOKEN;
    }
    token_free(lexer_pop(lexer));

    enum parser_status status = parse_compound_list(res, lexer);
    if (status != PARSER_OK)
        return PARSER_UNEXPECTED_TOKEN;

    // TODO do_group
    status = do_group(res, lexer);
    if (status != PARSER_OK)
        return PARSER_UNEXPECTED_TOKEN;

    return PARSER_OK;
}

static enum parser_status parse_ruleuntil(struct ast **res, struct lexer *lexer)
{
    UNUSED(res);
    struct token *tok = lexer_peek(lexer);
    if (tok->type != TOKEN_UNTIL)
    {
        token_free(tok);
        return PARSER_UNEXPECTED_TOKEN;
    }
    token_free(lexer_pop(lexer));

    enum parser_status status = parse_compound_list(res, lexer);
    if (status != PARSER_OK)
        return PARSER_UNEXPECTED_TOKEN;

    // TODO do_group
    status = do_group(res, lexer);
    if (status != PARSER_OK)
        return PARSER_UNEXPECTED_TOKEN;

    return PARSER_OK;
}

static enum parser_status parse_rulecase(struct ast **res, struct lexer *lexer)
{
    UNUSED(res);
    UNUSED(lexer);
    // TODO rule_case
    return PARSER_UNEXPECTED_TOKEN;
}

static enum parser_status parse_caseclause(struct ast **res,
                                           struct lexer *lexer)
{
    UNUSED(res);
    if (parse_caseitem(res, lexer) != PARSE_OK)
        return PARSER_UNEXPECTED_TOKEN;

    int count_semicolon = 0;

    while (true)
    {
        int count_semicolon = 0;
        struct token *tok = lexer_peek(lexer);
        if (tok->type != TOKEN_DOUBLE_SEMICOLON)
        {
            token_free(tok);
            break;
        }
        count_semicolon++;

        token_free(lexer_pop(lexer));
        linebreak_while(lexer);
        if (parse_caseitem(res, lexer) != PARSE_OK)
            break;
    }

    struct token *tok = lexer_peek(lexer);
    if (tok->type != TOKEN_DOUBLE_SEMICOLON)
        token_free(tok);
    linebreak_while(lexer);
    count_semicolon++;

    if (count_semicolon > 1)
        return PARSER_UNEXPECTED_TOKEN;

    return PARSER_OK;
}

struct enum parse_caseitem(struct ast **res, struct lexer *lexer)
{
    struct token *tok = lexer_peek(lexer);
    if (tok->type == TOKEN_LEFT_PARENTHESIS) token_free(lexer_pop(lexer));

    tok = lexer_peek(lexer); if (tok->type != TOKEN_WORD){
        token_free(tok); return PARSER_UNEXPECTED_TOKEN; }

token_free(lexer_pop(lexer));

while (true)
{
    tok = lexer_peek(lexer);
    if (tok->type != TOKEN_PIPE)
        break;
    token_free(lexer_pop(lexer));
    tok = lexer_peek(lexer);
    if (tok->type != TOKEN_WORD)
    {
        token_free(tok);
        return PARSER_UNEXPECTED_TOKEN;
    }
    token_free(lexer_pop(lexer));
}

token_free(lexer_pop(lexer));

if (token->type != TOKEN_RIGHT_PARENTHESIS)
{
    token_free(tok);
    return PARSER_UNEXPECTED_TOKEN;
}

linebreak_while(lexer);
if (parse_compoundlist(res, lexer) == PARSER_OK)
    return PARSER_OK;
return PARSER_OK;
}
