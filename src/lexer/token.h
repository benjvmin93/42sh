#ifndef TOKEN_H
#define TOKEN_H

enum token_type
{
    TOKEN_SEMICOLON, // ';'
    TOKEN_DOUBLE_SEMICOLON, // ';;'
    TOKEN_LINE_BREAK, // '\n'
    TOKEN_SIMPLE_QUOTE, // '''
    TOKEN_AND, // '&'
    TOKEN_NOT, // '!'
    TOKEN_DOUBLE_AND, // '&&'
    TOKEN_PIPE, // '|'
    TOKEN_DOUBLE_PIPE, // '||'
    TOKEN_LEFT_PARENTHESIS, // '('
    TOKEN_RIGHT_PARENTHESIS, // ')'
    TOKEN_LEFT_ACC, // '{'
    TOKEN_RIGHT_ACC, // '}'
    TOKEN_CHEVRON_RIGHT, // '>'
    TOKEN_CHEVRON_RIGHT_OR, // '>|'
    TOKEN_DOUBLE_CHEVRON_RIGHT, // '>>'
    TOKEN_CHEVRON_LEFT, // '<'
    TOKEN_CHEVRON_LEFT_AND, // '<&'
    TOKEN_CHEVRON_LEFT_AND_MINUS, // '<&-'
    TOKEN_CHEVRON_RIGHT_AND, // '>&'
    TOKEN_CHEVRON_RIGHT_AND_MINUS, // '>&-'
    TOKEN_CHEVRON_LEFT_RIGHT, // '<>'
    TOKEN_DOUBLE_CHEVRON_LEFT, // '<<'
    TOKEN_DOUBLE_CHEVRON_LEFT_DASH, // '<<-'
    TOKEN_EOF, // EOF
    TOKEN_WORD, // WORD
    TOKEN_IF, // 'if'
    TOKEN_THEN, // 'then'
    TOKEN_ELIF, // 'elif'
    TOKEN_ELSE, // 'else'
    TOKEN_FI, // 'fi'
    TOKEN_FOR, // 'for'
    TOKEN_WHILE, // 'while'
    TOKEN_UNTIL, // 'until'
    TOKEN_DO, // 'do'
    TOKEN_DONE, // 'done'
    TOKEN_CASE // 'case'
};

struct token
{
    enum token_type type;
    char *data;
};

struct token_model
{
    const char *str;
    enum token_type token_type;
};

enum token_type token_type(char *str);
struct token *token_new(char *input);
void token_free(struct token *token);

#endif /* ! LEXER_H */
