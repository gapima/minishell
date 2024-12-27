NAME = minishell

CC = cc
CFLAGS = -g3 -Wall -Wextra -Werror
RM = rm -f

INC = ./include
LIBFT = ./libft


HEADERS = -I $(INC)
LIBS = ./libft/libft.a

FILES = src/main.c
OBJS = $(FILES:%.c=%.o)

all: $(NAME)

%.o: %.c
	@$(CC) $(CFLAGS) $(HEADERS) -c $< -o $@

$(NAME): $(OBJS)
	@make -C $(LIBFT)
	@$(CC) $(OBJS) $(CFLAGS) $(HEADERS) -o $(NAME)
	@echo "SUCCESS!!"

clean:
	@$(RM) $(OBJS)
	@make clean -C libft
	@echo "Objects cleaned!"

fclean: clean
	@make fclean -C libft
	@$(RM) $(NAME)
	@echo "Executable cleaned!"

re: fclean all

.PHONY: all clean fclean re