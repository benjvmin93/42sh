#include <err.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../utils/alloc.h"

int exec_command(char **argv)
{
    pid_t pid;
    int status = 0;
    fflush(NULL);
    if ((pid = fork()) < 0)
        err(errno, "Failed to create child process");

    if (pid == 0)
    {
        execvp(*argv, argv);
        warn("Failed to execute command %s", *argv);
        exit(127);
    }
    waitpid(pid, &status, 0);

    return WEXITSTATUS(status);
}
