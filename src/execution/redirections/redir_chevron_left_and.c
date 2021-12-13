#define _POSIX_C_SOURCE 200809L

#include "redirections.h"

#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

int redir_chevron_left_and(struct ast_node *ast)
{
    struct ast_redir node = ast->data.ast_redir;
    int io_number;
    
    if (node.io_number != -1)
        io_number = node.io_number;
    else
        io_number = STDIN_FILENO;
    
    if (*node.right == '-')
    {
        if (close(io_number) == -1)
            warn("%d", io_number);
        exit(2);     
    }

    int in = open(node.right, O_RDONLY);
    if (in < 0)
    {
        warnx("%s: No such file or directory", node.right);
        return 1;
    }

    int in_dup;
    if ((in_dup = dup2(io_number, in)) == -1)
    {
        warnx("Syntax error: Bad fd number");
        exit(2);
    }
    if (close(io_number) == -1)
    {
        warn("Failed to close file descriptor");
        exit(2);
    }
    return 0;
}