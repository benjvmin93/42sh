#include "../commands/exec_commands.h"
#include "exec.h"

int exec_node_command(struct ast_node *ast)
{
    char **data = ast->data.ast_cmd.argv;
    if (!data)
        return 0;
    int b = strcmp(data[0], "echo");
    switch (b)
    {
    case 0:
        return echo(data);
    default:
        return exec_command(data);
    }
}
