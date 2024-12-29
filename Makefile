NAME = minishell

CC = cc
CFLAGS = -g3 -lreadline -Wall -Wextra -Werror
RM = rm -f

INC = ./include
LIBFT = ./libft


HEADERS = -I $(INC)
LIBS = ./libft/libft.a

FILES = src/main.c src/utils0.c src/lexer.c src/utils_lexer0.c src/shellzin.c

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
