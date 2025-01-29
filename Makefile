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
#------------------------------------------------------------------------------#
SRCDIR		:= ./src
INCDIR		:= ./include
OBJDIR		:= ./obj
LIBFTDIR	:= ./lib/libft
LIBMLXDIR	:= ./lib/MLX42
#------------------------------------------------------------------------------#
SRC	:=	fdf.c \
		map_handling.c \
		world_manipulation.c \
		world_centering.c \
		world_max.c \
		world_min.c \
		view_manipulation.c \
		view_centering.c \
		view_min_max.c \
		view_projections.c \
		2d_point_manipulation.c \
		3d_point_manipulation.c \
		background_and_map_drawing.c \
		input_01.c \
		line_segment_drawing.c \
		pixels_and_colors.c \
		default_gradient.c \
		data_printing.c

OBJ		:= $(addprefix $(OBJDIR)/, $(SRC:.c=.o))
#------------------------------------------------------------------------------#
BNAME		:= fdf_bonus
BONUSSRCDIR	:= ./src_bonus
BOBJDIR		:= ./obj_bonus

BONUSSRC :=	fdf_bonus.c \
		map_handling_bonus.c \
		world_manipulation_bonus.c \
		world_centering_bonus.c \
		world_max_bonus.c \
		world_min_bonus.c \
		view_manipulation_bonus.c \
		view_centering_bonus.c \
		view_min_max_bonus.c \
		view_projections_bonus.c \
		2d_point_manipulation_bonus.c \
		3d_point_manipulation_bonus.c \
		background_and_map_drawing_bonus.c \
		input_01_bonus.c \
		input_02_bonus.c \
		line_segment_drawing_bonus.c \
		pixels_and_colors_bonus.c \
		default_gradient_bonus.c \
		non_uniform_scaling_bonus.c \
		data_printing_bonus.c

BOBJ	:= $(addprefix $(BOBJDIR)/, $(BONUSSRC:.c=.o))
#------------------------------------------------------------------------------#
LIBFT		:= $(LIBFTDIR)/libft.a
LIBFTH		:= $(LIBFTDIR)/libft.h
LIBMLX		:= $(LIBMLXDIR)/build/libmlx42.a
LIBMLXFLAGS	:= -ldl -lglfw -pthread -lm
INC		:= -I $(LIBFTDIR) -I $(INCDIR) -I $(LIBMLXDIR)/include
FDFH		:= $(INCDIR)/fdf.h
#------------------------------------------------------------------------------#
all: $(NAME)

$(NAME): $(LIBMLX) $(OBJDIR) $(OBJ) $(LIBFT) $(LIBFTH) $(FDFH)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(LIBMLX) $(LIBMLXFLAGS) $(INC) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(FDFH)
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(LIBMLX): $(LIBMLXDIR)
	cmake $(LIBMLXDIR) -B $(LIBMLXDIR)/build && make -C $(LIBMLXDIR)/build -j4

$(LIBMLXDIR):
	git clone https://github.com/codam-coding-college/MLX42.git $(LIBMLXDIR)

$(LIBFT):
	make all -C $(LIBFTDIR)

clean:
	make clean -C $(LIBFTDIR)
	make clean -C $(LIBMLXDIR)/build
	rm -rf $(OBJDIR)
	rm -rf $(BOBJDIR)

fclean: clean
	make fclean -C $(LIBFTDIR)
	rm -f $(LIBMLX)
	rm -f $(NAME)
	rm -f $(BNAME)

re: fclean all
#------------------------------------------------------------------------------#
bonus: $(BNAME)

$(BNAME): $(LIBMLX) $(BOBJDIR) $(BOBJ) $(LIBFT) $(LIBFTH) $(FDFH)
	$(CC) $(CFLAGS) $(BOBJ) $(LIBFT) $(LIBMLX) $(LIBMLXFLAGS) $(INC) -o $@

$(BOBJDIR)/%.o: $(BONUSSRCDIR)/%.c $(FDFH)
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(BOBJDIR):
	mkdir -p $(BOBJDIR)
#------------------------------------------------------------------------------#
debug: CFLAGS += $(DEBUGFLAGS)
debug: fclean libftdebug all

libftdebug:
	make fclean -C $(LIBFTDIR)
	make debug -C $(LIBFTDIR)
#------------------------------------------------------------------------------#
.PHONY: clean fclean re all phony debug libftdebug bonus
#------------------------------------------------------------------------------#
