#pragma once

#include <utils/error.h>

enum error interruptible_readline(char *prompt, char **line);
void interruptible_readline_setup(void);
