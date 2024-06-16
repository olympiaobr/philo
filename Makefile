NAME	=	philo

CC		=	cc

CFLAGS = -Wextra -Wall -Werror -g -pthread

DEBUG	= -fsanitize=thread

SRC	=	src/main.c src/utils1.c\
		src/initialize.c src/actions.c\
		src/philo.c src/utils2.c\
		src/supervision.c

OBJS	=	$(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(LIGHT_GREEN)Compiling $(NAME)$(RES)"
	@$(CC) $(OBJS) $(INCLUDE) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)
	@echo "$(LIGHT_CYAN)Cleaning object files$(RES)"

fclean: clean
	rm -rf $(NAME)
	@echo "$(LIGHT_PURPLE)Cleaning $(NAME)$(RES)"

re: fclean all

.PHONY: all clean fclean re

LIGHT_GREEN			=	\033[0;38;5;82m
LIGHT_PURPLE		=	\033[0;38;5;177m
LIGHT_CYAN			=	\033[0;38;5;87m
RES				=	\033[0m
