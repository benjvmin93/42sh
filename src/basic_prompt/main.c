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
    int code_res = 0;
    if (argc == 1)
    {
        char *buf = NULL;
        size_t n = 0;
        while (getline(&buf, &n, stdin) != -1)
        {
            lexer = lexer_new(buf);
            parser = parse(lexer);
            if (parser->status == PARSER_OK)
                exec_all(parser->vector);
            else
                code_res = 1;
            n = 0;
            buf = NULL;
            lexer_free(lexer);
            vector_destroy(parser->vector);
            free(parser);
        }
        

        free(buf);
    }
    else
    {
        if (!*(argv + 2))
            errx(2, "Usage: ./42sh [OPTIONS] [SCRIPT] [ARGUMENTS ...]");
        char *input = strdup(argv[2]);
        lexer = lexer_new(input);
        parser = parse(lexer);
        if (parser->status == PARSER_OK)
            exec_all(parser->vector);
        else
            code_res = 1;
        lexer_free(lexer);
        vector_destroy(parser->vector);
    }
    return code_res;
}
