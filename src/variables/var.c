#include "var.h"

#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "../parser/parser.h"
#include "../utils/vector/vector.h"

extern struct variables *variables;

static int random_nbr(void)
{
    srand(time(NULL));
    int r = rand();

    return r;
}

struct vector *add_specialvar(struct vector *vector, int argc, char **argv)
{
    // $? get the last res status
    struct var *question_mark = xmalloc(sizeof(struct var));
    question_mark->name = strdup("?");
    question_mark->value = xmalloc(8);
    sprintf(question_mark->value, "%d", 0);
    vector = vector_append(vector, question_mark);

    // $$ get PID ->use getpid(), change when fork
    struct var *pid = xmalloc(sizeof(struct var));
    int curr_pid = getpid();
    pid->name = strdup("$");
    pid->value = xmalloc(8);
    sprintf(pid->value, "%d", curr_pid);
    vector = vector_append(vector, pid);

    // $RANDOM get a RANDOM number
    struct var *random = xmalloc(sizeof(struct var));
    random->name = strdup("RANDOM");
    random->value = xmalloc(64 * sizeof(int));
    sprintf(random->value, "%d", random_nbr());
    vector = vector_append(vector, random);

    // $UID get UID
    struct var *uid = xmalloc(sizeof(struct var));
    int curr_uid = getuid();
    uid->name = strdup("UID");
    uid->value = xmalloc(64);
    sprintf(uid->value, "%d", curr_uid);
    vector = vector_append(vector, uid);

    // $OLDPWD get the pwd set before the last cd command
    struct var *opwd = xmalloc(sizeof(struct var));
    opwd->name = strdup("OLDPWD");
    opwd->value = xmalloc(4096);
    getcwd(opwd->value, 4096);
    vector = vector_append(vector, opwd);

    // $IFS
    struct var *ifs = xmalloc(sizeof(struct var));
    ifs->name = strdup("IFS");
    ifs->value = strdup("");
    vector = vector_append(vector, ifs);

    // $@ and $* list of args -> *char argv[]
    struct var *list_arg = xmalloc(sizeof(struct var));
    list_arg->name = strdup("@");
    list_arg->value = xmalloc(4096);
    *list_arg->value = '\0';

    for (int i = 1; i < argc; i++)
        list_arg->value = strcat(list_arg->value, argv[i]);
    vector = vector_append(vector, list_arg);

    struct var *list_arg2 = xmalloc(sizeof(struct var));
    list_arg2->name = strdup("*");
    list_arg2->value = xmalloc(4096);
    *list_arg2->value = '\0';

    for (int i = 1; i < argc; i++)
        list_arg2->value = strcat(list_arg2->value, argv[i]);
    vector = vector_append(vector, list_arg2);

    // $# nbr d'argument -> int argc
    struct var *nbr_arg = xmalloc(sizeof(struct var));
    nbr_arg->name = strdup("#");
    nbr_arg->value = xmalloc(8);
    sprintf(nbr_arg->value, "%d", argc);
    vector = vector_append(vector, nbr_arg);

    // $1 ... $n arg n -> argv[1]; argv[2]; argv[3]
    for (int i = 1; i < argc; i++)
    {
        struct var *narg = xmalloc(sizeof(struct var));

        narg->name = xmalloc(8);
        narg->value = strdup(argv[i]);

        sprintf(narg->name, "%d", i);

        vector = vector_append(vector, narg);
    }

    return vector;
}

struct variables *init_variables(int argc, char **argv)
{
    variables = xmalloc(sizeof(struct variables));
    variables->variables = vector_init(1, sizeof(struct var));
    variables->variables = add_specialvar(variables->variables, argc, argv);

    return variables;
}
