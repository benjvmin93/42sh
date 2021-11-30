#include "../parser.h"

enum parser_status handle_parse_error(enum parser_status status,
                                      struct ast_node **res)
{
    warnx("unexpected token");
    // ast_free(*res);
    *res = NULL;
    return status;
}
