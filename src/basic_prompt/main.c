#define _POSIX_C_SOURCE 200809L

#define DEFAULT_BUFF_SIZE 500

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../execution/exec.h"
#include "../lexer/lexer.h"
#include "../parser/parser.h"
#include "../utils/alloc.h"

int main(int argc, char **argv)
{
    struct lexer *lexer = NULL;
    struct ast_node *ast = NULL;
    enum parser_status status;
    ast_vector = vector_init(1, sizeof(struct ast_node));

    if (argc == 1)
    {
        char *buf = NULL;
        size_t n = 0;
        if (getline(&buf, &n, stdin) == -1)
            errx(2, "%s", *argv);

        lexer = lexer_new(buf);
        status = parse(&ast, lexer);
    }
    else
    {
        if (!*(argv + 2))
            errx(2, "Usage: ./42sh [OPTIONS] [SCRIPT] [ARGUMENTS ...]");
        char *input = strdup(argv[2]);
        lexer = lexer_new(input);
        status = parse(&ast, lexer);
    }

    exec_all(ast);

    // printf("%d\n", status);
    lexer_free(lexer);
    free_node(ast);
    return status;
}
