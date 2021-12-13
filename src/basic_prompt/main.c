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
#include "../utils/vector/vector.h"
#include "../variables/var.h"

struct variables *variables = NULL;

int annexe_file_interactive_mode(struct lexer *lexer, char *buf,
                                 int index_error)
{
    int code_res = 0;
    variables->is_assignment = 0;
    lexer = lexer_new(buf);
    parser = parse(lexer, index_error);
    if (parser->status == PARSER_OK)
    {
        if (parser->vector->size)
        {
            code_res = exec_all(parser->vector);
            struct var *var = variables->variables->data[0];
            var->value = xrealloc(var->value, 64);
            sprintf(var->value, "%d", code_res);
        }
    }
    else if (parser->status != PARSER_OK)
        code_res = 2;
    lexer_free(lexer);
    vector_destroy(parser->vector);
    index_error++;
    return code_res;
}

int interactive_mode(void)
{
    struct lexer *lexer = NULL;
    int index_error = 1;
    int interactive_mode = 0;
    int code_res = 0;

    interactive_mode = isatty(fileno(stdin));

    char *buf = NULL;
    size_t n = 0;
    if (interactive_mode)
        fprintf(stdout, "42sh$ ");
    while (getline(&buf, &n, stdin) != -1)
    {
        code_res = annexe_file_interactive_mode(lexer, buf, index_error);
        n = 0;
        if (interactive_mode)
            fprintf(stdout, "42sh$ ");
        free(parser);
    }
    if (variables)
        vector_destroy_variables(variables->variables);

    free(buf);

    return code_res;
}

char *string_file(FILE *file)
{
    char buf = fgetc(file);
    char *c = NULL;
    size_t i = 0;
    while (buf != EOF)
    {
        c = realloc(c, sizeof(char) * (i + 1));
        if (c == NULL)
            fprintf(stderr, "fail to malloc");
        c[i] = buf;
        i++;
        buf = fgetc(file);
    }
    c = realloc(c, sizeof(char) * (i + 1));
    c[i] = '\0';

    return c;
}

int file_mode(char **argv)
{
    struct lexer *lexer = NULL;
    int index_error = 1;
    int code_res = 0;

    FILE *fp = fopen(argv[1], "r");
    char *buf = string_file(fp);
    code_res = annexe_file_interactive_mode(lexer, buf, index_error);

    if (parser->status != PARSER_OK)
        code_res = 2;
    free(parser);
    if (variables)
        vector_destroy_variables(variables->variables);
    return code_res;
}

int string_mode(int argc, char **argv)
{
    struct lexer *lexer = NULL;
    int index_error = 1;
    int code_res = 0;

    if (argv[1] && *argv[1] == '-')
    {
        if (argv[1][1] == 'c')
        {
            if (!*(argv + 2))
                errx(2, "Usage: ./42sh [OPTIONS] [SCRIPT] [ARGUMENTS ...]");
            char *input = strdup(argv[2]);
            variables->is_assignment = 0;
            lexer = lexer_new(input);
            parser = parse(lexer, index_error);
            if (parser->status == PARSER_OK)
            {
                if (parser->vector->size)
                {
                    code_res = exec_all(parser->vector);
                    struct var *var = variables->variables->data[0];
                    sprintf(var->value, "%d", code_res);
                }
            }
            else
                code_res = 2;
            lexer_free(lexer);
            vector_destroy(parser->vector);
            free(parser);
            if (variables)
                vector_destroy_variables(variables->variables);
        }
        else
            errx(2, "Usage: ./42sh [OPTIONS] [SCRIPT] [ARGUMENTS ...]");
    }
    else
        errx(2, "Usage: ./42sh [OPTIONS] [SCRIPT] [ARGUMENTS ...]");

    argc++;
    return code_res;
}

int main(int argc, char **argv)
{
    int res = 0;
    variables = init_variables(argc, argv);
    /**
     * INTERACTIVE MODE
     */
    if (argc == 1)
        res = interactive_mode();
    /**
     * FILE MODE
     */
    else if (argc == 2)
        res = file_mode(argv);
    /**
     * STRING MODE
     */
    else
        res = string_mode(argc, argv);

    free(variables);
    return res;
}
