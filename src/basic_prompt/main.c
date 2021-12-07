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

struct variables *variables = NULL;

int main(int argc, char **argv)
{
    struct lexer *lexer = NULL;
    int code_res = 0;
    // INTERACTIVE MODE
    if (argc == 1)
    {
        variables = xmalloc(sizeof(struct variables));
        variables->variables = vector_init(1, sizeof(struct var));

        char *buf = NULL;
        size_t n = 0;
        //fprintf(stdout, "42sh$ ");
        while (getline(&buf, &n, stdin) != -1)
        {
            variables->is_assignment = 0;
            lexer = lexer_new(buf);
            parser = parse(lexer);
            if (parser->status == PARSER_OK)
            {
                if (parser->vector->size)
                    exec_all(parser->vector);
            }
            else if (parser->status == PARSER_UNEXPECTED_TOKEN)
                code_res = 1;
            n = 0;
            buf = NULL;
            lexer_free(lexer);
            vector_destroy(parser->vector);
            free(parser);
            //fprintf(stdout, "42sh$ ");
        }
        if (variables)
            vector_destroy_variables(variables->variables);
        
        free(buf);
    }
    else
    {
        // STRING MODE
        if (argv[1][0] == '-' && argv[1][1] == 'c')
        {   
            variables = xmalloc(sizeof(struct variables));
            variables->variables = vector_init(1, sizeof(struct var));
            if (!*(argv + 2))
                errx(2, "Usage: ./42sh [OPTIONS] [SCRIPT] [ARGUMENTS ...]");
            char *input = strdup(argv[2]);
            variables->is_assignment = 0;
            lexer = lexer_new(input);
            parser = parse(lexer);
            if (parser->status == PARSER_OK)
            {
                if (parser->vector->size)
                    exec_all(parser->vector);
            }
            else if (parser->status == PARSER_UNEXPECTED_TOKEN)
                code_res = 1;
            lexer_free(lexer);
            vector_destroy(parser->vector);
            if (variables)
                vector_destroy_variables(variables->variables);
            free(parser);
        }
        // SHELL FILE MODE
        else if (argv[1] && *argv[1] != '-')
        {
            FILE *f = fopen(argv[1], "r");
            if (!f)
                err(1, "%s", argv[2]);
            
            variables = xmalloc(sizeof(struct variables));
            variables->variables = vector_init(1, sizeof(struct var));
            char *buf = NULL;
            size_t n = 0;
            while (getline(&buf, &n, f) != -1)
            {
            variables->is_assignment = 0;
            lexer = lexer_new(buf);
            parser = parse(lexer);
            if (parser->status == PARSER_OK)
            {
                if (parser->vector->size)
                    exec_all(parser->vector);
            }
            else if (parser->status == PARSER_UNEXPECTED_TOKEN)
                code_res = 1;
            n = 0;
            buf = NULL;
            lexer_free(lexer);
            vector_destroy(parser->vector);
            free(parser);
        }

        if (variables)
            vector_destroy_variables(variables->variables);
        
        free(buf);
        }
    }

    free(variables);
    return code_res;
}
