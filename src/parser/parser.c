#include "parser.h"

#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../ast/ast.h"

#define UNUSED(x) (void)(x)

static enum parser_status parse_list(struct ast_node **res,
                                     struct lexer *lexer);
static enum parser_status parse_and_or(struct ast_node **res,
                                       struct lexer *lexer);
static enum parser_status parse_pipeline(struct ast_node **res,
                                         struct lexer *lexer);
static enum parser_status parse_cmd(struct ast_node **res, struct lexer *lexer);
static enum parser_status parse_simplecmd(struct ast_node **res,
                                          struct lexer *lexer);
static enum parser_status parse_shellcmd(struct ast_node **res,
                                         struct lexer *lexer);
static enum parser_status parse_funcdec(struct ast_node **res,
                                        struct lexer *lexer);
static enum parser_status parse_redirection(struct ast_node **res,
                                            struct lexer *lexer);
static enum parser_status parse_prefix(struct ast_node **res,
                                       struct lexer *lexer);
static enum parser_status parse_element(struct ast_node **res,
                                        struct lexer *lexer);
static enum parser_status parse_compoundlist(struct ast_node **res,
                                             struct lexer *lexer);
static enum parser_status parse_rulefor(struct ast_node **res,
                                        struct lexer *lexer);
static enum parser_status parse_rulewhile(struct ast_node **res,
                                          struct lexer *lexer);
static enum parser_status parse_ruleuntil(struct ast_node **res,
                                          struct lexer *lexer);
static enum parser_status parse_rulecase(struct ast_node **res,
                                         struct lexer *lexer);
static enum parser_status parse_ruleif(struct ast_node **res,
                                       struct lexer *lexer);
static enum parser_status parse_else_clause(struct ast_node **res,
                                            struct lexer *lexer);
// static enum parser_status parse_do_group(struct ast_node **res, struct lexer
// *lexer); static enum parser_status parse_case_clause(struct ast_node **res,
// struct lexer *lexer); static enum parser_status parse_case_item(struct
// ast_node **res, struct lexer *lexer);
void linebreak_while(struct lexer *lexer);

static enum parser_status handle_parse_error(enum parser_status status,
                                             struct ast_node **res)
{
    warnx("unexpected token");
    // ast_free(*res);
    *res = NULL;
    return status;
}

void linebreak_while(struct lexer *lexer)
{
    // struct token *tok = lexer_peek(lexer);
    struct token *tok = NULL;
    while (true)
    {
        tok = lexer_peek(lexer);
        if (tok->type != TOKEN_LINE_BREAK)
        {
            token_free(tok);
            break;
        }
        token_free(tok);
        token_free(lexer_pop(lexer));
    }
}

/**
 * input:  EOF | '\n' | list EOF | list '\n'
 */
enum parser_status parse(struct ast_node **res, struct lexer *lexer)
{
    UNUSED(res);
    // If we're at the end of file, there's no input
    struct token *tok = lexer->current_tok;
    if (tok->type == TOKEN_EOF)
    {
        *res = NULL;
        return PARSER_OK;
    }

    if (tok->type == TOKEN_LINE_BREAK)
        return PARSER_OK;
    // try to parse an expression. if an error occured, free the
    // produced ast and return the same error code
    enum parser_status status = parse_list(res, lexer);
    if (status != PARSER_OK)
    {
        printf("%s\n", lexer->input + lexer->pos);
        return handle_parse_error(status, res);
    }
    // once parsing the expression is done, we should have
    // reached the end of file.
    tok = lexer_peek(lexer);
    if (tok->type == TOKEN_EOF)
    {
        token_free(tok);
        token_free(lexer_pop(lexer));
        return PARSER_OK;
    }
    if (tok->type == TOKEN_LINE_BREAK)
    {
        token_free(tok);
        token_free(lexer_pop(lexer));
        return PARSER_OK;
    }
    // if we didn't reach the end of file, it's an error
    token_free(tok);
    printf("%s\n", lexer->input + lexer->pos);
    return handle_parse_error(PARSER_UNEXPECTED_TOKEN, res);
}

/**
 * list: and_or  ((';' | '&')  and_or)*  [';' | '&']
 */

static enum parser_status parse_list(struct ast_node **res, struct lexer *lexer)
{
    UNUSED(res);
    enum parser_status status = parse_and_or(res, lexer);
    if (status != PARSER_OK)
        return status;

    while (true)
    {
        // have a look at the type of the next token. Stop if not interested
        struct token *tok = lexer_peek(lexer);
        if (tok->type != TOKEN_SEMICOLON && tok->type != TOKEN_AND)
        {
            token_free(tok);
            return PARSER_OK;
        }
        token_free(tok);
        token_free(lexer_pop(lexer));

        // parse the stuff at the right
        if ((status = parse_and_or(res, lexer)) != PARSER_OK)
            break;
    }

    return PARSER_OK;
}

/**
 * and_or: and_or  (('&&' | '||') ('\n')* pipeline)*
 */

static enum parser_status parse_and_or(struct ast_node **res,
                                       struct lexer *lexer)
{
    UNUSED(res);
    enum parser_status status = parse_pipeline(res, lexer);
    if (status != PARSER_OK)
        return status;

    while (true)
    {
        struct token *tok = lexer_peek(lexer);
        if (tok->type != TOKEN_DOUBLE_AND && tok->type != TOKEN_DOUBLE_PIPE)
        {
            token_free(tok);
            return PARSER_OK;
        }

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
        if ((status = parse_pipeline(res, lexer)) != PARSER_OK)
            return status;
    }
}

/**
 * pipeline: ['!'] command ('|' ('\n')* command)*
 */

static enum parser_status parse_pipeline(struct ast_node **res,
                                         struct lexer *lexer)
{
    UNUSED(res);
    // TODO add not !
    enum parser_status status = parse_cmd(res, lexer);
    if (status != PARSER_OK)
        return status;

    while (true)
    {
        struct token *tok = lexer_peek(lexer);
        if (tok->type != TOKEN_PIPE)
        {
            token_free(tok);
            return PARSER_OK;
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
        if ((status = parse_cmd(res, lexer)) != PARSER_OK)
            return status;
    }
}

/**
 * command: simplecmd | shellcmd (redirection)* | funcdec (redirection)*
 */

static enum parser_status parse_cmd(struct ast_node **res, struct lexer *lexer)
{
    UNUSED(res);
    enum parser_status status = parse_simplecmd(res, lexer);
    if (status == PARSER_OK)
        return status;

    status = parse_shellcmd(res, lexer);
    if (status == PARSER_OK)
    {
        while (true)
        {
            status = parse_redirection(res, lexer);
            if (status != PARSER_OK)
                break;
            token_free(lexer_pop(lexer));
        }
        return PARSER_OK;
    }

    status = parse_funcdec(res, lexer);
    if (status == PARSER_OK)
    {
        while (true)
        {
            status = parse_redirection(res, lexer);
            if (status != PARSER_OK)
                break;
            token_free(lexer_pop(lexer));
        }
        return PARSER_OK;
    }

    return PARSER_UNEXPECTED_TOKEN;
}

/**
 * simplemcd: (prefix)+ | (prefix)* (element)+
 */
static enum parser_status parse_simplecmd(struct ast_node **res,
                                          struct lexer *lexer)
{
    enum parser_status status_prefix = PARSER_UNEXPECTED_TOKEN;
    enum parser_status status;
    
    struct ast_node *cmd = ast_new(NODE_COMMAND);
    struct vector *vector_cmd = cmd->data.ast_cmd.cmd;
    char *s = NULL;
    size_t i = 0;

    while (1)
    {
        status = parse_prefix(res, lexer);
        if (status_prefix == PARSER_UNEXPECTED_TOKEN && status == PARSER_OK)
            status_prefix = PARSER_OK;

        if (status != PARSER_OK)
            break;

        s = xrealloc(s, strlen(s) + strlen(lexer->current_tok->data) + 1);
        s = strcat(s, lexer->current_tok->data);
        s[strlen(s)] = 0;
    }

    if (!s)
        fprintf(stderr, "Error when parsing prefix of command\n");
    
    enum parser_status status_elt = PARSER_UNEXPECTED_TOKEN;
    while (true)
    {
        status = parse_element(res, lexer);
        if (status_elt == PARSER_UNEXPECTED_TOKEN && status == PARSER_OK)
            status_elt = PARSER_OK;

        if (status != PARSER_OK)
            break;

        s = xrealloc(s, strlen(s) + strlen(lexer->current_tok->data) + 1);
        s = strcat(s, lexer->current_tok->data);
        s[strlen(s)] = 0;
    }

    vector_cmd = vector_append(vector_cmd, s);

    if (status_prefix != PARSER_OK && status_elt != PARSER_OK)
    {
        vector_destroy(vector_cmd);
        free_node(cmd);
        return PARSER_UNEXPECTED_TOKEN;
    }


    return PARSER_OK;
}

/**
 * Sous foction de shellcmd pour les rules
 */

static enum parser_status parser_rules_shellcmd(struct ast_node **res,
                                                struct lexer *lexer)
{
    UNUSED(res);
    struct token *tok = lexer_peek(lexer);
    if (tok->type == TOKEN_FOR)
    {
        enum parser_status status = parse_rulefor(res, lexer);
        if (status == PARSER_OK)
        {
            token_free(tok);
            token_free(lexer_pop(lexer));
            return status;
        }
    }

    if (tok->type == TOKEN_WHILE)
    {
        enum parser_status status = parse_rulewhile(res, lexer);
        if (status == PARSER_OK)
        {
            token_free(tok);
            token_free(lexer_pop(lexer));
            return status;
        }
    }

    if (tok->type == TOKEN_UNTIL)
    {
        enum parser_status status = parse_ruleuntil(res, lexer);
        if (status == PARSER_OK)
        {
            token_free(tok);
            token_free(lexer_pop(lexer));
            return status;
        }
    }

    if (tok->type == TOKEN_CASE)
    {
        enum parser_status status = parse_rulecase(res, lexer);
        if (status == PARSER_OK)
        {
            token_free(tok);
            token_free(lexer_pop(lexer));
            return status;
        }
    }

    if (tok->type == TOKEN_IF)
    {
        enum parser_status status = parse_ruleif(res, lexer);
        if (status == PARSER_OK)
        {
            token_free(tok);
            token_free(lexer_pop(lexer));
            return status;
        }
    }

    token_free(tok);
    return PARSER_UNEXPECTED_TOKEN;
}

/**
 * shellcmd: '{' compound_list '}'
 *      | '(' compound_list ')'
 *      | rule_for
 *      | rule_while
 *      | rule_until
 *      | rule_case
 *      | rule_is
 */

static enum parser_status parse_shellcmd(struct ast_node **res,
                                         struct lexer *lexer)
{
    UNUSED(res);
    struct token *tok = lexer_peek(lexer);
    if (tok->type == TOKEN_LEFT_ACC)
    {
        token_free(lexer_pop(lexer));
        enum parser_status status = parse_compoundlist(res, lexer);
        if (status != PARSER_OK)
            return status;

        tok = lexer_peek(lexer);
        if (tok->type == TOKEN_RIGHT_ACC)
        {
            token_free(lexer_pop(lexer));
            return status;
        }
        else
        {
            token_free(tok);
            return PARSER_UNEXPECTED_TOKEN;
        }
    }

    if (tok->type == TOKEN_LEFT_PARENTHESIS)
    {
        token_free(lexer_pop(lexer));
        enum parser_status status = parse_compoundlist(res, lexer);
        if (status != PARSER_OK)
            return status;

        tok = lexer_peek(lexer);
        if (tok->type == TOKEN_RIGHT_PARENTHESIS)
        {
            token_free(lexer_pop(lexer));
            return status;
        }
        else
        {
            token_free(tok);
            return PARSER_UNEXPECTED_TOKEN;
        }
    }
    else
        token_free(tok);

    if (parser_rules_shellcmd(res, lexer) == PARSER_OK)
        return PARSER_OK;

    return PARSER_UNEXPECTED_TOKEN;
}

/**
 * funcdec: WORD '(' ')' ('\n')* shellcmd
 */

static enum parser_status parse_funcdec(struct ast_node **res,
                                        struct lexer *lexer)
{
    UNUSED(res);
    struct token *tok = lexer_peek(lexer);

    if (tok->type != TOKEN_WORD)
    {
        token_free(tok);
        return PARSER_UNEXPECTED_TOKEN;
    }
    token_free(tok);
    token_free(lexer_pop(lexer));

    tok = lexer_peek(lexer);
    if (tok->type != TOKEN_LEFT_PARENTHESIS)
    {
        token_free(tok);
        return PARSER_UNEXPECTED_TOKEN;
    }

    token_free(lexer_pop(lexer));

    tok = lexer_peek(lexer);
    if (tok->type != TOKEN_RIGHT_PARENTHESIS)
    {
        token_free(tok);
        return PARSER_UNEXPECTED_TOKEN;
    }

    token_free(lexer_pop(lexer));

    tok = lexer_peek(lexer);
    if (tok->type == TOKEN_LINE_BREAK)
        linebreak_while(lexer);

    token_free(tok);
    enum parser_status status = parse_shellcmd(res, lexer);
    if (status != PARSER_OK)
        return PARSER_UNEXPECTED_TOKEN;

    return PARSER_OK;
}

/**
 * redirection: [IONUMBER] '>' WORD
        | [IONUMBER] '<' WORD
        | [IONUMBER] '>>' WORD
        | [IONUMBER] '<<' HEREDOC
        | [IONUMBER] '<<-' HEREDOC
        | [IONUMBER] '>&' WORD
        | [IONUMBER] '<&' WORD
        | [IONUMBER] '>|' WORD
        | [IONUMBER] '<>' WORD
 */

static enum parser_status parse_redirection(struct ast_node **res,
                                            struct lexer *lexer)
{
    UNUSED(res);
    UNUSED(lexer);
    // TODO redirection
    return PARSER_UNEXPECTED_TOKEN;
}

int is_assignment_word(struct lexer *lexer)
{
    struct token *tok = lexer_peek(lexer);
    if (!strcmp(tok->data, "echo"))
    {
        token_free(tok);
        return 1;
    }
    token_free(tok);

    return 0;
}

/**
 * prefix: ASSIGNMENT_WORD | redirection
 */

static enum parser_status parse_prefix(struct ast_node **res,
                                       struct lexer *lexer)
{
    UNUSED(res);
    if (is_assignment_word(lexer))
    {
        token_free(lexer_pop(lexer));
        return PARSER_OK;
    }

    enum parser_status status = parse_redirection(res, lexer);
    if (status == PARSER_OK)
        return status;

    return PARSER_UNEXPECTED_TOKEN;
}

/**
 * element: WORD | redirection
 */

static enum parser_status parse_element(struct ast_node **res,
                                        struct lexer *lexer)
{
    UNUSED(res);
    struct token *tok = lexer_peek(lexer);
    if (tok->type >= TOKEN_WORD)
    {
        token_free(tok);
        token_free(lexer_pop(lexer));
        return PARSER_OK;
    }
    /*
    if (follow_elt(tok))    //TODO check les follow de element pour s'arreter au
    moment ou l'on rencontre le bon token {                       // EXEMPLE: if
    echo foo; then echo OK; elif echo bar then KO; else return 1 => WRONG SYNTAX
    BC NO SEMICOLON BETWEEN ELIF AND THEN. token_free(tok); return PARSER_OK;
    }*/
    token_free(tok);
    enum parser_status status = parse_redirection(res, lexer);
    if (status == PARSER_OK)
        return status;

    return PARSER_UNEXPECTED_TOKEN;
}

/**
 * compound_list: ('\n')* and_or ((';'|'&'|'\n') ('\n')* and_or)*
 * [('&'|';'|'\n') ('\n')*]
 */

static enum parser_status parse_compoundlist(struct ast_node **res,
                                             struct lexer *lexer)
{
    UNUSED(res);
    struct token *tok = lexer_peek(lexer);
    if (tok->type == TOKEN_LINE_BREAK)
    {
        linebreak_while(lexer);
    }

    token_free(tok);
    enum parser_status status = parse_and_or(res, lexer);
    if (status != PARSER_OK)
        return status;

    // token_free(lexer_pop(lexer));
    while (true)
    {
        tok = lexer_peek(lexer);
        if (tok->type != TOKEN_SEMICOLON && tok->type != TOKEN_LINE_BREAK
            && tok->type != TOKEN_AND)
            break;

        if (tok->type == TOKEN_LINE_BREAK)
        {
            token_free(tok);
            linebreak_while(lexer);
        }
        token_free(tok);
        enum parser_status status = parse_and_or(res, lexer);
        if (status != PARSER_OK)
        {
            token_free(lexer_pop(lexer));
            return PARSER_OK;
        }
    }

    token_free(tok);
    return PARSER_OK;
}

static enum parser_status parse_rulefor(struct ast_node **res,
                                        struct lexer *lexer)
{
    UNUSED(res);
    UNUSED(lexer);
    // TODO rule_for
    return PARSER_UNEXPECTED_TOKEN;
}

static enum parser_status parse_rulewhile(struct ast_node **res,
                                          struct lexer *lexer)
{
    UNUSED(res);
    UNUSED(lexer);
    // TODO rule_while
    return PARSER_UNEXPECTED_TOKEN;
}

static enum parser_status parse_ruleuntil(struct ast_node **res,
                                          struct lexer *lexer)
{
    UNUSED(res);
    UNUSED(lexer);
    // TODO rule_until
    return PARSER_UNEXPECTED_TOKEN;
}

static enum parser_status parse_rulecase(struct ast_node **res,
                                         struct lexer *lexer)
{
    UNUSED(res);
    UNUSED(lexer);
    // TODO rule_case
    return PARSER_UNEXPECTED_TOKEN;
}

/**
 * rule_if: If compound_list Then compound_list [else_clause] Fi
 */

static enum parser_status parse_ruleif(struct ast_node **res,
                                       struct lexer *lexer)
{
    UNUSED(res);
    struct token *tok = lexer_peek(lexer);
    if (tok->type != TOKEN_IF && tok->type != TOKEN_ELIF)
    {
        token_free(tok);
        return PARSER_UNEXPECTED_TOKEN;
    }

    token_free(tok);
    token_free(lexer_pop(lexer));

    enum parser_status status = parse_compoundlist(res, lexer);
    if (status != PARSER_OK)
        return status;
    else
    {
        // token_free(lexer_pop(lexer));
        tok = lexer_peek(lexer);
        if (tok->type != TOKEN_THEN)
        {
            token_free(tok);
            return PARSER_UNEXPECTED_TOKEN;
        }
        token_free(lexer_pop(lexer));
        token_free(tok);
        status = parse_compoundlist(res, lexer);
        if (status != PARSER_OK)
            return status;
    }
    // token_free(lexer_pop(lexer));

    tok = lexer_peek(lexer);
    if (tok->type == TOKEN_ELSE || tok->type == TOKEN_ELIF)
        status = parse_else_clause(res, lexer);

    token_free(tok);
    tok = lexer_peek(lexer);
    if (tok->type != TOKEN_FI)
    {
        token_free(tok);
        return PARSER_UNEXPECTED_TOKEN;
    }

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
        token_free(tok);
        token_free(lexer_pop(lexer));

        status = parse_compoundlist(res, lexer);
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

// TODO do_group

// TODO case_clause

// TODO case_time
/*
#include <stdio.h>

int main(void)
{
    char *input = "fi\n";
    struct lexer *lexer = lexer_new(input);

    struct ast *ast = NULL;

    enum parser_status status = parse(&ast, lexer);

    printf("%d\n", status);

    lexer_free(lexer);
}*/
