#include "../parser/parser.h"

#include "redirections/redirections.h"

extern struct parse_ast *parser;

/**
 * redirection: [IONUMBER] '>' WORD
        | [IONUMBER] '<' WORD
        | [IONUMBER] '>>' WORD
        | [IONUMBER] '<<' HEREDOC
        | [IONUMBER] '<<-' HEREDOC
        | [IONUMBER] '>&' WORD
        | [IONUMBER] '<&' WORD
        | [IONUMBER] '>|' WORD
        | [IONUMBER] '<>' WORD
 */

struct function
{
    char *str;
    int (*redir)(struct ast_node *ast);
};

struct function redirs[] = { { ">", &redir_chevron_right },
                             { ">|", &redir_chevron_right_or },
                             { ">>", &redir_double_chevron_right },
                             { "<", &redir_chevron_left },
                             { "<&", &redir_chevron_left_and },
                             { ">&", &redir_chevron_right_and },
                             { "<>", &redir_chevron_left_right },
                             { "<<", &redir_double_chevron_left },
                             { "<<-", &redir_double_chevron_left_dash } };

int exec_node_redirection(struct ast_node *ast)
{
    int res;
    for (unsigned i = 0; i < sizeof(redirs) / sizeof(*redirs); i++)
    {
        if (!strcmp(ast->data.ast_redir.redirection, redirs[i].str))
        {
             res = redirs[i].redir(ast);
             break;
        }
    }
    return res;
}
