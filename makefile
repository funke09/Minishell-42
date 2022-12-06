# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: macos <macos@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/16 00:18:30 by zcherrad          #+#    #+#              #
#    Updated: 2022/12/05 23:55:36 by macos            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRC = main.c env.c printferror.c norm_file.c excution.c utils.c\
 		export_unset.c built_in.c redirections.c \
		tokenz_utils.c toknezation.c tok_utils2.c \
		built_in_utils.c exc.c expantion.c ft_execve.c\
		utils_export.c ft_env.c analyse_line.c

OBJ = $(SRC:.c=.o)

HEADER = minishell.h


CC = gcc
LIB = libft/libft.a
FLAGS = -Werror -Wall -Wextra  -fsanitize=address 

all :$(NAME) 


$(NAME) : $(OBJ)
	@echo "\n"
	@echo "\033[0;32mCompiling libft..."
	@make -C libft/
	@echo "\033[0;32mCompiling minishell..."
	@$(CC) $(FLAGS) $(OBJ) -L $(shell brew --prefix readline)/lib -lreadline  $(LIB) -o $@ 
	
	@echo "\n\033[0mCompilation Done !"

%.o : %.c  $(HEADER)
	@printf "\033[0;33mcreating minishell objects... %-33.33s\r" $@
	$(CC) $(FLAGS) -I $(shell brew --prefix readline)/include -c $< -o $@
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
	@make -C libft/ fclean
	@echo "\nDeleting executable..."
	@echo "\033[0m"
re : fclean all
