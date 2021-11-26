CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -Werror -g -fsanitize=address
OBJS=src/lexer/lexer.o src/lexer/token.o src/utils/clist.o src/basic_prompt/main.o

all: $(OBJS)
	$(CC) $(CFLAGS) -o lexer $(OBJS)

clean:
	$(RM) src/lexer/*.o src/utils/*.o lexer
