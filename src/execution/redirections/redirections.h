#ifndef REDIRECTIONS_H
#define REDIRECTIONS_H

#include "../exec.h"

int redir_chevron_left_and(struct ast_node *ast);
int redir_chevron_left(struct ast_node *ast);
int redir_chevron_left_right(struct ast_node *ast);
int redir_chevron_right_and(struct ast_node *ast);
int redir_chevron_right(struct ast_node *ast);
int redir_chevron_right_or(struct ast_node *ast);
int redir_double_chevron_right(struct ast_node *ast);

int my_redir(char **argv);

#endif /* REDIRECTIONS_H */
