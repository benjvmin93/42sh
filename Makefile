CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -Werror -g -fsanitize=address
OBJS=src/lexer/lexer.o src/lexer/token.o src/parser/parse_grammar/*.o src/utils/clist.o src/utils/vector/vector.o src/ast/ast.o src/basic_prompt/main.o

all: $(OBJS)
	$(CC) $(CFLAGS) -o 42sh $(OBJS)

clean:
	$(RM) src/ast/*.o src/utils/vector/*.o src/parser/parse_grammar/*.o src/lexer/*.o src/parser/*.o src/basic_prompt/main.o src/utils/*.o 42sh
