#ifndef EXPANDS_H
#define EXPANDS_H

#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../ast/ast.h"
#include "../lexer/lexer.h"
#include "../parser/parser.h"
#include "../utils/alloc.h"
#include "../utils/vector/vector.h"

char **expands_args(char **list_word);

#endif
