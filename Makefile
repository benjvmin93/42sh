CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -Werror -g -fsanitize=address
OBJS=src/lexer/lexer.o src/lexer/token.o src/utils/clist.o src/parser/parser.o src/basic_prompt/main.o

all: $(OBJS)
	$(CC) $(CFLAGS) -o 42sh $(OBJS)

clean:
	$(RM) src/lexer/*.o src/parser/*.o src/basic_prompt/main.o src/utils/*.o 42sh
