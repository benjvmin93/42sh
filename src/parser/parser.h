#ifndef PARSER_H
#define PARSER_H

#include "../ast/ast.h"
#include "../lexer/lexer.h"

enum parser_status
{
    PARSER_OK, // 0
    PARSER_UNEXPECTED_TOKEN // 1
};

enum parser_status parse(struct ast **res, struct lexer *lexer);

#endif
