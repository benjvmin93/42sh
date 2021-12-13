#define _POSIX_C_SOURCE 200809L

#include "redirections.h"

#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

#define SIZE 1000

int redir_chevron_left(struct ast_node *ast)
{
    struct ast_redir node = ast->data.ast_redir;

    if (!node.left)
        return 0;

    char **cmd = node.left->data.ast_cmd.argv;
    int io_number;
    
    if (node.io_number != -1)
        io_number = node.io_number;
    else
        io_number = 0;

    int in = open(node.right, O_RDONLY);
    if (in < 0)
    {
        warnx("%s: No such file or directory", node.right);
        return 1;
    }

    int in_dup = 0;
    int status = 0;
    pid_t pid;

    if ((pid = fork()) < 0)
    {
        warn("Failed to create child process");
        exit(errno);
    }
    if (pid == 0)
    {
        if ((in_dup = dup2(in, io_number)) == -1)
        {
            warnx("Failed to duplicate file descriptor");
            exit(errno);
        }
        if (close(in) < 0)
        {
            warn("Failed to close file descriptor");
            exit(errno);
        }

        execvp(*cmd, cmd);
        warn("%s:", *cmd);
        exit(errno);
    }

    waitpid(pid, &status, 0);

    close(in_dup);

    return status;
}
/*
#include <stdlib.h>

int main(void)
{   
    struct ast_node cmd;
    cmd.data.ast_cmd.argv = xmalloc(2 * sizeof(char *));
    cmd.data.ast_cmd.argv[0] = "cat";
    cmd.data.ast_cmd.argv[1] = NULL;
    struct ast_node ast;
    ast.data.ast_redir.io_number = 0;
    ast.data.ast_redir.right = "foo";
    ast.data.ast_redir.left = &cmd;

    int res = redir_chevron_left(&ast);

    free(cmd.data.ast_cmd.argv);
    return res;
}
*/