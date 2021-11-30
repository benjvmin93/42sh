#include "utils/alloc.h"

#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <err.h>

int exec_command(char **argv)
{
    char *binary_path = xmalloc((6 * strlen(argv[0])) + 1);
    binary_path = strcpy(binary_path, "/bin/");
    binary_path = strcat(binary_path, *argv);

    argv[0] = xrealloc(argv[0], strlen(binary_path) + 1);
    argv[0] = strcpy(argv[0], binary_path);

    pid_t pid;
    int status;
    if ((pid = fork()) < 0)
        err(2, "Failed to create child process");

    if (pid == 0)
    {
        if (execv(binary_path, argv) < 0)
            err(2, "Failed to execute command %s", binary_path);
    }
    else
    {
        while (wait(&status) != pid)
            ;
    }

    free(binary_path);
    
    return status;
}

int main(void)
{
    char **argv = xmalloc(3 * sizeof(char *));
    argv[0] = strdup("ls");
    argv[1] = strdup(".");
    argv[2] = NULL;

    
    exec_command(argv);
    free(argv[0]);
    free(argv[1]);
    free(argv);

}