#include "../commands/exec_commands.h"
#include "exec.h"

struct func
{
    char *name;
    int (*fun)(char **argv);
};

struct func funcs[] = { { "echo", &echo },
                        {"true", &exec_true_false },
                        {"true", &exec_true_false } };

int exec_node_command(struct ast_node *ast)
{
    char **data = ast->data.ast_cmd.argv;
    if (!data)
        return 0;

    for (unsigned i = 0; i < sizeof(funcs) / sizeof(*funcs); i++)
    {
        if (!strcmp(data[0], funcs[i].name))
            return funcs[i].fun(data);
    }

    return exec_command(data);
/*
    int b = strcmp(data[0], "echo");
    switch (b)
    {
    case 0:
        return echo(data);
    default:
        return exec_command(data);
    }
    */
}
