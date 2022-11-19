# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: macos <macos@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/16 00:18:30 by zcherrad          #+#    #+#              #
#    Updated: 2022/11/19 15:53:56 by macos            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRC = main.c env.c printferror.c teest.c

OBJ = $(SRC:.c=.o)

HEADER = minishell.h

CC = gcc
LIB = libft/libft.a
FLAGS = -Wall -Wextra -Werror -fsanitize=address

all :$(NAME) 


$(NAME) : $(OBJ)
	@echo "\n"
	@echo "\033[0;32mCompiling libft..."
	@make -C libft/
	@echo "\033[0;32mCompiling minishell..."
	@$(CC) $(FLAGS) $(OBJ) -lreadline -o $(NAME) $(LIB)
	@echo "\n\033[0mCompilation Done !"

%.o : %.c  $(HEADER)
	@printf "\033[0;33mcreating minishell objects... %-33.33s\r" $@
	@$(CC) $(FLAGS) -c $< -o $@
clean :
	@echo "\nDeleting minishell objects..."
	@rm -f $(OBJ)
	@echo "\nDeleting libft objects..."
	@make -C libft/ clean
	@echo "\033[0m"

fclean : clean
	@echo "\nDeleting minishell objects..."
	@rm -f $(NAME)
	@echo "\nDeleting libft objects..."
	@make -C libft/ clean
	@echo "\nDeleting executable..."
	@echo "\033[0m"
re : fclean all
