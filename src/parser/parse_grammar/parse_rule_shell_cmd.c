#include "../parser.h"

extern struct parse_ast *parser;

void free_annexe(struct lexer *lexer, struct token *tok)
{
    if (parser->status == PARSER_OK)
    {
        token_free(tok);
        token_free(lexer_pop(lexer));
    } 
}

struct parse_ast *parse_for(struct lexer *lexer, struct token *tok)
{
    parser = parse_rule_for(lexer);
    free_annexe(lexer, tok);
    return parser;
}

struct parse_ast *parse_while(struct lexer *lexer, struct token *tok)
{
    parser = parse_rule_while(lexer);
    free_annexe(lexer, tok);
    return parser;

}

struct parse_ast *parse_until(struct lexer *lexer, struct token *tok)
{
    parser = parse_rule_until(lexer);
    free_annexe(lexer, tok);
    return parser;
}

struct parse_ast *parse_case(struct lexer *lexer, struct token *tok)
{
    parser = parse_rule_case(lexer);
    free_annexe(lexer, tok);
    return parser;
}

struct parse_ast *parse_if(struct lexer *lexer, struct token *tok)
{
    parser = parse_rule_if(lexer);
    if (parser->status == PARSER_OK)
        token_free(tok);
    return parser;
}

/**
 * Sous fonction de shellcmd pour les rules
 */
struct parse_ast *parser_rules_shell_cmd(struct lexer *lexer)
{
    struct token *tok = lexer_peek(lexer);
    if (tok->type == TOKEN_FOR)
    {
        /*parser = parse_rule_for(lexer);
        if (parser->status == PARSER_OK)
        {
            token_free(tok);
            token_free(lexer_pop(lexer));
            return parser;
        }*/
        parser = parse_for(lexer, tok);
        if (parser->status == PARSER_OK)
            return parser;
    }

    if (tok->type == TOKEN_WHILE)
    {
        /*parser = parse_rule_while(lexer);
        if (parser->status == PARSER_OK)
        {
            token_free(tok);
            token_free(lexer_pop(lexer));
            return parser;
        }*/
        parser = parse_while(lexer, tok);
        if (parser->status == PARSER_OK)
            return parser; 
    }

    if (tok->type == TOKEN_UNTIL)
    {
        /*parser = parse_rule_until(lexer);
        if (parser->status == PARSER_OK)
        {
            token_free(tok);
            token_free(lexer_pop(lexer));
            return parser;
        }*/
        parser = parse_until(lexer, tok);
        if (parser->status == PARSER_OK)
            return parser; 
    }

    if (tok->type == TOKEN_CASE)
    {
        /*parser = parse_rule_case(lexer);
        if (parser->status == PARSER_OK)
        {
            token_free(tok);
            token_free(lexer_pop(lexer));
            return parser;
        }*/
        parser = parse_case(lexer, tok);
        if (parser->status == PARSER_OK)
            return parser;
    }

    if (tok->type == TOKEN_IF)
    {
        /*parser = parse_rule_if(lexer);
        if (parser->status == PARSER_OK)
        {
            token_free(tok);
            return parser;
        }*/
        parser = parse_if(lexer, tok);
        if (parser->status == PARSER_OK)
            return parser;
    }

    token_free(tok);
    parser->status = PARSER_UNEXPECTED_TOKEN;
    return parser;
}
