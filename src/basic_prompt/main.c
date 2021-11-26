#define _POSIX_C_SOURCE 200809L

#define DEFAULT_BUFF_SIZE 500

#include "../lexer/lexer.h"
#include "../utils/alloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>

int main(int argc, char **argv)
{
    char **input = NULL;
    struct lexer *lexer = NULL;

    if (argc == 1)
    {
        char *buf = xmalloc(sizeof(char) * DEFAULT_BUFF_SIZE);
        size_t n = 0;
        if (getline(&buf, &n, stdin) == -1)
            errx(2, "%s", *argv);

        lexer = build_lexer(buf);
        free(buf);
    }
    else
    {
        input = argv + 2;
        lexer = build_lexer(*input);
    }

    print_tokens(lexer);
    lexer_free(lexer);
    return 0;

}
