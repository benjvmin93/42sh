#define _POSIX_C_SOURCE 2

#include "exec_commands.h"

#include <stdio.h>
#include <unistd.h>

static int count_tokens(char **argv)
{
    int count = 0;
    while (argv[count])
        count++;
    return count;
}

int echo(char **argv)
{
    int count_toks = count_tokens(argv);
    int resOPT = getopt(count_toks, argv, "eEn");
    int no_newline = (resOPT == 'n');
    // int escape = (resOPT == 'e');
    // int no_escape = 1;

    if (!no_newline && !no_newline && resOPT != 'E')
    {
        for (int i = 0; i < count_toks; i++)
        {
            if (i == count_toks - 1)
                fprintf(stdout, "%s\n", argv[i]);
            else
                fprintf(stdout, "%s ", argv[i]);
        }
        return 0;
    }

    if (no_newline)
    {
        for (int i = 1; i < count_toks; i++)
        {
            if (i == count_toks - 1)
                fprintf(stdout, "%s", argv[i]);
            else
                fprintf(stdout, "%s ", argv[i]);
        }
        return 0;
    }

    return 0;
}