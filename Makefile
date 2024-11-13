# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/14 14:50:48 by gecarval          #+#    #+#              #
#    Updated: 2024/11/13 11:26:17 by gecarval         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# VARIABLES
CC = cc
CFLAGS = -Wall -Wextra -Werror
NAME = minishell
SRCS =	main.c								\
		./srcs/free_handler.c				\
		./srcs/parser/parser_utils.c		\
		./srcs/parser/parser.c				\
		./srcs/parser/ft_parser_split.c		\
		./srcs/exec.c						\
		./srcs/built_in.c					\
		./srcs/env/env_handle.c				\
		./srcs/env/env_utils.c				\
		./srcs/env/env_utils2.c				\
		./srcs/expand_sign.c				\

OBJS = $(SRCS:.c=.o)
LIBFT = libft/libft.a

# RULES
all: $(LIBFT) $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -lreadline -L./libft -lft

$(LIBFT):
	make -C libft
	make bonus -C libft

clean:
	rm -f $(OBJS)
	make -C libft clean

fclean: clean
	rm -f $(NAME)
	make -C libft fclean

re: fclean all