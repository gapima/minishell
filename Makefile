NAME = minishell

CC = cc
CFLAGS = -g3 -lreadline -Wall -Wextra -Werror
RM = rm -f

INC = ./include
LIBFT = ./libft


HEADERS = -I $(INC)
LIBS = ./libft/libft.a

FILES = src/builtin/export.c src/builtin/echo.c src/builtin/pwd.c src/builtin/exit.c src/builtin/env.c src/builtin/unset.c src/builtin/cd.c src/builtin/run.c \
		src/main.c src/utils.c src/heredoc.c src/lexer.c src/lexer_utils.c src/shellzin.c src/parsing/parser.c src/ast/ast.c src/list_utils.c src/utils1.c \
		src/ast/redirection.c src/utils2.c src/ast/pipe.c src/ast/ast_utils.c src/ast/word_list.c src/lexer_utils1.c src/parsing/parser_utils.c \
		src/parsing/parser_utils1.c src/parsing/parser_utils2.c src/parsing/parser_pipe.c src/parsing/parser_redirect.c src/parsing/parser_word.c \


all: $(NAME)

$(NAME): $(OBJS)
	@make -C $(LIBFT)
	@make -C $(LIBFT) bonus
	@$(CC) $(FILES) -o $(NAME) $(CFLAGS) $(HEADERS) $(LIBS)
	@echo "SUCCESS!!"

clean:
	@make clean -C libft
	@echo "Objects cleaned!"

fclean: clean
	@make fclean -C libft
	@$(RM) $(NAME)
	@echo "Executable cleaned!"

re: fclean all

.PHONY: all clean fclean re
