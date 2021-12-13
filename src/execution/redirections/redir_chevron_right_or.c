#define _POSIX_C_SOURCE 200809L

#include "redirections.h"

#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

int redir_chevron_right_or(struct ast_node *ast)
{
    struct ast_redir node = ast->data.ast_redir;
    FILE *out = fopen(node.right, "w+");
    
    if (!node.left)
        return 0;

    char **cmd = node.left->data.ast_cmd.argv;
    int io_number;
    
    if (node.io_number != -1)
        io_number = node.io_number;
    else
        io_number = STDOUT_FILENO;

    int out_dup = 0;
    int status = 0;
    pid_t pid;

    if ((pid = fork()) < 0)
    {
        warn("Failed to create child process");
        exit(errno);
    }
    if (pid == 0)
    {
        if ((out_dup = dup2(fileno(out), io_number)) == -1)
        {
            warnx("Failed to duplicate file descriptor");
            exit(errno);
        }
        if (fclose(out) < 0)
        {
            warn("Failed to close file descriptor");
            exit(errno);
        }

        execvp(*cmd, cmd);
        warn("%s", *cmd);
        exit(errno);
    }

    waitpid(pid, &status, 0);

    fflush(stdout);
    close(out_dup);

    return status;
}
