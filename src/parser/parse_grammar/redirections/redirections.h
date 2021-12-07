#ifndef REDIRECTIONS_H
#define REDIRECTIONS_H

struct parse_ast *redir_chevron_left_and(struct lexer *lexer);
struct parse_ast *redir_chevron_left(struct lexer *lexer);
struct parse_ast *redir_chevron_left_right(struct lexer *lexer);
struct parse_ast *redir_chevron_right_and(struct lexer *lexer);
struct parse_ast *redir_chevron_right(struct lexer *lexer);
struct parse_ast *redir_chevron_right_or(struct lexer *lexer);
struct parse_ast *redir_double_chevron_left(struct lexer *lexer);
struct parse_ast *redir_double_chevron_left_dash(struct lexer *lexer);
struct parse_ast *redir_double_chevron_right(struct lexer *lexer);

#endif /* REDIRECTIONS_H */
