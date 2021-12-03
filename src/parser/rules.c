#include "rules.h"

#include "../lexer/lexer.h"
#include "parser.h"

int follow_list(struct token *token)
{
    return (token->type == TOKEN_LINE_BREAK) || (token->type == TOKEN_EOF);
}

int follow_andor(struct token *token)
{
    //token->type == TOKEN_SEMICOLON
    return (token->type == TOKEN_AND) || follow_list(token);
}

int follow_pipeline(struct token *token)
{
    return (token->type == TOKEN_DOUBLE_AND)
        || (token->type == TOKEN_DOUBLE_PIPE) || (follow_andor(token));
}
// command, simple command
int follow_cmd(struct token *token)
{
    return (token->type == TOKEN_PIPE) || (follow_pipeline(token));
}
// fundec, shellcmd
int follow_shellcmd_fundec(struct token *token)
{
    return follow_cmd(token) || follow_redirection(token);
}

int follow_redirection(struct token *token)
{
    return follow_cmd(token) || follow_prefix(token) || follow_elt(token);
}

int follow_prefix(struct token *token)
{
    return follow_cmd(token);
}

int follow_elt(struct token *token)
{
    return follow_cmd(token);
}

int follow_compoundlist(struct token *token)
{
    return (token->type == TOKEN_RIGHT_ACC)
        || (token->type == TOKEN_RIGHT_PARENTHESIS)
        || (token->type == TOKEN_THEN) || follow_elseclause(token);
}

int follow_rule(struct token *token)
{
    return follow_shellcmd_fundec(token);
}

int follow_elseclause(struct token *token)
{
    return (token->type == TOKEN_FI);
}
