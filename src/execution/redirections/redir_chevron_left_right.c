#define _POSIX_C_SOURCE 200809L

#include "redirections.h"

#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

int redir_chevron_left_right(struct ast_node *ast)
{
    struct ast_redir node = ast->data.ast_redir;
    int io_number;

    if (node.io_number != -1)
        io_number = node.io_number;
    else
        io_number = STDIN_FILENO;

    FILE *out = fopen(node.right, "w+");
    int out_dup;
    if ((out_dup = dup2(io_number, fileno(out)) == -1))
    {
        warnx("Failed to duplicate file descriptor");
        exit(2);
    }

    if (close(io_number) == -1)
        warn("Failed to close file descriptor");

    return 0;
}
