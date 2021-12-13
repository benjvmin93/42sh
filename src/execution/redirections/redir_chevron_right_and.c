#define _POSIX_C_SOURCE 200809L

#include "redirections.h"

#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

int redir_chevron_right_and(struct ast_node *ast)
{
    struct ast_redir node = ast->data.ast_redir;
    int io_number;
    
    if (node.io_number != -1)
        io_number = node.io_number;
    else
        io_number = STDOUT_FILENO;
    
    if (*node.right == '-')
    {
        if (close(io_number) == -1)
            warn("%d", io_number);
        return 0;     
    }

    FILE *out = fopen(node.right, "w+");
    int out_dup;
    if ((out_dup = dup2(io_number, fileno(out)) == -1))
    {
        warnx("Failed to duplicate file descriptor");
        exit(0);
    }
    if (close(io_number) == -1)
    {
        warn("Failed to close file descriptor");
        exit(2);
    }

    return 0;
}
/*
int main(void)
{   
    struct ast_node cmd;
    cmd.data.ast_cmd.argv = xmalloc(3 * sizeof(char *));
    cmd.data.ast_cmd.argv[0] = "echo";
    cmd.data.ast_cmd.argv[1] = "teub";
    cmd.data.ast_cmd.argv[2] = NULL;
    struct ast_node ast;
    ast.data.ast_redir.io_number = 2;
    ast.data.ast_redir.right = "foo";
    ast.data.ast_redir.left = &cmd;

    int res = redir_chevron_right_and(&ast);

    free(cmd.data.ast_cmd.argv);
    return res;
}
*/