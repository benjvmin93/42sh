#include "exec_commands.h"

#include <stdio.h>
#include <string.h>

int exec_true_false(char **argv)
{
    if (!strcmp(argv[0], "true"))
        return 0;
    else if (!strcmp(argv[0], "false"))
        return 1;
    else
        fprintf(stderr, "Unexpected token %s\n", argv[0]);

    return -1;
}
