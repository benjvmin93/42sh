#ifndef VAR_H
#define VAR_H

enum special_var
{
    VAR_RES, // $?
    VAR_PID,
    VAR_RANDOM,
    VAR_UID,
};

struct variables *init_variables(int argc, char **argv);

#endif
