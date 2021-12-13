#include "../commands/exec_commands.h"
#include "exec.h"

struct func
{
    char *name;
    int (*fun)(char **argv);
};

struct func funcs[] = { { "echo", &echo },
                        { "true", &exec_true_false },
                        { "false", &exec_true_false } };

void free_list(char **argv)
{
    char **list = argv;

    if (list)
    {
        while (*list)
        {
            char *tmp = *list;
            free(tmp);
            list++;
        }
    }

    free(argv);
}

int exec_node_command(struct ast_node *ast)
{
    char **data = ast->data.ast_cmd.argv;

    char **c = NULL;
    size_t j = 1;
    while (data[j - 1] != NULL)
    {
        c = xrealloc(c, sizeof(char *) * (j + 1));
        c[j - 1] = strdup(data[j - 1]);
        c[j++] = NULL;
    }

    if (!c)
        return 0;

    c = expands_args(c);
    int res = 0;

    for (unsigned i = 0; i < sizeof(funcs) / sizeof(*funcs); i++)
    {
        if (!strcmp(data[0], funcs[i].name))
        {
            res = funcs[i].fun(c);
            free_list(c);
            return res;
        }
    }

    res = exec_command(c);
    free_list(c);
    return res;
}
