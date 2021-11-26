#ifndef RULES_H
#define RULES_H

#include "parser.h"
#include "../ast/ast.h"

struct rule_model
{
    enum ast_type type;
    char **follow;
};

#endif /* RULES_H */
