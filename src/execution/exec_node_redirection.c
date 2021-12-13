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
                             { "<>", &redir_chevron_left_right } };

int exec_node_redirection(struct ast_node *ast)
{
    struct ast_redir redir = ast->data.ast_redir;
    // printf("redir: %s, right: %s\n", redir.redirection, redir.right);

    int res;
    for (unsigned i = 0; i < sizeof(redirs) / sizeof(*redirs); i++)
    {
        if (!strcmp(redir.redirection, redirs[i].str))
        {
            res = redirs[i].redir(ast);
            break;
        }
    }

    return res;
}
