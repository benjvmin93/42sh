#include "../../parser.h"
#include "redirections.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <err.h>

int my_redir(char **argv)
{
    if (! argv[1])
        return -1;
    int out = open(argv[1], O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (out == -1)
    {
        fprintf(stderr, "Failed to open '%s'\n", argv[2]);
        return 2;
    }

    int out_dup;
    int status;
    pid_t pid;

    if ((pid = fork()) < 0)
        return 1;

    if (pid == 0)
    {
        if ((out_dup = dup2(out, 1)) != 1)
        {
            fprintf(stderr, "Failed to duplicate file descriptor\n");
            return 1;
        }
        close(out);
        
        if (argv[2])
        {
            if (execvp(argv[2], argv + 2) == -1)
            {
                fprintf(stderr, "Execvp failed\n");
                return 127;
            }
        }
    }
    else
    {
        while (wait(&status) != pid)
            ;
    }
    close(out_dup);
    fprintf(stdout, "%s exited with %d!\n", argv[2], status);
    
    return status;
}

int redir_chevron_right(struct ast_node *ast)
{
    char *arg_left = ast->data.ast_redir.left;
    char *arg_right = ast->data.ast_redir.right;
    char *argv[3] = { NULL };
    argv[0] = arg_right;

    if (arg_left)
    {
        argv[1] = arg_left;
    }
    
    return my_redir(argv);
}