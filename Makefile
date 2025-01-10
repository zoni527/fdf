# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/07 15:58:04 by jvarila           #+#    #+#              #
#    Updated: 2025/01/10 19:03:36 by jvarila          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:= fdf

CC		:= cc
CFLAGS		:= -Wall -Wextra -Werror
RM		:= rm -f
DEBUG		:=

SRCDIR		:= ./src
INCDIR		:= ./include

SRC	:=	$(SRCDIR)/fdf.c

OBJ		:= $(SRC:.c=.o)

LIBFTDIR	:= ./lib/libft
LIBFT		:= $(LIBFTDIR)/libft.a
LIBFTH		:= $(LIBFTDIR)/libft.h
LIBMLXDIR	:= ./lib/MLX42
INC		:= -I$(LIBFTDIR) -I$(INCDIR) -I $(LIBMLXDIR)/include

all: $(NAME) libmlx

$(NAME): $(OBJ) $(LIBFT) $(LIBFTH)
	$(CC) $(CFLAGS) $(INC) $(OBJ) $(LIBFT) -o $@ $(DEBUG)

liblmx:
	cmake $(LIBMLXDIR) -B $(LIBMLXDIR)/build && make -C $(LIBMLXDIR)/build -j4

$(LIBFT):
	make $(LIBFT) -C $(LIBFTDIR)

%.o: %.c
	$(CC) -c $< -o $@ $(DEBUG)

clean:
	make clean -C $(LIBFTDIR)
	$(RM) $(OBJ)

fclean: clean
	make fclean -C $(LIBFTDIR)
	$(RM) $(NAME)

re: fclean all

.PHONY: clean fclean re all phony libmlx
