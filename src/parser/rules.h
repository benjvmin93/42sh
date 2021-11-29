#ifndef RULES_H
#define RULES_H

#include "../ast/ast.h"
#include "parser.h"

struct rule_model
{
    enum ast_type type;
    char **follow;
};

int follow_list(struct token *token);
int follow_andor(struct token *token);
int follow_pipeline(struct token *token);
int follow_cmd(struct token *token);
int follow_shellcmd_fundec(struct token *token);
int follow_redirection(struct token *token);
int follow_prefix(struct token *token);
int follow_elt(struct token *token);
int follow_compoundlist(struct token *token);
int follow_rule(struct token *token);
int follow_elseclause(struct token *token);

#endif /* RULES_H */
