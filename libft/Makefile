# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zcherrad <zcherrad@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/16 02:18:15 by zcherrad          #+#    #+#              #
#    Updated: 2021/11/25 04:29:44 by zcherrad         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
SRCS =ft_atoi.c\
ft_bzero.c\
ft_calloc.c\
ft_isalnum.c\
ft_isalpha.c\
ft_isascii.c\
ft_isdigit.c\
ft_strncpy.c\
ft_isprint.c\
ft_strcpy.c\
ft_strcmp.c\
ft_itoa.c\
ft_lstadd_back.c\
ft_lstadd_front.c\
ft_lstlast.c\
ft_lstnew.c\
ft_lstsize.c\
ft_memchr.c\
ft_memcmp.c\
ft_memcpy.c\
ft_memmove.c\
ft_memset.c\
ft_putchar_fd.c\
ft_putendl_fd.c\
ft_putnbr_fd.c\
ft_putstr_fd.c\
ft_split.c\
ft_strdel.c\
ft_strchr.c\
ft_strdup.c\
ft_striteri.c\
ft_strjoin.c\
ft_strlcat.c\
ft_strlcpy.c\
ft_strlen.c\
get_next_line.c\
get_line.c\
ft_strmapi.c\
ft_strncmp.c\
ft_strnstr.c\
ft_strrchr.c\
ft_strtrim.c\
ft_substr.c\
ft_tolower.c\
ft_toupper.c\

B_SRCS = ft_lstadd_back.c\
ft_lstadd_front.c\
ft_lstlast.c\
ft_lstnew.c\
ft_lstsize.c\
ft_lstdelone.c\
ft_lstclear.c\

OBJS = $(SRCS:.c=.o)
B_OBJS = $(B_SRCS:.c=.o)
CFLAGS = -Wall -Werror -Wextra
CC = gcc
RM = rm -f
AR = ar rc

all: $(NAME)

$(NAME):$(OBJS)
	$(AR) $(NAME) $(OBJS)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

bonus:$(B_OBJS) $(OBJS)
	$(AR) $(NAME) $(B_OBJS) $(OBJS)

clean:
	$(RM) $(OBJS) $(B_OBJS)

fclean:clean
	$(RM) $(NAME)

re: fclean all