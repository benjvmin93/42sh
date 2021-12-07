#include "../parser.h"

extern struct parse_ast *parser;

/**
 * rule_for : 
 For WORD ([';']|[('\n')* 'in' (WORD)* (';'|'\n')]) ('\n')* do_group
 */

void next_lexer(struct lexer *lexer, struct token *tok)
{
    token_free(tok);
    token_free(lexer_pop(lexer));
}

void linebreak(struct lexer *lexer, struct token *tok)
{
    if (tok->type == TOKEN_LINE_BREAK)
    {
        token_free(tok);
        linebreak_while(lexer);
    }
}

void vector_add(struct ast_node *node_for)
{
    node_for->data.ast_for.body = parser->vector->data[parser->vector->size - 1];
    parser->vector = vector_remove(parser->vector, parser->vector->size - 1);

    parser->vector = vector_append(parser->vector, node_for);
}

//Function to parse the word/variable of the condition so it can be change
struct ast_node *cond_name(struct lexer *lexer)
{
    struct token *tok = lexer_peek(lexer); //Take the first WORD
    if (tok->type != TOKEN_WORD)
    {
        token_free(tok);
        return NULL;
    }

    //Create an ast to parse the first WORD due to the grammar
    struct ast_node *name = ast_new(NODE_COMMAND);
    char **s = NULL;
    size_t i = 1;
    if (tok->data)
    {
        s = xrealloc(s, sizeof(char *) * (i + 1));
        s[i - 1] = strdup(tok->data);
        s[i++] = NULL;
    }
    name->data.ast_cmd.argv = s;
    token_free(tok);
    token_free(lexer_pop(lexer));

    return name;
}

//Parse all the sequence of condition
struct ast_node *cond_list(struct lexer *lexer, struct token *tok)
{
    struct ast_node *cond = ast_new(NODE_COMMAND);
    char **c = NULL;
    size_t j = 1;
    while (tok->type == TOKEN_WORD)
    {
        if (tok->data)
        {
            c = xrealloc(c, sizeof(char *) * (j + 1));
            c[j - 1] = strdup(tok->data);
            c[j++] = NULL;
        }
        /*
           token_free(tok);
           token_free(lexer_pop(lexer));*/
        next_lexer(lexer, tok);
        tok = lexer_peek(lexer);
    }
    cond->data.ast_cmd.argv = c;
    token_free(tok);

    return cond;
}

//Parse the conditon in two different ast 
struct parse_ast *parse_cond_for(struct lexer *lexer, struct ast_node *node_for)
{
    struct token *tok = lexer_peek(lexer); //Take the ';' or '\n' or "in"
    if (tok->type == TOKEN_SEMICOLON)
        next_lexer(lexer, tok);
    else if (tok->type == TOKEN_LINE_BREAK || !strcmp(tok->data, "in"))
    {
        linebreak(lexer, tok); //(\n)*

        if (strcmp(tok->data, "in"))
        {
            token_free(tok);
            free_node(node_for);
            return send_error(lexer, "in");
        }

        next_lexer(lexer, tok);

        tok = lexer_peek(lexer);
        if (tok->type == TOKEN_WORD)
            node_for->data.ast_for.cond = cond_list(lexer, tok);

        tok = lexer_peek(lexer);

        if (tok->type != TOKEN_SEMICOLON && tok->type != TOKEN_LINE_BREAK)
        {
            token_free(tok);
            free_node(node_for);
            return send_error(lexer, "; or \n");
        }
    }

    next_lexer(lexer, tok);
    parser->status = PARSER_OK;
    return parser;
}

struct parse_ast *parse_rule_for(struct lexer *lexer)
{
    struct token *tok = lexer_peek(lexer); //Take the FOR
    if (tok->type != TOKEN_FOR)
    {
        token_free(tok);
        return send_error(lexer, "for");
    }

    next_lexer(lexer, tok);

    //Create a new structure to parse the for function until they find done
    struct ast_node *node_for = ast_new(NODE_FOR);

    //Add into the structure the variable of the condition
    //and check if the grammar is respected
    node_for->data.ast_for.name = cond_name(lexer);
    if (node_for->data.ast_for.name == NULL)
        return send_error(lexer, "variable");

    //Parse the condition part
    parser = parse_cond_for(lexer, node_for);
    if (parser->status != PARSER_OK)
        return parser;

    tok = lexer_peek(lexer);
    linebreak(lexer, tok); //(\n)*

    //Parse the body part
    parser = parse_do_group(lexer);
    if (parser->status != PARSER_OK)
    {
        token_free(tok);
        free_node(node_for);
        return parser;
    }

    token_free(tok);
    vector_add(node_for);
    parser->status = PARSER_OK;
    return parser;
}
