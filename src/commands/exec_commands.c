#include <err.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../utils/alloc.h"

int exec_command(char **argv)
{
    pid_t pid;
    int status = 0;
    if ((pid = fork()) < 0)
        err(2, "Failed to create child process");

    if (pid == 0)
    {
        if (execvp(*argv, argv) == -1)
            err(2, "Failed to execute command %s", *argv);
    }
    else
    {
        while (wait(&status) != pid)
            ;
    }

    return status;
}
