#ifndef PARSER_H
#define PARSER_H


#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../ast/ast.h"
#include "../utils/alloc.h"
#include "../lexer/lexer.h"

#define UNUSED(x) (void)(x)

enum parser_status
{
    PARSER_OK, // 0
    PARSER_UNEXPECTED_TOKEN // 1
};

enum parser_status parse(struct ast_node **res, struct lexer *lexer);
void linebreak_while(struct lexer *lexer);
enum parser_status parse_else_clause(struct ast_node **res,
                                            struct lexer *lexer);
enum parser_status parse_ruleif(struct ast_node **res, struct lexer *lexer);
enum parser_status parse_rulecase(struct ast_node **res,
                                         struct lexer *lexer);
enum parser_status parse_ruleuntil(struct ast_node **res,
                                          struct lexer *lexer);
enum parser_status parse_rulewhile(struct ast_node **res,
                                          struct lexer *lexer);
enum parser_status parse_rulefor(struct ast_node **res,
                                        struct lexer *lexer);
enum parser_status parse_compoundlist(struct ast_node **res,
                                             struct lexer *lexer);
enum parser_status parse_element(struct ast_node **res,
                                        struct lexer *lexer);
enum parser_status parse_prefix(struct ast_node **res,
                                       struct lexer *lexer);
enum parser_status parse_redirection(struct ast_node **res,
                                            struct lexer *lexer);
enum parser_status parse_funcdec(struct ast_node **res,
                                        struct lexer *lexer);
enum parser_status parse_shellcmd(struct ast_node **res,
                                         struct lexer *lexer);
enum parser_status parser_rules_shellcmd(struct ast_node **res,
                                                struct lexer *lexer);
enum parser_status parse_simplecmd(struct ast_node **res,
                                          struct lexer *lexer);
enum parser_status parse_cmd(struct ast_node **res, struct lexer *lexer);
enum parser_status parse_pipeline(struct ast_node **res,
                                         struct lexer *lexer);
enum parser_status parse_and_or(struct ast_node **res,
                                       struct lexer *lexer);
enum parser_status parse_list(struct ast_node **res, struct lexer *lexer);

enum parser_status handle_parse_error(enum parser_status status,
                                            struct ast_node **res);

int is_assignment_word(struct lexer *lexer);

#endif
