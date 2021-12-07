#ifndef LIST_H
#define LIST_H
#include <stdlib.h>

struct clist
{
    char *data;
    size_t size;
    size_t size_mal;
};

struct clist *init_clist();
void print_clist(struct clist *string);
struct clist *cp_list(struct clist *string);
struct clist *app_str(struct clist *string, char *str);
struct clist *app_char(struct clist *string, char elt);
void free_clist(struct clist *string);
struct clist *add_spacestr(struct clist *sentence, char *str);
struct clist *new_clist(struct clist *str);

#endif /* ! LIST_H */
