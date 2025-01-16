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
DEBUGFLAGS	:= -g
CFLAGS		:= -Wall -Wextra -Werror
RM		:= rm -f
DEBUG		:=

SRCDIR		:= ./src
INCDIR		:= ./include

SRC	:=	$(SRCDIR)/fdf.c \
		$(SRCDIR)/map_handling.c \
		$(SRCDIR)/matrix.c

OBJ		:= $(SRC:.c=.o)

LIBFTDIR	:= ./lib/libft
LIBFT		:= $(LIBFTDIR)/libft.a
LIBFTH		:= $(LIBFTDIR)/libft.h
LIBMLXDIR	:= ./lib/MLX42
LIBMLX		:= $(LIBMLXDIR)/build/libmlx42.a
LIBMLXFLAGS	:= -ldl -lglfw -pthread -lm
INC		:= -I $(LIBFTDIR) -I $(INCDIR) -I $(LIBMLXDIR)/include

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT) $(LIBFTH) $(LIBMLX)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(LIBMLX) $(LIBMLXFLAGS) $(INC) -o $@ $(DEBUG)

$(LIBMLX):
	cmake $(LIBMLXDIR) -B $(LIBMLXDIR)/build && make -C $(LIBMLXDIR)/build -j4

$(LIBFT):
	make all -C $(LIBFTDIR)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)

clean:
	make clean -C $(LIBFTDIR)
	$(RM) $(OBJ)

fclean: clean
	make fclean -C $(LIBFTDIR)
	rm $(LIBMLX)
	$(RM) $(NAME)

re: fclean all

debug: CFLAGS += $(DEBUGFLAGS)
debug: fclean libftdebug all

libftdebug:
	make fclean -C $(LIBFTDIR)
	make debug -C $(LIBFTDIR)

.PHONY: clean fclean re all phony debug libftdebug
