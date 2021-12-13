#include "exec.h"

int exec_node_pipeline(struct ast_node *ast)
{
    struct vector *v = ast->data.ast_pipeline.argv;

    printf("%ld\n", v->size);
    printf("TU AS REUSSI!\n");

    /*
       struct vector *v = ast->data.ast_pipeline.argv;
       size_t i = 0;
       for (; i < v->size - 1; i++)
       {
       int fd[2];
       pipe(fd);

       if (!fork())
       {
       dup2(fd[1], 1);
       exec(v->data[i]);
    //perror("exec");
    //abort();
    }

    dup2(fd[0], 0);
    close(fd[1]);
    }

    exec(v->data[i]);
    //perror("exec");
    //abort();
    */

    return 1;
}
