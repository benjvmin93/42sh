#ifndef PARSER_H
#define PARSER_H

#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../ast/ast.h"
#include "../lexer/lexer.h"
#include "../utils/alloc.h"
#include "../utils/vector/vector.h"

#define UNUSED(x) (void)(x)

enum parser_status
{
    PARSER_OK, // 0
    PARSER_UNEXPECTED_TOKEN // 1
};

struct var
{
    char *name;
    char *value;
};

struct variables
{
    struct vector *variables;
    int is_assignment;
};

struct parse_ast
{
    enum parser_status status;
    struct vector *vector;
};

extern struct parse_ast *parser;

/*
struct parse_ast *parser_init(void);
void parser_free(struct parse_ast *parser);
*/
struct parse_ast *parse(struct lexer *lexer);
void linebreak_while(struct lexer *lexer);
struct parse_ast *parse_else_clause(struct lexer *lexer);
struct parse_ast *parse_rule_if(struct lexer *lexer);
struct parse_ast *parse_rule_case(struct lexer *lexer);
struct parse_ast *parse_rule_until(struct lexer *lexer);
struct parse_ast *parse_rule_while(struct lexer *lexer);
struct parse_ast *parse_rule_for(struct lexer *lexer);
struct parse_ast *parse_compound_list(struct lexer *lexer);
struct parse_ast *parse_element(struct lexer *lexer);
struct parse_ast *parse_prefix(struct lexer *lexer);
struct parse_ast *parse_redirection(struct lexer *lexer);
struct parse_ast *parse_func_dec(struct lexer *lexer);
struct parse_ast *parse_shell_cmd(struct lexer *lexer);
struct parse_ast *parser_rules_shell_cmd(struct lexer *lexer);
struct parse_ast *parse_simple_cmd(struct lexer *lexer);
struct parse_ast *parse_cmd(struct lexer *lexer);
struct parse_ast *parse_pipeline(struct lexer *lexer);
struct parse_ast *parse_and_or(struct lexer *lexer);
struct parse_ast *parse_list(struct lexer *lexer);
struct parse_ast *parse_do_group(struct lexer *lexer);

struct parse_ast *send_error(struct lexer *lexer, char *str);
void handle_parse_error(struct lexer *lexer);

// FOLLOW

int follow_elt(struct token *token);

int is_assignment_word(struct lexer *lexer);
int is_keyword(struct token *token);

#endif
