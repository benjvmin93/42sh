#include "exec.h"

int exec_node_pipeline(struct ast_node *ast)
{
    struct vector *v = ast->data.ast_pipeline.argv;

    printf("%ld\n",v->size);
    printf("TU AS REUSSI!\n");
    return 1;
}
