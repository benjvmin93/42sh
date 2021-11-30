#include <stdio.h>
#include <unistd.h>

#include "exec_commands.h"

// Check if the string at index argv + 1 is an option of type "-n" || "-e" ||
// "-E".
int is_opt(char *str)
{
    if (!str)
        return 0;
    if (*str == '-'
        && (*(str + 1) == 'n' || *(str + 1) == 'e' || *(str + 1) == 'E'))
        return 1;

    return 0;
}

// Parse options inside the given string.
// If we encounter an option of type "-n" || "-e" || "-E", we iterate trough the
// string in order to check if it contains a sequence of options. If the
// sequence is valide we change the value inside the option booleans *no_newline
// and *escape and the function returns 1. Otherwise the boolean are not
// modified and the function returns 0.
int parse_opt(char *argv, int *no_newline, int *escape)
{
    size_t i = 1;
    int tmp_newline = 0;
    int tmp_escape = 0;
    if (*argv == '-')
    {
        while (argv[i])
        {
            if (argv[i] == 'n')
                tmp_newline = 1;
            else if (argv[i] == 'e')
                tmp_escape = 1;
            else if (argv[i] == 'E')
                tmp_escape = 0;
            else
                return 0;
            i++;
        }
        *no_newline = tmp_newline;
        *escape = tmp_escape;
        return 1;
    }
    else
        return 0;
}
int echo(char **argv)
{
    // Initialization of booleans in order to know which options are asked by
    // the user.
    int no_newline = 0;
    int escape = 0;

    // Return value of parse_opt function.
    int opt_res = 0;

    // We know it's echo command, so we juste move to the next string to parse
    // its arguments.
    argv++;

    // Here, parse_opt return value indicates if the string is a valid option
    // sequence and change the values of the option booleans. If the string is
    // an option and a valid option sequence, we move to the next string and
    // repeat the process.
    while (is_opt(*argv) && (opt_res = parse_opt(*argv, &no_newline, &escape)))
        argv++;

    // echo -n [ARGUMENTS]
    if (no_newline)
    {
        for (int i = 0; argv[i]; i++)
        {
            if (argv[i + 1])
                fprintf(stdout, "%s ", argv[i]);
            else
                fprintf(stdout, "%s", argv[i]);
        }
        return 0;
    }

    // echo [ARGUMENTS]
    for (size_t i = 0; argv[i]; i++)
    {
        if (argv[i + 1])
            fprintf(stdout, "%s ", argv[i]);
        else
            fprintf(stdout, "%s\n", argv[i]);
    }

    return 0;
}
/*
int main(void)
{
    char *argv[] = { "echo", "-neEEEEnnn", "-eeEEEEnnnn" "foo", NULL};
    return echo(argv);
}*/
