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
struct parse_ast *parse_ruleif(struct lexer *lexer);
struct parse_ast *parse_rulecase(struct lexer *lexer);
struct parse_ast *parse_ruleuntil(struct lexer *lexer);
struct parse_ast *parse_rulewhile(struct lexer *lexer);
struct parse_ast *parse_rulefor(struct lexer *lexer);
struct parse_ast *parse_compoundlist(struct lexer *lexer);
struct parse_ast *parse_element(struct lexer *lexer);
struct parse_ast *parse_prefix(struct lexer *lexer);
struct parse_ast *parse_redirection(struct lexer *lexer);
struct parse_ast *parse_funcdec(struct lexer *lexer);
struct parse_ast *parse_shellcmd(struct lexer *lexer);
struct parse_ast *parser_rules_shellcmd(struct lexer *lexer);
struct parse_ast *parse_simplecmd(struct lexer *lexer);
struct parse_ast *parse_cmd(struct lexer *lexer);
struct parse_ast *parse_pipeline(struct lexer *lexer);
struct parse_ast *parse_and_or(struct lexer *lexer);
struct parse_ast *parse_list(struct lexer *lexer);

void handle_parse_error(void);

int is_assignment_word(struct lexer *lexer);

#endif
