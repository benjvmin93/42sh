#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../utils/alloc.h"
#include "../utils/clist.h"
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
    int tmp_escape = 1;

    if (*argv == '-')
    {
        while (argv[i])
        {
            if (argv[i] == 'n')
                tmp_newline = 1;
            else if (argv[i] == 'e')
                tmp_escape = 0;
            else if (argv[i] == 'E')
                tmp_escape = 1;
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

int is_escape_char(char c)
{
    switch (c)
    {
    case '\a':
        return 'a';
    case '\b':
        return 'b';
    case '\t':
        return 't';
    case '\n':
        return 'n';
    case '\v':
        return 'v';
    case '\f':
        return 'f';
    case '\r':
        return 'r';
    case '\\':
        return '\\';
    default:
        return 0;
    }
}

void processStr(char *src, struct clist *dest)
{
    size_t i = 0;
    int cur = 0;
    int clen;
    char code[5];

    for (; i < strlen(src); i++)
    {
        if (cur == 0)
        {
            if (src[i] != '\\')
                dest = app_char(dest, src[i]);
            else
                cur = 1;
        }
        else if (cur == 1)
        {
            switch (src[i])
            {
            case 'b':
                dest = app_char(dest, '\b');
                break;
            case 't':
                dest = app_char(dest, '\t');
                break;
            case 'v':
                dest = app_char(dest, '\v');
                break;
            case 'a':
                dest = app_char(dest, '\a');
                break;
            case 'n':
                dest = app_char(dest, '\n');
                break;
            case '\\':
                dest = app_char(dest, '\\');
                break;
            case '0':
                cur = 2;
                clen = 0;
                code[clen++] = '0';
                break;
            default:
                // dst[j++] = '\\';
                dest = app_char(dest, src[i]);
            }

            if (cur == 1)
                cur = 0;
        }
        else if (cur == 2)
        {
            if (src[i] >= '0' && src[i] <= '9')
                code[clen++] = src[i];
            else
            {
                code[clen] = 0;
                dest = app_char(dest, (char)atoi(code));
                if (src[i] == '\\')
                    cur = 1;
                else
                {
                    dest = app_char(dest, src[i]);
                    cur = 0;
                }
            }
        }
    }

    if (cur == 2)
    {
        code[clen] = 0;
        dest = app_char(dest, (char)atoi(code));
    }
}

void process_escape(char *src, struct clist *dest)
{
    for (size_t i = 0; src[i]; i++)
    {
        char escape_char = is_escape_char(src[i]);
        if (escape_char != '\\')
        {
            if (escape_char)
                dest = app_char(dest, escape_char);
            else
                dest = app_char(dest, src[i]);
        }
    }
}

int echo(char **argv)
{
    struct clist *res = init_clist();
    int no_newline = 0;
    int escape = 1;
    int opt_res = 0;
    size_t i = 1;

    while (is_opt(argv[i])
           && (opt_res = parse_opt(argv[i], &no_newline, &escape)))
        i++;

    for (; argv[i]; i++)
    {
        /*
        if (escape)
            process_escape(argv[i], res);
        else
            processStr(argv[i], res);
*/
        if (escape)
            res = app_str(res, argv[i]);
        else
            processStr(argv[i], res);

        printf("%s", res->data);
        if (argv[i + 1])
            printf(" ");
        res = new_clist(res);
    }
    if (!no_newline)
        putchar('\n');

    free_clist(res);

    return 0;
}
