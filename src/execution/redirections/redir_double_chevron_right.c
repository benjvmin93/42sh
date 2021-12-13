#define _POSIX_C_SOURCE 200809L

#include "redirections.h"

#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

int redir_double_chevron_right(struct ast_node *ast)
{
    struct ast_redir node = ast->data.ast_redir;
    FILE *out = fopen(node.right, "a");
    
    if (!node.left)
        return 0;

    char **cmd = node.left->data.ast_cmd.argv;
    int io_number;
    
    if (node.io_number != -1)
        io_number = node.io_number;
    else
        io_number = 1;

    

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
/*
#include <stdlib.h>

int main(void)
{   
    struct ast_node cmd;
    cmd.data.ast_cmd.argv = xmalloc(3 * sizeof(char *));
    cmd.data.ast_cmd.argv[0] = "echo";
    cmd.data.ast_cmd.argv[1] = "caca";
    cmd.data.ast_cmd.argv[2] = NULL;
    struct ast_node ast;
    ast.data.ast_redir.io_number = -1;
    ast.data.ast_redir.right = "foo";
    ast.data.ast_redir.left = &cmd;

    int res = redir_double_chevron_right(&ast);

    free(cmd.data.ast_cmd.argv);
    return res;
}*/