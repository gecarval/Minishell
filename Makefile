# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/14 14:50:48 by gecarval          #+#    #+#              #
#    Updated: 2024/10/23 09:49:27 by gecarval         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Variables
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS = main.c ./srcs/free_handler.c ./srcs/parser_utils.c ./srcs/parser.c ./srcs/exec.c
OBJS = $(SRCS:.c=.o)
NAME = minishell
LIBFT = libft/libft.a

# Rules
all: $(LIBFT) $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -lreadline -L./libft -lft

$(LIBFT):
	make -C libft
	make bonus -C libft

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	make -C libft clean

fclean: clean
	rm -f $(NAME)
	make -C libft fclean

re: fclean all