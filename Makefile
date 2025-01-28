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

SRCDIR		:= ./src
INCDIR		:= ./include

SRC	:=	$(SRCDIR)/fdf.c \
		$(SRCDIR)/map_handling.c \
		$(SRCDIR)/world_manipulation.c \
		$(SRCDIR)/world_centering.c \
		$(SRCDIR)/world_max.c \
		$(SRCDIR)/world_min.c \
		$(SRCDIR)/view_manipulation.c \
		$(SRCDIR)/view_centering.c \
		$(SRCDIR)/view_min_max.c \
		$(SRCDIR)/view_projections.c \
		$(SRCDIR)/2d_point_manipulation.c \
		$(SRCDIR)/3d_point_manipulation.c \
		$(SRCDIR)/background_and_map_drawing.c \
		$(SRCDIR)/input_01.c \
		$(SRCDIR)/input_02.c \
		$(SRCDIR)/line_segment_drawing.c \
		$(SRCDIR)/pixels_and_colors.c \
		$(SRCDIR)/default_gradient.c \
		$(SRCDIR)/non_uniform_scaling.c \
		$(SRCDIR)/data_printing.c

OBJ		:= $(SRC:.c=.o)

LIBFTDIR	:= ./lib/libft
LIBFT		:= $(LIBFTDIR)/libft.a
LIBFTH		:= $(LIBFTDIR)/libft.h
LIBMLXDIR	:= ./lib/MLX42
LIBMLX		:= $(LIBMLXDIR)/build/libmlx42.a
LIBMLXFLAGS	:= -ldl -lglfw -pthread -lm
INC		:= -I $(LIBFTDIR) -I $(INCDIR) -I $(LIBMLXDIR)/include
FDFH		:= $(INCDIR)/fdf.h

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT) $(LIBFTH) $(LIBMLX) $(FDFH)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(LIBMLX) $(LIBMLXFLAGS) $(INC) -o $@

$(LIBMLX):
	cmake $(LIBMLXDIR) -B $(LIBMLXDIR)/build && make -C $(LIBMLXDIR)/build -j4

$(LIBFT):
	make all -C $(LIBFTDIR)

%.o: %.c $(FDFH)
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)

clean:
	make clean -C $(LIBFTDIR)
	make clean -C $(LIBMLXDIR)/build
	$(RM) $(OBJ)

fclean: clean
	make fclean -C $(LIBFTDIR)
	$(RM) $(LIBMLX)
	$(RM) $(NAME)

re: fclean all

debug: CFLAGS += $(DEBUGFLAGS)
debug: fclean libftdebug all

libftdebug:
	make fclean -C $(LIBFTDIR)
	make debug -C $(LIBFTDIR)

.PHONY: clean fclean re all phony debug libftdebug
